package edu.iupui.iac.msalignplus.controller;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.ModelAndView;

import edu.iupui.iac.msalignplus.action.Commands;
import edu.iupui.iac.msalignplus.action.FileUtils;
import edu.iupui.iac.msalignplus.dao.MsDeconvArgument;
import edu.iupui.iac.msalignplus.dao.RunningInfo;
import edu.iupui.iac.msalignplus.dao.TopIDargument;
import edu.iupui.iac.msalignplus.task.*;

@Controller
public class RunApplicationMsAlignPlus {
	@Autowired
	private HttpServletRequest request;

	@RequestMapping("process")
	public String Porcess(String title, String protitle, String fragMeth,
			int error, String cpg, String cType, double cutValue, int nuPTM,
			String sType, String nptm1, String nptm2, String nptm3,
			int maxptmmass, @RequestParam("specFile") MultipartFile specFile,
			@RequestParam("dataFile") MultipartFile dataFile) {

		int id = FileUtils.getTaskId(request.getSession().getServletContext()
				.getRealPath("/")
				+ "tool/taskids.xml");
		String rootPath = request.getSession().getServletContext()
				.getRealPath("/");
		String processingPath = "result/TASK" + id + "/";

		new File(rootPath + processingPath).mkdir();
		// String datafilefolder = "in/";
		// new File(rootPath+processingPath).mkdir();

		if (!specFile.isEmpty()) {
			try {
				String filePath = rootPath + processingPath
						+ specFile.getOriginalFilename();
				specFile.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		if (!dataFile.isEmpty()) {
			try {
				String filePath = rootPath + processingPath
						+ dataFile.getOriginalFilename();
				dataFile.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		List<String> nptms = new ArrayList<String>();
		if (null != nptm1) {
			nptms.add(nptm1);
		}
		if (null != nptm2) {
			nptms.add(nptm2);
		}
		if (null != nptm3) {
			nptms.add(nptm3);
		}
		if (null == sType) {
			sType = "TARGET";
		}
		TopIDargument arguments = new TopIDargument(title, protitle, fragMeth,
				specFile.getOriginalFilename(), dataFile.getOriginalFilename(),
				nptms, error, cpg, sType, cType, cutValue, nuPTM, maxptmmass);

		TopIDargument.saveArgument(rootPath + processingPath, arguments);

		FileUtils.saveTimeFlag(rootPath + processingPath);

		FileUtils.addTaskList(id + "", rootPath + processingPath, rootPath
				+ "tool/tasklist.xml");

		return "subsucc";
	}

	@RequestMapping("process2")
	public String Porcess(String title2, String protitle2, String centroided,
			String output, int maxcharge, double tolerance, double ratio,
			String peaks, String missing,
			@RequestParam("specFile2") MultipartFile specFile2) {

		int id = FileUtils.getTaskId(request.getSession().getServletContext()
				.getRealPath("/")
				+ "tool/taskids.xml");
		String rootPath = request.getSession().getServletContext()
				.getRealPath("/");
		String processingPath = "result/TASK" + id + "/";

		new File(rootPath + processingPath).mkdir();
		// String datafilefolder = "in/";
		// new File(rootPath+processingPath).mkdir();

		if (!specFile2.isEmpty()) {
			try {
				String filePath = rootPath + processingPath
						+ specFile2.getOriginalFilename();
				specFile2.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		MsDeconvArgument arguments = new MsDeconvArgument(title2, protitle2,
				centroided, specFile2.getOriginalFilename(), output, maxcharge,
				tolerance, ratio, peaks, missing);

		MsDeconvArgument.saveArgument(rootPath + processingPath, arguments);

		FileUtils.saveTimeFlag(rootPath + processingPath);

		FileUtils.addTaskList(id + "", rootPath + processingPath, rootPath
				+ "tool/tasklist.xml");

		return "subsucc";
	}

	@RequestMapping("deleteTask")
	public String deleteTask(String delName, String isRunning) {
		String rootPath = request.getSession().getServletContext()
				.getRealPath("/")
				+ "result/";
		String os = System.getProperties().get("os.name").toString();
		if (os.startsWith("Win")) {
			try {
				if (isRunning.equalsIgnoreCase("1")) {
					String prog_name = MsDeconvArgument.getProgramName(rootPath
							+ delName + "/", "arguments.xml");
					if (prog_name.equals("topid")) {
						Commands.RunCommand("cmd /c taskkill /f /im topid.exe ");

						FileUtils.appendFile(rootPath + delName + "/log.txt",
								"[Force stopp]");
						TopIDargument arg = TopIDargument.getArgument(rootPath
								+ delName + "/", "arguments.xml");
						arg.setEnd(System.currentTimeMillis());
						TopIDargument.saveArgument(rootPath + delName + "/",
								arg);
					} else if (prog_name.equals("msdeconv")) {
						// RunMsDeconv.stopme();
						if (!RunMsDeconv.checkThread()) {
							System.out.println("[Force stopp]");
							Commands.RunCommand("cmd /c taskkill /f /im msconvert.exe ");
							FileUtils.appendFile(rootPath + delName
									+ "/log.txt", "[Force stopp]");
						} else {
							System.out.println("[Force stopp]");
							FileUtils.appendFile(rootPath + delName
									+ "/log.txt", "[Force stopp]");
						}

						MsDeconvArgument arg = MsDeconvArgument.getArgument(
								rootPath + delName + "/", "arguments.xml");
						arg.setEnd(System.currentTimeMillis());
						MsDeconvArgument.saveArgument(rootPath + delName + "/",
								arg);
					}

				} else {
					FileUtils.delTaskInfo(delName.replace("TASK", ""), request
							.getSession().getServletContext().getRealPath("/")
							+ "tool/tasklist.xml");
					Commands.RunCommand("cmd /c cd " + rootPath
							+ " && rd /s /q " + delName);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return "redirect:/taskList.do";
	}

	@RequestMapping("runningPercent")
	public void processInfoRow(String name, HttpServletResponse response) {

		String rootPath = request.getSession().getServletContext()
				.getRealPath("/");

		String prog_name = MsDeconvArgument.getProgramName(rootPath + "result/"
				+ name + "/", "arguments.xml");
		// System.out.println(prog_name);

		File taskEndFile = new File(rootPath + "result/" + name + "/log.txt");
		double per = 0.00;
		String status = "Waiting";
		int change = 1;
		if (taskEndFile.exists()) {
			per = 0.01;
			FileReader reader = null;
			BufferedReader br = null;
			// read file
			try {
				reader = new FileReader(rootPath + "result/" + name
						+ "/log.txt");
				br = new BufferedReader(reader);
				String str = null;
				if (prog_name.equals("topid")) {
					TopIDargument arg = TopIDargument.getArgument(rootPath
							+ "result/" + name + "/", "arguments.xml");
					while ((str = br.readLine()) != null) {
						if (str.contains("Exectutive directory is")) {
							per = 0.01;
						}

						if (str.contains("Zero ptm searching is processing")) {
							String temp = str.replace(
									"Zero ptm searching is processing ", "");
							temp = temp.replace(" spectra.", "");
							String[] array = temp.split("of");
							int a = Integer.parseInt(array[0].replace(" ", ""));
							int b = Integer.parseInt(array[1].replace(" ", ""));
							per = 0.01 + a * 1500 / b / 100.0;
						}
						if (str.contains("Fast filtering block")
								&& str.contains("is processing")) {
							// Fast filtering block 1 is processing 2987 of 3474
							// spectra.
							String temp = str.replace("Fast filtering block",
									"");
							temp = temp.substring(
									temp.indexOf("is processing "),
									temp.length());
							temp = temp.replace("is processing ", "");
							temp = temp.replace(" spectra.", "");
							String[] array = temp.split("of");
							int a = Integer.parseInt(array[0].replace(" ", ""));
							int b = Integer.parseInt(array[1].replace(" ", ""));
							per = 15 + a * 3000 / b / 100.0;
						}
						if (str.contains("Ptm searching is processing")) {
							String temp = str.replace(
									"Ptm searching is processing ", "");
							temp = temp.replace(" spectra.", "");
							String[] array = temp.split("of");
							int a = Integer.parseInt(array[0].replace(" ", ""));
							int b = Integer.parseInt(array[1].replace(" ", ""));
							per = 45 + a * 3000 / b / 100.0;
						}
						if (str.contains("E-value computation is processing")) {
							String temp = str.replace(
									"E-value computation is processing ", "");
							temp = temp.replace(" spectra.", "");
							String[] array = temp.split("of");
							int a = Integer.parseInt(array[0].replace(" ", ""));
							int b = Integer.parseInt(array[1]
									.split("scan number")[0].replace(" ", ""));
							per = 75 + a * 2200 / b / 100.0;
						}
						if (str.contains("Top selector")) {
							per = 97;
						}
						if (str.contains("Table output")) {
							per = 98;
						}
						if (str.contains("XalanTransformer")) {
							per = 99;
						}
						if (str.contains("proteins.xml")
								|| str.contains("Identification End!")) {
							per = 100;
						}
						if (str.contains("[Force stopp]")) {
							status = "force stop";
						}
						if (str.contains("[Exception]")) {
							status = "error";
						}
					}
					if (per == 100) {
						status = "finished";
					} else if (RunningInfo.taskName.contains(name)) {
						status = "running";
					} else {
						if (!status.equalsIgnoreCase("force stop")) {
							status = "error";
						}
					}
					if (status.equalsIgnoreCase("error") && arg.getEnd() == 0) {
						status = "unexpected stop";
						arg.setEnd(arg.getStart());
					}

					String per_str = per + "";
					if (per_str.length() == per_str.indexOf(".") + 2) {
						per_str += "0";
					}
					if (per_str.length() > per_str.indexOf(".") + 2) {
						per_str = per_str
								.substring(0, per_str.indexOf(".") + 3);
					}

					response.getWriter().write(
							per_str + " %," + status + "," + change + ","
									+ arg.getCostString());
				} else if (prog_name.equals("msdeconv")) {
					MsDeconvArgument arg = MsDeconvArgument.getArgument(
							rootPath + "result/" + name + "/", "arguments.xml");
					boolean reading = true;
					per = 10.00;
					while (((str = br.readLine()) != null) && reading) {
						if (str.contains("parameters end")) {
							per = 10.01;
							status = "running";
						} else if (str.startsWith("Processing spectrum")) {
							String[] temp = str.split(" ");
							str = temp[3];
							per = Double.parseDouble(str.substring(0,
									str.length() - 1));
							per = 10 + per * 0.9;
							status = "running";

						}

						if (str.contains("[Force stopp]")) {
							status = "force stop";
							reading = false;
						}
						if (str.contains("[Exception]")) {
							status = "error";
							reading = false;
						}
					}
					if (!status.equalsIgnoreCase("force stop")
							&& !status.equalsIgnoreCase("error")) {
						if (per == 0.01) {
							status = "waiting";
						} else if (per > 0.01 && per <= 99) {
							status = "running";
						} else if (per > 99.0) {
							status = "finished";
							arg.setEnd(System.currentTimeMillis());
						} else if (RunningInfo.taskName.contains(name)
								&& status.equalsIgnoreCase("waiting")) {
							status = "running";
						} else {
							if (!status.equalsIgnoreCase("force stop")) {
								status = "error";
							}
						}
					}
					if (status.equalsIgnoreCase("error") && arg.getEnd() == 0) {
						status = "unexpected stop";
						arg.setEnd(arg.getStart());
					}

					String per_str = per + "";
					if (per_str.length() == per_str.indexOf(".") + 2) {
						per_str += "0";
					}
					if (per_str.length() > per_str.indexOf(".") + 2) {
						per_str = per_str
								.substring(0, per_str.indexOf(".") + 3);
					}
					arg.setEnd(System.currentTimeMillis());
					response.getWriter().write(
							per_str + " %," + status + "," + change + ","
									+ arg.getCostString());
				}

			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				if (reader != null)
					try {
						reader.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				if (br != null)
					try {
						br.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
			}
		} else {
			try {
				response.getWriter().write("0.00 %,waiting,0,0:00:00");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	@RequestMapping("checkZip")
	public ModelAndView checkZip(String path) {
		ModelAndView ma = new ModelAndView("zipping");
		ma.addObject("path", path);
		return ma;
	}

	@RequestMapping("zipping")
	public void zipping(String path) throws Exception {
		String rootPath = request.getSession().getServletContext()
				.getRealPath("/")
				+ "result/" + path + "/";
		File file = new File(rootPath + path + ".zip");
		if (!file.exists()) {
			FileUtils.zip(
					request.getSession().getServletContext().getRealPath("/")
							+ "temp/" + path + ".zip", rootPath);
			Commands.RunCommand("cmd /c move "
					+ request.getSession().getServletContext().getRealPath("/")
					+ "temp\\" + path + ".zip " + rootPath);
		}
	}

	@RequestMapping("zipover")
	public void zipover(String path, HttpServletResponse response)
			throws Exception {
		String rootPath = request.getSession().getServletContext()
				.getRealPath("/")
				+ "result/" + path + "/";
		File file = new File(rootPath + path + ".zip");
		if (file.exists()) {
			response.getWriter().write("1");
		} else {
			response.getWriter().write("0");
		}
	}

}

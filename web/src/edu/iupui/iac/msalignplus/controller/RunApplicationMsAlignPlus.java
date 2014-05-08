package edu.iupui.iac.msalignplus.controller;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.w3c.dom.Document;

import edu.iupui.iac.msalignplus.action.Commands;
import edu.iupui.iac.msalignplus.action.FileUtils;
import edu.iupui.iac.msalignplus.dao.Argument;
import edu.iupui.iac.msalignplus.dao.RunningInfo;

@Controller
public class RunApplicationMsAlignPlus {
	@Autowired  
	private  HttpServletRequest request;
	
	@RequestMapping("process")
	public String Porcess(String title, String fragMeth, int error, String cpg,
			String cType, double cutValue, int nuPTM, String sType,
			String nptm1,String nptm2, String nptm3,
			@RequestParam("specFile") MultipartFile specFile,
			@RequestParam("dataFile") MultipartFile dataFile) {
		
		int id = FileUtils.getTaskId(request.getSession().getServletContext().getRealPath("/")+"tool/taskids.xml");
		String rootPath = request.getSession().getServletContext().getRealPath("/");
		String processingPath = "result/TASK"+id+"/";
		
		new File(rootPath+processingPath).mkdir();
//		String datafilefolder = "in/";
//		new File(rootPath+processingPath).mkdir();
		
		if (!specFile.isEmpty()) {
			try {
				String filePath = rootPath+processingPath+ specFile.getOriginalFilename();
				specFile.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		if (!dataFile.isEmpty()) {
			try {
				String filePath = rootPath+processingPath+ dataFile.getOriginalFilename();
				dataFile.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		List<String> nptms = new ArrayList<String>();
		if(null != nptm1){
			nptms.add(nptm1);
		}
		if(null != nptm2){
			nptms.add(nptm2);
		}
		if(null != nptm3){
			nptms.add(nptm3);
		}
		if(null == sType){
			sType="TARGET";
		}
		Argument arguments = new Argument(title, fragMeth,
				specFile.getOriginalFilename(), dataFile.getOriginalFilename(),
				nptms, error, cpg, sType, cType, cutValue, nuPTM);
		
		FileOutputStream out = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder builder;
			
			builder = factory.newDocumentBuilder();

			Document document = builder.newDocument();
			document.appendChild(arguments.toElement(document));
			TransformerFactory transFactory = TransformerFactory.newInstance();
			Transformer transFormer = transFactory.newTransformer();
			DOMSource domSource = new DOMSource(document);
			File file = new File(rootPath+processingPath+"arguments.xml");
			if (!file.exists()) {
				file.createNewFile();
			}
			out = new FileOutputStream(file);
			StreamResult xmlResult = new StreamResult(out);
			transFormer.transform(domSource, xmlResult);
//			System.out.println(file.getAbsolutePath());
		} catch (ParserConfigurationException | IOException
				| TransformerException e) {
			e.printStackTrace();
		}
		finally{
			if(out!=null){
				try {
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		FileUtils.addTaskList(id+"",rootPath+processingPath,rootPath+"tool/tasklist.xml");
		
//		String os = System.getProperties().get("os.name").toString();
//		System.out.println(os);
//		String commandLine="";
//		if (os.startsWith("Win")) {
//			try {
//				commandLine = "cmd /c cd " + rootPath + " && xcopy tool "
//						+ processingPath.replace("/", "\\") + " /y /e";
//
//				Commands.RunCommand(commandLine,true);
//
//				commandLine = "cmd /c cd " + rootPath + processingPath + " && proteomics_cpp.exe>log.txt";
//				Commands.RunCommand(commandLine,true);
//				
//				commandLine = "cmd /c cd " + rootPath + processingPath + " && html_tools.jar";
//				Commands.RunCommand(commandLine,true);
//				
//				commandLine = "cmd /c cd " + rootPath + processingPath + " && copy in\\*.msalign && copy in\\*.fasta && copy in\\arguments.xml && copy in\\*.OUTPUT_TABLE  && rd /s /q conf etc xml xsl in && del *.jar *.exe *.dll";
//				Commands.RunCommand(commandLine,true);
//				
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//		}
		return "subsucc";
	}

	@RequestMapping("deleteTask")
	public String deleteTask(String delName){
		String rootPath = request.getSession().getServletContext().getRealPath("/")+"result/";
		String os = System.getProperties().get("os.name").toString();
		if (os.startsWith("Win")) {
			try {
				Commands.RunCommand("cmd /c cd " + rootPath + " && rd /s /q "+delName);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return "redirect:/taskList.do";
	}
	
	@RequestMapping("runningPercent")
	public void processInfoRow(String name, HttpServletResponse response){
		String rootPath = request.getSession().getServletContext().getRealPath("/");
		
		File taskEndFile = new File(rootPath+"result/"+name+"/log.txt");
		double per=0.00;
		String status="Waiting";
		if(taskEndFile.exists()){
			per=0.01;
			FileReader reader = null;
			BufferedReader br = null;
			//read file 
		    try {
				reader = new FileReader(rootPath + "result/" + name + "/log.txt");
				br = new BufferedReader(reader);
				String str = null;
				String line1 = "";
				String line2 = "";
				String line3 = "";
				while ((str = br.readLine()) != null) {
					line3 = line2;
					line2 = line1;
					line1 = str;
					if(str.contains("Exectutive directory is")){
						per=1.0;
					}
					
					if(str.contains("Zero ptm search complete")){
						int a=Integer.parseInt(str.substring(str.length()-6, str.length()-5));
						int b=Integer.parseInt(str.substring(str.length()-1, str.length()));
						per=a*15/b;
					}
					if(str.contains("Fast filter")){
						per=15;
					}
					
					if(str.contains("Ptm alignment")){
						per=45;
					}
					if(str.contains("Ptm search complete")){
						int a=Integer.parseInt(str.substring(str.length()-6, str.length()-5));
						int b=Integer.parseInt(str.substring(str.length()-1, str.length()));
						per=45+a*35/b;
					}
					if(str.contains("Combine prsms")){
						per=80;
					}
					if(str.contains("Evalue computation complete")){
						String temp = str.replace("Evalue computation complete ", "");
						//System.out.println(temp.substring(0, 1));
						//System.out.println(temp.substring(5, 6));
						int a=Integer.parseInt(temp.substring(0, 1));
						int b=Integer.parseInt(temp.substring(5, 6));
						per=80+a*17/b;
					}
					if(str.contains("Top selector")){
						per=97;
					}
					if(str.contains("Table output")){
						per=98;
					}
					if(str.contains("XalanTransformer")){
						per=99;
					}
					if(str.contains("proteins.xml")){
						per=100;
					}
				}
				if(per==100){
					status="finished";
				}
				else if(RunningInfo.taskName.contains(name)){
					status="running";
				}
				else {
					status="error";
				}
				response.getWriter().write(per+"%,"+status);
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
		}
		else{
			try {
				response.getWriter().write("0.0%,waiting");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	@RequestMapping("zipdown")
	public void zipAndDownload(String path,HttpServletResponse response) throws Exception{
        String rootPath = request.getSession().getServletContext().getRealPath("/")+"result/"+path+"/";
        
        File file = new File(rootPath+path+".zip");
		if (!file.exists()) {
			FileUtils.zip(
					request.getSession().getServletContext().getRealPath("/")
							+ "temp/" + path + ".zip", rootPath);
			Commands.RunCommand("cmd /c move "
					+ request.getSession().getServletContext().getRealPath("/")
					+ "temp\\" + path + ".zip " + rootPath);
		}
        response.setHeader("Content-disposition","attachment; filename="+path+".zip");
        BufferedInputStream buffInput=new BufferedInputStream(new FileInputStream(rootPath+path+".zip"));
        BufferedOutputStream buffout=new BufferedOutputStream(response.getOutputStream());
        int length=-1;
        byte[] buff=new byte[1024];
		while ((length = buffInput.read(buff)) != -1) {
			buffout.write(buff, 0, length);
		}
		buffout.flush();
		buffInput.close();
		buffout.close();

	}
	
//	@RequestMapping("runninginfo")
//	public void processInfo(HttpServletResponse response){
//		String rootpath = request.getSession().getServletContext().getRealPath("/");
//		String workpath = (String)request.getSession().getAttribute("workingpath");
//		File file = new File(rootpath+workpath+"log.txt");
//		System.out.println(rootpath+workpath+"log.txt");
//		FileReader reader = null;
//		BufferedReader br = null;
//		response.setStatus(HttpServletResponse.SC_OK);
//		if (!file.exists()) {
//			try {
//				response.getWriter().write("not running!");
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//
//		}
//		else{
//			try {
//				reader = new FileReader(rootpath + workpath
//						+ "log.txt");
//				br = new BufferedReader(reader);
//				String str = null;
//				String line1="";
//				String line2="";
//				String line3="";
//				while ((str = br.readLine()) != null) {
//					line3=line2;
//					line2=line1;
//					line1=str;
//				}
//				
//				response.getWriter().write(line3+"<br/>"+line2+"<br/>"+line1);
//			} catch (IOException e) {
//				e.printStackTrace();
//			}finally{
//				if(reader!=null)
//				try {
//					reader.close();
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
//				if(br!=null)
//				try {
//					br.close();
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
//			}
//		}
//	}
	
	
	
//	@RequestMapping("run")
//	public ModelAndView index(String command){
//		System.out.println(command);
//		ModelAndView mav = new ModelAndView("result/succ");
//		//save to file
//		System.out.println(request.getSession().getServletContext().getRealPath("/"));
//		System.out.println();
//		try{
//		    FileWriter writer = new FileWriter(request.getSession().getServletContext().getRealPath("/")+"tool/conf/file",true);
//		    writer.write(command);
//		    writer.close();
//		}catch(IOException e){
//			e.printStackTrace();
//		}
//		//print result
//		String result = "OS is "+System.getProperties().get("os.name")
//				+ "<br>The commond is:"+command+"<br> the result is: <br>";
//		try {
//			String commandLine = "cmd /c " + command;
//			if(command.equalsIgnoreCase("run")){
//				commandLine = "cmd /c cd "+request.getSession().getServletContext().getRealPath("/")+"tool && folder_tools.jar && proteomics_cpp.exe && html_tools.jar";
//			}
//			
//			if(command.equalsIgnoreCase("ht")){
//				commandLine = "cmd /c cd "+request.getSession().getServletContext().getRealPath("/")+"tool && html_tools.jar";
//			}
//			
//			if(command.equalsIgnoreCase("clean")){
//				commandLine = "cmd /c cd "+request.getSession().getServletContext().getRealPath("/")+"tool/in && del /q *.* && cd .. && rd /s /q html && rd /s /q xml ";
//				System.out.println(commandLine);
//			}
//			
//			Process pro = Runtime.getRuntime().exec(commandLine);
//			BufferedReader br = new BufferedReader(new InputStreamReader(
//					pro.getInputStream()));
//			String msg = null;
//			while ((msg = br.readLine()) != null) {
//				result += msg + "<br>";
//				System.out.println(msg);
//			}
//		} catch (IOException exception) {
//		}
//
//		mav.addObject("result", result);
//		return mav;
//	}
//	
//	@RequestMapping("arguments")
//	public ModelAndView getArguments(@RequestParam("title" ) String title,
//			@RequestParam("fragMeth" ) String fragMeth,
//			@RequestParam("error" ) String error,
//			@RequestParam("sType" ) String sType,
//			@RequestParam("cType" ) String cType,
//			@RequestParam("cutValue" ) String cutValue,
//			@RequestParam("nuPTM" ) String nuPTM){
//		System.out.println(title);
//		System.out.println(fragMeth);
//		System.out.println(error);
//		System.out.println(sType);
//		System.out.println(cType);
//		System.out.println(cutValue);
//		System.out.println(nuPTM);
//		System.out.println(request.getParameter("fixPTM"));
//		System.out.println(request.getParameter("nptm1"));
//		System.out.println(request.getParameter("nptm2"));
//		System.out.println(request.getParameter("nptm3"));
//		ModelAndView mav = new ModelAndView("index");
//		return mav;
//	}
	
}

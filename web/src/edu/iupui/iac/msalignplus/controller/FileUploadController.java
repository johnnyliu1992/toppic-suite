package edu.iupui.iac.msalignplus.controller;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.ModelAndView;

import edu.iupui.iac.msalignplus.action.CompratorByLastModified;

@Controller
public class FileUploadController {

	@Autowired
	private HttpServletRequest request;

	@RequestMapping("filesUpload")
	public String filesUpload(@RequestParam("files") MultipartFile[] files) {
		if(files!=null&&files.length>0){
			for(int i = 0;i<files.length;i++){
				MultipartFile file = files[i];
				saveFile(file);
			}
		}
		return "redirect:/list.do";
	}

	private boolean saveFile(MultipartFile file) {
		if (!file.isEmpty()) {
			try {
				String filePath = request.getSession().getServletContext().getRealPath("/") + "tool/in/"
						+ file.getOriginalFilename();
				file.transferTo(new File(filePath));
				return true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return false;
	}

	@RequestMapping("fileUpload")
	public String fileUpload(@RequestParam("file") MultipartFile file) {
		if (!file.isEmpty()) {
			try {
				String filePath = request.getSession().getServletContext().getRealPath("/") + "tool/in/"
						+ file.getOriginalFilename();
				file.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return "redirect:/list.do";
	}

	@RequestMapping("list")
	public ModelAndView list() {
		String filePath = request.getSession().getServletContext().getRealPath("/") + "tool/in/";
		System.out.println(filePath);
		ModelAndView mav = new ModelAndView("list");
		File uploadDest = new File(filePath);
		String[] fileNames = uploadDest.list();
		for (int i = 0; i < fileNames.length; i++) {
			System.out.println(fileNames[i]);
		}
		return mav;
	}
	
	@RequestMapping("SpectraFile")
	public String SpectraFile(@RequestParam("file") MultipartFile file) {
		if (!file.isEmpty()) {
			try {
				String filePath = request.getSession().getServletContext().getRealPath("/") + "tool/in/spectra.msalign";
				file.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return "redirect:/uploadsucc.do";
	}
	
	@RequestMapping("DatabaseFile")
	public String DatabaseFile(@RequestParam("file") MultipartFile file) {
		if (!file.isEmpty()) {
			try {
				String filePath = request.getSession().getServletContext().getRealPath("/") + "tool/in/prot.fasta";
				file.transferTo(new File(filePath));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return "redirect:/uploadsucc.do";
	}
	
	@RequestMapping("config")
	public String config(@RequestParam("files") MultipartFile[] files) {
		if(files!=null&&files.length>0){
			for(int i = 0;i<files.length;i++){
				MultipartFile file = files[i];
				saveConfFile(file);
			}
		}
		return "redirect:/index.do";
	}
	
	private boolean saveConfFile(MultipartFile file) {
		if (!file.isEmpty()) {
			try {
				String filePath = request.getSession().getServletContext().getRealPath("/") + "tool/conf/"
						+ file.getOriginalFilename();
				file.transferTo(new File(filePath));
				return true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return false;
	}
	
	@RequestMapping("folderList")
	public ModelAndView folderList(String path) {
		String filePath = request.getSession().getServletContext().getRealPath("/") + "result/"+(path==null?"":path);
		System.out.println(filePath);
		ModelAndView mav = new ModelAndView("folderList");
		File uploadDest = new File(filePath);
		String[] fileNames = uploadDest.list();
		request.setAttribute("files", fileNames);
		return mav;
	}
	
	@RequestMapping("fileList")
	public ModelAndView fileList(String path) {
		String filePath = request.getSession().getServletContext().getRealPath("/") + "result/"+(path==null?"":path);
		System.out.println(filePath);
		ModelAndView mav = new ModelAndView("fileList");
		File uploadDest = new File(filePath);
		String[] fileNames = uploadDest.list();
		List<String> file_level_2 = new ArrayList<String>();
		for(int i=0;i<fileNames.length;i++){
			File temp = new File(filePath+"/"+fileNames[i]);
			if(temp.isDirectory()){
				file_level_2.add(fileNames[i]+"...");
				String[] level_2 = temp.list();
				for(int j=0;j<level_2.length;j++){
					File temp_level_2 = new File(filePath+"/"+fileNames[i]+"/"+level_2[j]);
					if(temp_level_2.isFile()){
						file_level_2.add(fileNames[i]+"/"+level_2[j]);
					}
				}
			}
			else{
				file_level_2.add(fileNames[i]);
			}
		}
		request.setAttribute("files", (String[])file_level_2.toArray(new String[0]));
		request.setAttribute("path", path);
		return mav;
	}
	
	@RequestMapping("taskList")
	public ModelAndView taskList() {
		String filePath = request.getSession().getServletContext().getRealPath("/") + "result/";
		System.out.println(filePath);
		ModelAndView mav = new ModelAndView("taskList");
		File uploadDest = new File(filePath);
		File[] files = uploadDest.listFiles();
		Arrays.sort(files, new CompratorByLastModified());  
		String[] fileNames = new String[files.length];
		for(int i=0;i< fileNames.length;i++){
			fileNames[i]=files[i].getName();
		}
//		String[] fileNames = uploadDest.list();
		request.setAttribute("files", fileNames);
		return mav;
	}
	
}

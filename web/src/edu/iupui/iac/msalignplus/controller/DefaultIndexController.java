package edu.iupui.iac.msalignplus.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

@Controller 
public class DefaultIndexController {
	
	@RequestMapping("index")
	public ModelAndView index(){
		return new ModelAndView("index");
	}
	
//	@RequestMapping("toFileUpload")
//	public String toFileUpload(){
//		return "fileUpload";
//	}
//	
//	@RequestMapping("toFilesUpload")
//	public String toFilesUpload(){
//		return "filesUpload";
//	}
//	
//	@RequestMapping("toSpectraFile")
//	public String toSpectraFile(){
//		return "SpectraFile";
//	}
//	
//	@RequestMapping("toDatabaseFile")
//	public String toDatabaseFile(){
//		return "DatabaseFile";
//	}
//	
//	@RequestMapping("toConfig")
//	public String toConfig(){
//		return "config";
//	}
//	
//	@RequestMapping("uploadsucc")
//	public String uploadSucc(){
//		return "uploadSucc";
//	}
}



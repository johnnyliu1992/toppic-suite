package edu.iupui.iac.msalignplus.listener;

import java.io.IOException;
import java.util.List;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import edu.iupui.iac.msalignplus.action.Commands;
import edu.iupui.iac.msalignplus.action.FileUtils;
import edu.iupui.iac.msalignplus.task.MsTask;

public class MsListener implements ServletContextListener {

	private java.util.Timer timer = null ;
	
	@Override
	public void contextDestroyed(ServletContextEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void contextInitialized(ServletContextEvent arg0) {
		// TODO Auto-generated method stub
		System.out.println("task init");
		try {
			Commands.RunCommand("cmd /c tasklist |findstr /i topid.exe >"+arg0.getServletContext().getRealPath("/")+"temp/cmd.log");
			List<String[]> programs =  FileUtils.getProcessInfo(arg0.getServletContext().getRealPath("/")+"temp/cmd.log");
			if(programs.size()>0){
				System.out.println("Program is already running ,stop it");
				Commands.RunCommand("cmd /c taskkill /f /im topid.exe ");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		timer = new java.util.Timer(true) ; 
		arg0.getServletContext().log("") ;  
		timer.schedule(new MsTask(arg0.getServletContext()), 0, 1000) ;  
		arg0.getServletContext().log("") ;  
		
	}

}

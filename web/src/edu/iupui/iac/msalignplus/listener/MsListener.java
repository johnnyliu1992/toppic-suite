package edu.iupui.iac.msalignplus.listener;

import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

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
		timer = new java.util.Timer(true) ; 
		arg0.getServletContext().log("") ;  
		timer.schedule(new MsTask(arg0.getServletContext()), 0, 15000) ;  
		arg0.getServletContext().log("") ;  
		
	}

}

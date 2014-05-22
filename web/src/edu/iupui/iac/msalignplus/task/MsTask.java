package edu.iupui.iac.msalignplus.task;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.TimerTask;

import javax.servlet.ServletContext;

import edu.iupui.iac.msalignplus.action.Commands;
import edu.iupui.iac.msalignplus.action.FileUtils;
import edu.iupui.iac.msalignplus.dao.Argument;
import edu.iupui.iac.msalignplus.dao.RunningInfo;

public class MsTask extends TimerTask {
	 private static boolean isRunning = false; 
	 private ServletContext context = null; 

	public MsTask(ServletContext context) {
		this.context = context;
	}

	@Override
	public void run() {
		if (!isRunning) {
			boolean exist_pro=false;
			try {
				Commands.RunCommand("cmd /c tasklist |findstr /i topid.exe >"+this.context.getRealPath("/")+"temp/cmd.log");
				List<String[]> programs =  FileUtils.getProcessInfo(this.context.getRealPath("/")+"temp/cmd.log");
				if(programs.size()>0){
					exist_pro=true;
					System.out.println("Program is running at "+System.currentTimeMillis());
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			if(!exist_pro){
				String[] a = FileUtils.getTaskInfo(context.getRealPath("/")+"tool/tasklist.xml");
				if(null!=a){
					RunningInfo.taskName = "TASK"+a[0];
					try {
					    String[] argArray = FileUtils.getInputFile(a[1]+"arguments.xml");
					    System.out.println("cmd /c "+context.getRealPath("/")+"tool/topid.exe -f "+ a[1]+"arguments.xml "+a[1]+argArray[0]+" "+a[1]+argArray[1]+" >"+ a[1]+"log.txt");
						Commands.RunCommand("cmd /c "+context.getRealPath("/")+"tool/topid.exe -f "+ a[1]+"arguments.xml "+a[1]+argArray[0]+" "+a[1]+argArray[1]+" >"+ a[1]+"log.txt");
						
						Argument arg = Argument.getArgument(a[1], "arguments.xml");
						arg.setEnd(System.currentTimeMillis());
						Argument.saveArgument(a[1], arg);
						
					} catch (Exception e) {
						e.printStackTrace();
					}
					RunningInfo.taskName="";
				}
				else {
//					System.out.println("no task");
				}
			}
		} else {
//			context.log("the task is running");
		}
	}
}


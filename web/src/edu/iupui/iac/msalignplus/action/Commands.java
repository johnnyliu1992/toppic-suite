package edu.iupui.iac.msalignplus.action;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Commands {
	
	public static void main(String[] args){
		try {
			RunCommand("cmd /c ");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void RunCommand(String commandLine,boolean getresult) throws IOException {
		Process pro = null;
		BufferedReader br =null;
		try {
			pro = Runtime.getRuntime().exec(commandLine);
			if (getresult) {
				br = new BufferedReader(new InputStreamReader(
						pro.getInputStream()));
				String msg = null;
				while ((msg = br.readLine()) != null) {
					System.out.println(msg);
				}
			}
		} catch (IOException e) {
			throw e;
		}
		finally{
			if(br != null){
				try {
					br.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if(pro!=null){
				pro.destroy();
			}
		}
	}
	
	public static void RunCommand(String commandLine) throws IOException {
		RunCommand(commandLine,true);
	}
}

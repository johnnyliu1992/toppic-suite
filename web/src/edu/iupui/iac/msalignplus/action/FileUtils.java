package edu.iupui.iac.msalignplus.action;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class FileUtils {
	
	public static void appendFile(String filename,String content){
		File file = new File(filename);
		FileWriter writer;
		try {
			if(!file.exists()){
				file.createNewFile();
			}
			Thread.sleep(3000l);
			writer = new FileWriter(filename, true);
	        writer.write(content);
	        writer.close();
		} catch (IOException | InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public static List<String[]> getProcessInfo(String filename){
		List<String[]> list = new ArrayList<String[]>();
		FileReader reader;
		try {
			reader = new FileReader(filename);
			BufferedReader br = new BufferedReader(reader);
			String str = null;
			while ((str = br.readLine()) != null) {
				str = str.replace("                     ", ",");
				str = str.replace(" ", ",");
				str = str.replace("                    ", ",");
				str = str.replace("  ", ",");
				String[] temp = str.split(",");
				list.add(temp);
			}
			br.close();
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return list;
	}

	public static int getTaskId(String filename){
		Document data = load(filename);
		Node id = data.getElementsByTagName("id").item(0);
		int value = Integer.parseInt(id.getTextContent());
		id.setTextContent(value+1+"");
		doc2XmlFile(data,filename);
		return value;
	}
	
	public static void addTaskList(String id,String path,String file){
		Document data = load(file);
		Element root = data.getDocumentElement();
		Element task = data.createElement("task");
		Element ide = data.createElement("id");
		ide.setTextContent(id);
		Element pathe = data.createElement("path");
		pathe.setTextContent(path);
		root.appendChild(task);
		task.appendChild(ide);
		task.appendChild(pathe);
		doc2XmlFile(data,file);
	}
	
	public static String[] getTaskInfo(String file){
		Document data = load(file);
		int number = data.getElementsByTagName("task").getLength();
		if(number>0){
			String[] a=new String[]{"",""};
			Node task = data.getElementsByTagName("task").item(0);
			NodeList taskinfo = task.getChildNodes();
			for(int i=0;i<taskinfo.getLength();i++){
				if(taskinfo.item(i).getNodeName().equals("id")){
					a[0]=taskinfo.item(i).getTextContent();
				}
				if(taskinfo.item(i).getNodeName().equals("path")){
					a[1]=taskinfo.item(i).getTextContent();
				}
			}
			data.getDocumentElement().removeChild(task);
			doc2XmlFile(data,file);
			return a;
		}
		return null;
	}
	
	public static String[] delTaskInfo(String id,String file){
		Document data = load(file);
		int number = data.getElementsByTagName("task").getLength();
		boolean isExist=false;
		if(number>0){
			String[] a=new String[]{"",""};
			Node task = null;
			for(int i=0;i<number;i++){
				task = data.getElementsByTagName("task").item(i);
				NodeList taskinfo = task.getChildNodes();
				for(int j=0;j<taskinfo.getLength();j++){
					System.out.println(taskinfo.item(j).getTextContent());
					if(taskinfo.item(j).getNodeName().equals("path")){
						a[1]=taskinfo.item(j).getTextContent();
					}
					if(taskinfo.item(j).getNodeName().equals("id")){
						a[0]=taskinfo.item(j).getTextContent();
					}
				}
				if(a[0].equalsIgnoreCase(id)){
					isExist = true;
					break;
				}
			}
			if(isExist){
				data.getDocumentElement().removeChild(task);
			}
			doc2XmlFile(data,file);
			return a;
		}
		return null;
	}
	
	public static String[] getInputFile(String file){
		String[] a = new String[]{"",""};
		Document data = load(file);
		int number = data.getElementsByTagName("database_file_name").getLength();
		if(number>0){
			a[0]=data.getElementsByTagName("database_file_name").item(0).getTextContent();
		}
		number = data.getElementsByTagName("spectrum_file_name").getLength();
		if(number>0){
			a[1]=data.getElementsByTagName("spectrum_file_name").item(0).getTextContent();
		}
		return a;
		
	}
	
	//read xml
	public static Document load(String filename) {
		Document document = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			document = builder.parse(new File(filename));
			document.normalize();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
		return document;
	}
	
	//save xml
	public static boolean doc2XmlFile(Document document, String filename) {
		boolean flag = true;
		try {
			TransformerFactory tFactory = TransformerFactory.newInstance();
			Transformer transformer = tFactory.newTransformer();
			/** encode */
			// transformer.setOutputProperty(OutputKeys.ENCODING, "GB2312");
			DOMSource source = new DOMSource(document);
			StreamResult result = new StreamResult(new File(filename));
			transformer.transform(source, result);
		} catch (Exception ex) {
			flag = false;
			ex.printStackTrace();
		}
		return flag;
	}
	
	public static void zip(String zipFileName, String inputFile)
			throws Exception {
		File f = new File(inputFile);
		ZipOutputStream out = new ZipOutputStream(new FileOutputStream(
				zipFileName));
		zip(out, f, null); 
		System.out.println("zip done");
		out.close();
	}

	//zip
	private static void zip(ZipOutputStream out, File f, String base)
			throws Exception {
		System.out.println("zipping " + f.getAbsolutePath());
		if (f.isDirectory()) {
			File[] fc = f.listFiles();
			if (base != null)
				out.putNextEntry(new ZipEntry(base + "/"));
			base = base == null ? "" : base + "/";
			for (int i = 0; i < fc.length; i++) {
				zip(out, fc[i], base + fc[i].getName());
			}
		} else {
			out.putNextEntry(new ZipEntry(base));
			FileInputStream in = new FileInputStream(f);
			int b;
			while ((b = in.read()) != -1)
				out.write(b);
			in.close();
		}
	}

	// unzip
	public static void unzip(String zipFileName, String outputDirectory)
			throws Exception {
		ZipInputStream in = new ZipInputStream(new FileInputStream(zipFileName));
		ZipEntry z;
		while ((z = in.getNextEntry()) != null) {
			String name = z.getName();
			if (z.isDirectory()) {
				name = name.substring(0, name.length() - 1);
				File f = new File(outputDirectory + File.separator + name);
				f.mkdir();
				System.out.println("MD " + outputDirectory + File.separator
						+ name);
			} else {
				System.out.println("unziping " + z.getName());
				File f = new File(outputDirectory + File.separator + name);
				f.createNewFile();
				FileOutputStream out = new FileOutputStream(f);
				int b;
				while ((b = in.read()) != -1)
					out.write(b);
				out.close();
			}
		}
		in.close();
	}
}

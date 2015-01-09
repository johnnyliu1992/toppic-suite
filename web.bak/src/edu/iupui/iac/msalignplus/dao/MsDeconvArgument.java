package edu.iupui.iac.msalignplus.dao;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerFactoryConfigurationError;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import edu.iupui.iac.msalignplus.action.FileUtils;

public class MsDeconvArgument {

	private String Title;
	private String Program;
	private String Centroided;
	private String Spectrum_File;
	private String Output;
	private int Max_charge;
	private double Tolerance;
	private double Ratio;
	private String Peaks = "";
	private String Missing = "";

	private long start = 0l;
	private long end = 0l;

	public MsDeconvArgument(String title, String protitle, String centroided,
			String specFile, String output, int maxcharge, double tolerance,
			double ratio, String peaks, String missing) {
		Title = title;
		Program = protitle;
		Centroided = centroided;
		Spectrum_File = specFile;
		Output = output;
		Max_charge = maxcharge;
		Tolerance = tolerance;
		Ratio = ratio;
		Peaks = peaks;
		Missing = missing;
		start = System.currentTimeMillis();
	}

	public static void saveArgument(String processingPath,
			MsDeconvArgument arguments)
			throws TransformerFactoryConfigurationError {
		FileOutputStream out = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			DocumentBuilder builder;

			builder = factory.newDocumentBuilder();

			Document document = builder.newDocument();
			document.appendChild(arguments.toElement(document));
			System.out.println(arguments.getCostString());
			TransformerFactory transFactory = TransformerFactory.newInstance();
			Transformer transFormer = transFactory.newTransformer();
			DOMSource domSource = new DOMSource(document);
			File file = new File(processingPath + "arguments.xml");
			if (!file.exists()) {
				file.createNewFile();
			}
			out = new FileOutputStream(file);
			StreamResult xmlResult = new StreamResult(out);
			transFormer.transform(domSource, xmlResult);
			// System.out.println(file.getAbsolutePath());
		} catch (ParserConfigurationException | IOException
				| TransformerException e) {
			e.printStackTrace();
		} finally {
			if (out != null) {
				try {
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

	public Element toElement(Document document) {
		Element root = document.createElement("arguments");

		Element title = document.createElement("title");
		title.setTextContent(Title);
		root.appendChild(title);

		Element program = document.createElement("program");
		program.setTextContent(Program);
		root.appendChild(program);

		Element spec = document.createElement("spectrum_file_name");
		spec.setTextContent(Spectrum_File);
		root.appendChild(spec);

		Element centroided = document.createElement("centroided");
		centroided.setTextContent(Centroided);
		root.appendChild(centroided);

		Element output = document.createElement("output");
		output.setTextContent(Output);
		root.appendChild(output);

		Element max_charge = document.createElement("max_charge");
		max_charge.setTextContent(Max_charge + "");
		root.appendChild(max_charge);

		Element tolerance = document.createElement("tolerance");
		tolerance.setTextContent(Tolerance + "");
		root.appendChild(tolerance);

		Element ratio = document.createElement("ratio");
		ratio.setTextContent(Ratio + "");
		root.appendChild(ratio);

		Element peaks = document.createElement("peaks");
		peaks.setTextContent(Peaks + "");
		root.appendChild(peaks);

		Element missing = document.createElement("missing");
		missing.setTextContent(Missing + "");
		root.appendChild(missing);

		Element starttime = document.createElement("start_time");
		starttime.setTextContent(start + "");
		root.appendChild(starttime);

		Element costtime = document.createElement("end_time");
		costtime.setTextContent(end + "");
		root.appendChild(costtime);

		return root;
	}

	public static MsDeconvArgument getArgument(String path, String fileName) {
		String full_path = path + fileName;
		Document xml_doc = FileUtils.load(full_path);
		String title = xml_doc.getElementsByTagName("title").item(0)
				.getTextContent();
		String program = xml_doc.getElementsByTagName("program").item(0)
				.getTextContent();
		String centroided = xml_doc.getElementsByTagName("centroided").item(0)
				.getTextContent();
		String spectrum = xml_doc.getElementsByTagName("spectrum_file_name")
				.item(0).getTextContent();
		String output = xml_doc.getElementsByTagName("output").item(0)
				.getTextContent();
		int maxcharge = Integer.parseInt(xml_doc
				.getElementsByTagName("max_charge").item(0).getTextContent());
		double tolerance = Double.parseDouble(xml_doc
				.getElementsByTagName("tolerance").item(0).getTextContent());
		double ratio = Double.parseDouble(xml_doc.getElementsByTagName("ratio")
				.item(0).getTextContent());
		String peaks = xml_doc.getElementsByTagName("peaks").item(0)
				.getTextContent();
		String missing = xml_doc.getElementsByTagName("missing").item(0)
				.getTextContent();

		long start = Long.parseLong(xml_doc.getElementsByTagName("start_time")
				.item(0).getTextContent());
		long cost = Long.parseLong(xml_doc.getElementsByTagName("end_time")
				.item(0).getTextContent());
		List<String> nptm = new ArrayList<String>();
		NodeList list = xml_doc.getElementsByTagName("protein_variable_ptm");
		for (int i = 0; i < list.getLength(); i++) {
			nptm.add(list.item(i).getTextContent());
		}

		MsDeconvArgument arg = new MsDeconvArgument(title, program, centroided,
				spectrum, output, maxcharge, tolerance, ratio, peaks, missing);
		arg.setStart(start);
		arg.setEnd(cost);
		return arg;
	}

	public static String getProgramName(String path, String fileName) {
		String full_path = path + fileName;
		Document xml_doc = FileUtils.load(full_path);

		String program = xml_doc.getElementsByTagName("program").item(0)
				.getTextContent();
		return program;
	}

	public String getProgram() {
		return Program;
	}

	public long getStart() {
		return start;
	}

	public String getTitle() {
		return Title;
	}

	public String getSpectrum_File() {
		return Spectrum_File;
	}

	public void setStart(long start) {
		this.start = start;
	}

	public long getRealEnd() {
		return end;
	}

	public long getEnd() {
		if (end == 0) {
			return System.currentTimeMillis();
		}
		return end;
	}

	public void setEnd(long cost) {
		this.end = cost;
	}

	public String getCostString() {
		String timestr = "";
		long time;
		if (end == 0) {
			time = (System.currentTimeMillis() - start) / 1000;
		} else {
			time = (end - start) / 1000;
		}
		long hour = time / 3600;
		time = time % 3600;
		long min = time / 60;
		time = time % 60;
		long sec = time;
		timestr += hour + ":";
		if (min < 10) {
			timestr += "0" + min + ":";
		} else {
			timestr += min + ":";
		}
		if (sec < 10) {
			timestr += "0" + sec;
		} else {
			timestr += sec;
		}
		return timestr;
	}

	public String getCostComment() {
		String timestr = "";
		long time = (getEnd() - getStart()) / 1000;

		long hour = time / 3600;
		time = time % 3600;
		long min = time / 60;
		time = time % 60;
		long sec = time;
		timestr += hour + " hours " + min + " minutes " + sec + " seconds ";

		return timestr;
	}

}

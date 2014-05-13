package edu.iupui.iac.msalignplus.dao;

import java.util.ArrayList;
import java.util.List;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import edu.iupui.iac.msalignplus.action.FileUtils;

public class Argument {

	private String Title;
	private String Fragmentation_Method;
	private String Spectrum_File;
	private String Database_File;
	private List<String> Protein_N_Terminal_Variable_PTM;
	private int Parent_and_Fragment_Mass_Error_Tolerance;
	private String Cysteine_Protecting_Group;
	private String Decoy;
	private String Cutoff_Type;
	private double Cutoff_Value;
	private int Number_of_Unexpected_PTMs;
	
	public Argument(String title, String fragMeth, String spectrum,
			String database, List<String> nptm, int error, String cys,
			String decoy, String ctype,double cvalue,int numup) {
		Title = title;
		Fragmentation_Method = fragMeth;
		Spectrum_File = spectrum;
		Database_File = database;
		Protein_N_Terminal_Variable_PTM = nptm;
		Parent_and_Fragment_Mass_Error_Tolerance = error;
		Cysteine_Protecting_Group = cys;
		Decoy = decoy;
		Cutoff_Type = ctype;
		Cutoff_Value = cvalue;
		Number_of_Unexpected_PTMs = numup;
	}
	
	public static Argument getArgument(String path,String fileName){
		String full_path = path+fileName;
		Document xml_doc = FileUtils.load(full_path);
		String title = xml_doc.getElementsByTagName("title").item(0).getTextContent();
		String fragMeth = xml_doc.getElementsByTagName("fragmentation_method").item(0).getTextContent();
		String spectrum = xml_doc.getElementsByTagName("spectrum_file_name").item(0).getTextContent();
		String database = xml_doc.getElementsByTagName("database_file_name").item(0).getTextContent();
		String cys = xml_doc.getElementsByTagName("cysteine_protecting_group").item(0).getTextContent();
		String decoy = xml_doc.getElementsByTagName("search_type").item(0).getTextContent();
		String ctype = xml_doc.getElementsByTagName("cutoff_type").item(0).getTextContent();
		int error = Integer.parseInt(xml_doc.getElementsByTagName("error_tolerance").item(0).getTextContent());
		int numup = Integer.parseInt(xml_doc.getElementsByTagName("shift_number").item(0).getTextContent());
		double cvalue =Double.parseDouble(xml_doc.getElementsByTagName("cutoff_value").item(0).getTextContent());
		List<String> nptm = new ArrayList<String>();
		NodeList list = xml_doc.getElementsByTagName("protein_variable_ptm");
		for(int i=0;i<list.getLength();i++){
			nptm.add(list.item(i).getTextContent());
		}
		Argument arg = new Argument(title, fragMeth, spectrum, database, nptm,
				error, cys, decoy, ctype, cvalue, numup);
		return arg;
	}
	
	public Element toElement(Document document){
		Element root = document.createElement("arguments");
		
		Element title = document.createElement("title");
		title.setTextContent(Title);
		root.appendChild(title);
		
		Element frag = document.createElement("fragmentation_method");
		frag.setTextContent(Fragmentation_Method);
		root.appendChild(frag);
		
		Element spec = document.createElement("spectrum_file_name");
		spec.setTextContent(Spectrum_File);
		root.appendChild(spec);
		
		Element data = document.createElement("database_file_name");
		data.setTextContent(Database_File);
		root.appendChild(data);
		
		Element vptmlist = document.createElement("protein_variable_ptm_list");
		for(int i=0;i<Protein_N_Terminal_Variable_PTM.size();i++){
			Element vptm = document.createElement("protein_variable_ptm");
			vptm.setTextContent(Protein_N_Terminal_Variable_PTM.get(i));
			vptmlist.appendChild(vptm);
		}
		root.appendChild(vptmlist);
		
		Element masserror = document.createElement("error_tolerance");
		masserror.setTextContent(Parent_and_Fragment_Mass_Error_Tolerance+"");
		root.appendChild(masserror);
		
		Element fptm = document.createElement("cysteine_protecting_group");
		fptm.setTextContent(Cysteine_Protecting_Group);
		root.appendChild(fptm);
		
		Element decoy = document.createElement("search_type");
		decoy.setTextContent(Decoy);
		root.appendChild(decoy);
		
		Element ctype = document.createElement("cutoff_type");
		ctype.setTextContent(Cutoff_Type);
		root.appendChild(ctype);
		
		Element cvalue = document.createElement("cutoff_value");
		cvalue.setTextContent(Cutoff_Value+"");
		root.appendChild(cvalue);
		
		Element numup = document.createElement("shift_number");
		numup.setTextContent(Number_of_Unexpected_PTMs+"");
		root.appendChild(numup);
		
		return root;
	}
	
	public String getTitle() {
		return Title;
	}
	public void setTitle(String title) {
		Title = title;
	}
	public String getFragmentation_Method() {
		return Fragmentation_Method;
	}
	public void setFragmentation_Method(String fragmentation_Method) {
		Fragmentation_Method = fragmentation_Method;
	}
	public String getSpectrum_File() {
		return Spectrum_File;
	}
	public void setSpectrum_File(String spectrum_File) {
		Spectrum_File = spectrum_File;
	}
	public String getDatabase_File() {
		return Database_File;
	}
	public void setDatabase_File(String database_File) {
		Database_File = database_File;
	}
	public List<String> getProtein_N_Terminal_Variable_PTM() {
		return Protein_N_Terminal_Variable_PTM;
	}
	public void setProtein_N_Terminal_Variable_PTM(
			List<String> protein_N_Terminal_Variable_PTM) {
		Protein_N_Terminal_Variable_PTM = protein_N_Terminal_Variable_PTM;
	}
	public int getParent_and_Fragment_Mass_Error_Tolerance() {
		return Parent_and_Fragment_Mass_Error_Tolerance;
	}
	public void setParent_and_Fragment_Mass_Error_Tolerance(
			int parent_and_Fragment_Mass_Error_Tolerance) {
		Parent_and_Fragment_Mass_Error_Tolerance = parent_and_Fragment_Mass_Error_Tolerance;
	}
	public String getCysteine_Protecting_Group() {
		return Cysteine_Protecting_Group;
	}
	public void setCysteine_Protecting_Group(String cysteine_Protecting_Group) {
		Cysteine_Protecting_Group = cysteine_Protecting_Group;
	}
	public String getDecoy() {
		return Decoy;
	}
	public void setDecoy(String decoy) {
		Decoy = decoy;
	}
	public String getCutoff_Type() {
		return Cutoff_Type;
	}
	public void setCutoff_Type(String cutoff_Type) {
		Cutoff_Type = cutoff_Type;
	}
	public double getCutoff_Value() {
		return Cutoff_Value;
	}
	public void setCutoff_Value(double cutoff_Value) {
		Cutoff_Value = cutoff_Value;
	}
	public int getNumber_of_Unexpected_PTMs() {
		return Number_of_Unexpected_PTMs;
	}
	public void setNumber_of_Unexpected_PTMs(int number_of_Unexpected_PTMs) {
		Number_of_Unexpected_PTMs = number_of_Unexpected_PTMs;
	}
	
	
}

package edu.iupui.iac.msalignplus.app;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import dom.ParserWrapper;


public class GenerateHtml {
	
	public static void main(String[] args) throws FileNotFoundException, TransformerException{
		List<String[]> files = GenerateHtml.readFileList("xmlfiles.xml");
		transXML2HTML(files);
	}

	public static Element readXML(String filename){
		Element root = null;
		try {
			ParserWrapper parser = (ParserWrapper)Class.forName("dom.wrappers.Xerces").newInstance();
			Document document = parser.parse(filename);
			root = document.getDocumentElement();
			System.out.println("read xml file and return root element with name "+root.getTagName());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return root;
	}
	
	public static NodeList getChildList(Element parent,String tagName){
		NodeList list = parent.getElementsByTagName(tagName);
		if (list == null) {
            return null;
        }
		return list;
	}
	
	public static String getChildString(Element parent,String tagName,int seq){
		return parent.getElementsByTagName(tagName).item(seq).getTextContent();
	}
	
	public static List<String[]> readFileList(String filename){
		List<String[]> fileList = new ArrayList<String[]>();
		Element root = readXML(filename);
		NodeList childs = getChildList(root,"file");
		for(int i=0;i<childs.getLength();i++){
			String[] temp = new String[]{getChildString((Element)childs.item(i),"xml",0),
					getChildString((Element)childs.item(i),"xsl",0),
					getChildString((Element)childs.item(i),"html",0)
			};
			fileList.add(temp);
		}
		return fileList;
	}
	
	public static void transXML2HTML(List<String[]> files) throws FileNotFoundException, TransformerException{
		TransformerFactory tFactory = TransformerFactory.newInstance();
		Transformer transformer = null;
		for(int i=0;i<files.size();i++){
			transformer = tFactory.newTransformer(new StreamSource(files.get(i)[1]));
			transformer.transform(new StreamSource(files.get(i)[0]), new StreamResult(new FileOutputStream(files.get(i)[2])));
		}
	}
}

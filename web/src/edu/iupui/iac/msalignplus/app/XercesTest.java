package edu.iupui.iac.msalignplus.app;

import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;

import com.sun.org.apache.xalan.internal.xsltc.runtime.Attributes;
import com.sun.org.apache.xerces.internal.parsers.SAXParser;

public class XercesTest extends DefaultHandler {

	public static void main(String args[]) throws Exception {

		(new XercesTest()).run("prsm0.xml");

	}

	public void run(String file) throws Exception {

		XMLReader parser = new SAXParser();

		parser.setContentHandler(this);

		parser.parse(file);

	} 

	public void startDocument() throws SAXException {

		System.out.println("starting parse XML file....");

	}

	public void startElement(String uri, String localName, String rawName,
			Attributes attlist)

	throws SAXException {

		System.out.println(localName);

	}

	public void endElement(String uri, String localName, String rawName)
			throws SAXException {

	}

	public void characters(char[] ch, int start, int length)
			throws SAXException {

		System.out.println(new String(ch, start, length));

	}

	public void endDocument() throws SAXException {

		System.out.println("end parse XML file!");

	}

}
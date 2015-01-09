package edu.iupui.iac.msalignplus.action;

import java.io.File;
import java.util.Comparator;

public class CompratorByLastModified  implements Comparator<File> {
	public int compare(File f1, File f2) {
		File subFile1= new File(f1.getParent()+"\\"+f1.getName()+"\\time.flag");
		File subFile2= new File(f2.getParent()+"\\"+f2.getName()+"\\time.flag");
		long diff = subFile1.lastModified() - subFile2.lastModified();
		if (diff > 0)
			return 1;
		else if (diff == 0)
			return 0;
		else
			return -1;
	}

	public boolean equals(Object obj) {
		return true;
	}
}
package edu.iupui.iac.msalignplus.task;

import edu.iupui.iac.msalignplus.action.Commands;
import edu.ucsd.proteomics.msdeconv.DeconvMng;
import edu.ucsd.proteomics.msdeconv.DeconvParameter;
import edu.ucsd.proteomics.msdeconv.DeconvProcess;
import jargs.gnu.CmdLineParser;

import java.io.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.jdom.*;
import org.jdom.input.SAXBuilder;

import com.jap.proteomics.base.util.BioIo;

public class RunMsDeconv implements Runnable {

	// RunMsDeconv(String xml, String log) {
	// xmlfile = xml;
	//
	// logfile = log;
	// }

	RunMsDeconv(String path, String cmd) {
		this.path = path.replaceAll("\\\\", "/");
		xmlfile = this.path + "arguments.xml";

		logfile = this.path + "log.txt";
		msconvert = cmd;
	}

	private String xmlfile, logfile, msconvert, path;

	// public

	@Override
	public void run() {
		Thread.currentThread().setName("msdeconv");
		SAXBuilder builder = new SAXBuilder();
//		String location = xmlfile.replaceAll("\\\\", "/");
		System.out.println(xmlfile);
		File xmlFile = new File(xmlfile);
		System.out.println(xmlfile);
		try {

			Document doc = builder.build(xmlFile);
			Element root = doc.getRootElement();
			Element element = root.getChild("program");
			String program = element.getValue();
			System.out.println(path);
			if (program.equals("msdeconv")) {
				System.out.println(program);
				String cmd = xmlfile.substring(0, xmlfile.length() - 13);
				element = root.getChild("spectrum_file_name");
				String spectrum_file_name = element.getValue();
				System.out.println(spectrum_file_name);
				System.setOut(new PrintStream(new FileOutputStream(logfile)));
				// if (spectrum_file_name.endsWith("raw")
				// || spectrum_file_name.endsWith("RAW")) {
				System.out
						.println("cmd /c "
								+ msconvert
								+ " "
								+ path
								+ spectrum_file_name
								+ " --mzXML --32 --zlib --filter \"peakPicking true 1-\" --filter \"zeroSamples removeExtra\" -o "
								+ path + " >" + logfile);

				Commands.RunCommand("cmd /c "
						+ msconvert
						+ " "
						+ path
						+ spectrum_file_name
						+ " --mzXML --32 --zlib --filter \"peakPicking true 1-\" --filter \"zeroSamples removeExtra\" -o "
						+ path);
				// }

				spectrum_file_name = spectrum_file_name.substring(0,
						spectrum_file_name.indexOf("."));
				spectrum_file_name += ".mzXML";
				cmd += spectrum_file_name;

				// element = root.getChild("centroided");
				// String centroided = element.getValue();
				// cmd += " -t " + centroided.toLowerCase();

				element = root.getChild("output");
				String output = element.getValue();
				cmd += " -o " + output.toLowerCase();

				element = root.getChild("max_charge");
				String max_charge = element.getValue();
				cmd += " -c " + max_charge;

				element = root.getChild("tolerance");
				String tolerance = element.getValue();
				cmd += " -e " + tolerance;

				element = root.getChild("ratio");
				String ratio = element.getValue();
				cmd += " -s " + ratio;

				element = root.getChild("peaks");
				String peaks = element.getValue();
				if (!peaks.equals("null")) {
					cmd += " -k ";
				}

				element = root.getChild("missing");
				String missing = element.getValue();
				if (!missing.equals("null")) {
					cmd += " -n";
				}

				System.out.println(cmd);
				try {

					DeconvParameter para = parseArgs(cmd.split(" "));
					DeconvProcess process = new DeconvProcess(para);
					process.process();
					process.getMsg();
				} catch (Exception e) {
					System.out.println(BioIo.getStackTraceAsString(e));
					System.exit(1);
				} catch (Error e) {
					System.out.println(BioIo.getStackTraceAsString(e));
					System.exit(1);
				}

			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static boolean checkThread() {
		boolean res = false;
		ThreadGroup group = Thread.currentThread().getThreadGroup();
		Thread[] threads = new Thread[group.activeCount() * 2];
		group.enumerate(threads);
		int count = group.enumerate(threads, true);
		for (int i = 0; i < count; i++) {
			if (threads[i].isAlive() && threads[i].getName().equals("msdeconv")) {
				res = true;
				// System.out.println("Check! ms-deconv is running!");
			}

		}
		return res;
	}

	@SuppressWarnings("deprecation")
	public static void stopme() {
		ThreadGroup group = Thread.currentThread().getThreadGroup();
		// Thread.currentThread()
		Thread[] threads = new Thread[group.activeCount() * 2];
		group.enumerate(threads);
		int count = group.enumerate(threads, true);
		for (int i = 0; i < count; i++) {
			if (threads[i].getName().equals("msdeconv")) {
				// DeconvProcess test = (DeconvProcess) threads[i];
				threads[i].stop();

			}
		}
	}

	public static DeconvParameter parseArgs(String args[]) throws Exception {
		CmdLineParser parser = new CmdLineParser();
		CmdLineParser.Option inputOption = parser.addStringOption('i', "input");
		CmdLineParser.Option outputOption = parser.addStringOption('o',
				"output");
		CmdLineParser.Option typeOption = parser.addStringOption('t', "type");
		CmdLineParser.Option precOption = parser.addBooleanOption('p',
				"precursor");
		CmdLineParser.Option levelOption = parser.addBooleanOption('l',
				"level-one");
		CmdLineParser.Option chargeOption = parser.addIntegerOption('c',
				"max-charge");
		CmdLineParser.Option massOption = parser.addDoubleOption('m',
				"max-mass");
		CmdLineParser.Option errorOption = parser.addDoubleOption('e',
				"mz-error");
		CmdLineParser.Option ratioOption = parser.addDoubleOption('s',
				"sn-ratio");
		CmdLineParser.Option keepOption = parser.addBooleanOption('k', "keep");
		CmdLineParser.Option multipleOption = parser.addBooleanOption('u',
				"multiple-mass");
		CmdLineParser.Option missingOption = parser.addBooleanOption('n',
				"missing-level-one");

		try {
			parser.parse(args);
		} catch (Exception e) {
			System.exit(1);
		}
		/* file name */
		String remain[] = parser.getRemainingArgs();
		if (remain == null || remain.length == 0) {
			System.exit(1);
		}
		DeconvParameter para = new DeconvParameter();
		para.setDataFileName(remain[0]);
		/* input type */
		String input = (String) parser.getOptionValue(inputOption, "mzXML");
		int result = para.setInputType(input);
		if (result > 0) {
			System.exit(1);
		}

		/* output type */
		String format = (String) parser.getOptionValue(outputOption, "mgf");
		result = para.setOutputType(format);

		if (result > 0) {
			System.exit(1);
		}

		/* centroid */
		para.doCentroid = false;
		String type = (String) parser.getOptionValue(typeOption, "centroided");
		if (type.equals("profile")) {
			para.doCentroid = true;
		}

		/* original precursor mass */
		para.doRefinePrecMass = true;
		Boolean useOriPrec = (Boolean) parser.getOptionValue(precOption,
				Boolean.FALSE);
		if (useOriPrec) {
			para.doRefinePrecMass = false;
		}
		/* level */
		Boolean reportLevelOne = (Boolean) parser.getOptionValue(levelOption,
				Boolean.FALSE);
		if (reportLevelOne) {
			para.msLevel = 1;
		} else {
			para.msLevel = 2;
		}

		Boolean missingLevelOne = (Boolean) parser.getOptionValue(
				missingOption, Boolean.FALSE);
		para.missingLevelOne = missingLevelOne;

		/* multiple mass */
		para.outputMultipleMass = false;
		Boolean multiple = (Boolean) parser.getOptionValue(multipleOption,
				Boolean.FALSE);
		if (multiple) {
			para.outputMultipleMass = true;
		}

		para.maxCharge = (Integer) parser.getOptionValue(chargeOption,
				DeconvMng.defaultMaxCharge);
		Double mass = (Double) parser.getOptionValue(massOption,
				DeconvMng.defaultMaxMass);
		para.maxMass = mass.floatValue();
		Double tolerance = (Double) parser.getOptionValue(errorOption, 0.02);
		para.tolerance = tolerance;
		Double ratio = (Double) (parser.getOptionValue(ratioOption, 1.0));
		para.snRatio = ratio.floatValue();
		para.keepUnusedPeaks = (Boolean) parser.getOptionValue(keepOption,
				false);
		return para;
	}

}

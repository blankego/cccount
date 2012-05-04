package org.need4steed.cccount;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.nio.charset.Charset;

import org.apache.commons.cli.*;
public class CLI implements IProgram{
	Charset _charset ;
	ITextProcessor _counter;
	private CLI(){_counter = new Counter();}
	
	//Singleton instance
	private static CLI _inst = null; 
	public static CLI GetInstance()
	{
		if(_inst==null)
			_inst = new CLI();
		return _inst;
	}
	//-----------
	
	HelpFormatter help = new HelpFormatter();
	CommandLine _cl;
	Options _ops = new Options();;
	
	void SetUp(String[] args)
	{
		
		_ops.addOption("h","help",false, "show this message and exit");
		_ops.addOption("l","listall",false,
				"list all distinguishable Chinese characters appeared,"+
				"with corresponding counting, sorted by the counts descendantly.");
		_ops.addOption("S","nosummary",false,"do not show the summary(default output).");
		_ops.addOption("e","encoding",true,
				"secify the encoding (the default is utf-8) of the input file(s)/text.");
		
		CommandLineParser parser = new GnuParser(); 
		try
		{
			_cl = parser.parse(_ops, args);
			_charset = Charset.forName(
					_cl.hasOption('e')?
					_cl.getOptionValue('e'):
					"UTF8");
			if(_cl.hasOption("h"))
				ShowHelp(true);
		}
		catch(ParseException exp)
		{
			System.out.println( "Unexpected exception:" + exp.getMessage());
			ShowHelp(false);
		}
		
	}
	private void ShowHelp(boolean normal) {
		// TODO Auto-generated method stub
		help.printHelp("cjkcount [OPTION]+ [FILE]+\n\n" + 
                "print the counts of total and distinguishable "+
                "Chinese characters\n appeared in given file(s)\n" + 
                "If no file is provided, it reads text from stdin.\n\n",
                _ops);
		
		System.exit(normal?0:1);
	}
	
	void DoJob() throws IOException
	{
		Reader rdr;
		String[] files = _cl.getArgs();
		
		if(files.length==0)
		{
			rdr = new InputStreamReader(System.in,_charset);
			_counter.Process(rdr);
			
		}
		else
		{
			for(String f:files)
				_counter.Process(
						new InputStreamReader(new  FileInputStream(f),_charset));
		}
	}
	
	void ShowResult() throws IOException
	{
		if(_cl.hasOption('l'))
			_counter.Output(new OutputStreamWriter(System.out,_charset));
		if(!_cl.hasOption('S'))
		{
			OutputStreamWriter w = new OutputStreamWriter(System.out,_charset);
			w.write(_counter.GetStatistics());
			w.flush();
		}
	}
	@Override
	public void Run(String[] args) {
		// TODO Auto-generated method stub
		SetUp(args);
		try {
			DoJob();
			ShowResult();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			ShowHelp(false);
			System.exit(1);
		}
		
	}
	
	
}

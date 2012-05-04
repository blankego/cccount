using System;
using System.Collections.Generic;
using NDesk.Options;
using System.Text;
using System.IO;
namespace CCCount
{
	class CLI: IProgram
	{
		
		
		List<string> _files;
		ITextProcessor _counter;
		
		#region options		
		bool show_help = false;
		Encoding input_encoding = Encoding.UTF8;
		Encoding output_encoding = Encoding.Default;
		string extension = "";
		bool summary = true;
		bool listall = false;
		#endregion
		
		
		#region singletonInst
		CLI(){}
		class SingletonCreator
		{
			static SingletonCreator(){}
			internal static readonly CLI  inst = new CLI();
		}
		static public CLI Instance {get{return SingletonCreator.inst;} }
		#endregion
		
				
		void SetUp(string[] args)
		{
			try
			{
				var p = new OptionSet { 
					{ "h|help", "show this message and exit", v => { show_help = v != null; } },
					{ "i|input-encoding=", "specify the encoding (the default is utf-8) of the input files/text.", v =>
						{
							input_encoding = Encoding.GetEncoding (v);							
						} },
					{"o|output-encoding=","specify the encoding of the output text(otherwise use the default one.",
						v=>
						{
							output_encoding = Encoding.GetEncoding(v);							
						}
					},
					{ "x|extension=", "if the given path(s) is a directory, the extenstion should be provided, " +
						"whereby, only matched files will be counted.", v => { extension = v; } },
					{ "s|summary+", "show result summary (default action), append [-] instead [+] to the flag will disable it, ", 
						v => { summary = v != null; } },
					{ "l|listall", "list all distinguishable Chinese characters appeared, with corresponding counting, " +
						"sorted by the counts descendantly.", v => { listall = v != null; } } 
				};
			
			
				_files = p.Parse (args);
				
				if (show_help)
				{
					ShowHelp (p);
					return;
				}
			}
			catch(ArgumentException ex)
			{
				Console.WriteLine(ex.Message);
				Environment.Exit(1);
			}
			catch (OptionException ex)
			{
				Console.WriteLine ("cjkcount: {0} \nTry cjkcount --help for more information.", ex.Message);
				Environment.Exit(1);
			}
			
		}
		
		void DoJob()
		{
			
			_counter = new Counter ();
			
			
			if (_files.Count == 0)//get input from stdin
			{
				using (var sr = new StreamReader (Console.OpenStandardInput (), input_encoding))
				{
					_counter.Process (sr);
				}
			}			
			else
			{
				WalkFileEntries (_files, f =>
				{
					if (string.IsNullOrEmpty (extension) || f.EndsWith ("." + extension))
					{
						using(var sr = new StreamReader(f,input_encoding))
						{
							_counter.Process (sr);							
						}
					}
				});
			}
		}
		
		void ShowResult()
		{
			Encoding enc = Console.OutputEncoding;
			Console.OutputEncoding = output_encoding;
			//TODO: The output encoding doesn't seem to work
			//result output------------
			if (listall){
				_counter.Output(Console.Out);				
			}
			//summary
			if (summary)
			{
				 Console.Out.WriteLine(_counter.Statistics);
			}
			Console.OutputEncoding = enc;
		}
		
		void ShowHelp (OptionSet p)
		{
			Console.WriteLine ("Usage: cjkcount [OPTION]+ [FILE|DIR]+\n\n" + 
			                   "print the counts of total and distinguishable "+
			                   "Chinese characters\n appeared in given file(s)\n" + 
			                   "If no file is provided, it reads text from stdin.\n\n" + 
			                   "Options:\n");
			p.WriteOptionDescriptions (Console.Out);
			Environment.Exit(0);
		}
		
		static void WalkFileEntries (IEnumerable<string> fes, Action<string> cb)
		{
			try
			{
				foreach (var f in fes)
				{
					var fa = File.GetAttributes (f);
					if ((fa & FileAttributes.Directory) > 0)
						WalkFileEntries (Directory.GetFileSystemEntries (f), cb);
					if ((fa & FileAttributes.Normal) > 0)
						cb (f);
				}
			}
			catch (FileNotFoundException e)
			{
				Console.WriteLine ("cjkcount: {0} \nTry cjkcount --help for more information.", e.Message);
				Environment.Exit (1);
			}
		}
		
		#region IProgram implementation
		public void Run (string[] args)
		{
			
			SetUp(args);
			DoJob();
			ShowResult();
		}
		#endregion		
		
	}
}


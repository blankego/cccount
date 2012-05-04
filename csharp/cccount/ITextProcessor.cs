using System;
using System.IO;
using System.Collections.Generic;
namespace CCCount
{
	public interface ITextProcessor
	{
		void Process(TextReader tr);
		void Process(string s );
		
		/// <summary>
		/// It's designed as an enumerable list of lines so that it could be more convenient to use.
		/// </summary>
		void Output (TextWriter tw);
		string Statistics {get;}
	}
}


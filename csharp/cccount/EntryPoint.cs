using System;
namespace CCCount
{
	public class EntryPoint
	{
		public static void Main (string[] args)
		{
			IProgram cli = CLI.Instance;
			cli.Run(args);
			
		}
	}
}


package org.need4steed.cccount;
import java.io.*;


public interface ITextProcessor extends Iterable<String> {
	void Process(String s);
	void Process(Reader rdr) throws IOException;
	String GetStatistics();
	void Output(Writer wtr)throws IOException;
	
}

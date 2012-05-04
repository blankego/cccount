package org.need4steed.cccount;

import java.io.*;
import java.util.*;
import java.util.Map.Entry;

public class Counter implements ITextProcessor , Iterable<String>{

	static final char L = '\u4dff', R = '\u9fa6';
	static final char LA = '\u33ff', RA = '\u4db6';

	//cjk ext B guards
	static final char SURROGATEHL = '\ud7ff', SURROGATEHR = '\udc00';
	static final char SURROGATELL = '\udbff', SURROGATELR = '\ue000';
	static final int LB = 0x1ffff, RB = 0x2a6d7;
	
	public Counter()
	{
		for(int i = 0; i!=3;++i)
		{
			_cjks.add(new HashMap<Integer, Integer>());
		}
	}
	// states
	int _nchars = 0;
	int _ncjks = 0;
	int _nfiles = 0;
	public int GetNChars(){return _nchars;}
	public int GetNCjks(){return _ncjks;}
	public int GetNFiles(){return _nfiles;}	
	public int GetNDistinguishable()
	{
		return _cjks.get(0).size() +
				_cjks.get(1).size()+
				_cjks.get(2).size();
	}
	final ArrayList<HashMap<Integer, Integer >> _cjks =new ArrayList<HashMap<Integer,Integer>>(3);
	boolean _sorted = false;
	final ArrayList<Map.Entry<Integer, Integer>> _sortedList = new ArrayList<Map.Entry<Integer, Integer>>();
	
	
	@Override
	public void Process(String s) {
		_sorted = false;
		for(int i=0;i!=s.length();++i)
		{
			HashMap<Integer,Integer> dic = null;
			int c = s.codePointAt(i);
			if(c > 0xffff)
			{
				i++;
				if(c >LB && c <RB)
				{
					dic = _cjks.get(2);
				}
			}
			else if(c > L && c < R )
				dic = _cjks.get(0);
			else if( c > LA && c < RA)
				dic = _cjks.get(1);
			if(dic != null)
		    {
		    	if(dic.containsKey(c))
		    		dic.put(c, dic.get(c)+1);
		    	else
		    		dic.put(c, 1);	
		    	_ncjks++;
		    }
			_nchars++;
		}
	}

	@Override
	public void Process(Reader rdr) throws IOException {
		LineNumberReader lr = new LineNumberReader(rdr);
		String l;
		while((l=lr.readLine())!=null)
		{
			Process(l);
		}
		_nfiles++;
	}

	@Override
	public String GetStatistics() {
		return toString();
	}
	@Override
	public String toString()
	{
		return String.format(	"#[Summary]:\n" + 
								"%1$8d -- Total number of characters have been read.\n"+
	                      		"%2$8d -- Total number of Chinese characters.\n" +
		                      	"%3$8d -- Number of distinguishabe Chinese characters.\n"+
	                      		"%4$8d -- Files processed.\n", 
		                      	_nchars,		                      	
		                      	_ncjks, 
		                      	GetNDistinguishable(),
		                      	_nfiles);
			                      	
	}

	@Override
	public void Output(Writer wtr) throws IOException {
		for(String ln : this)
		{
			wtr.write(ln);
			wtr.write('\n');
		}	
		wtr.flush();
	}
	
	
	private void sortResult()
	{
		
		_sortedList.clear();
		for(int i=0;i!=3;++i)
			_sortedList.addAll( _cjks.get(i).entrySet());
		Collections.sort(_sortedList,new Comparator<Entry<Integer,Integer>>() {
			
			public int compare(Entry<Integer,Integer>a,Entry<Integer,Integer>b)
			{
				int com = b.getValue().compareTo(a.getValue());
				return com == 0?
						a.getKey().compareTo(b.getKey()):
						com;
			}
		});
		_sorted = true;
		
	}
	
	@Override
	public Iterator<String> iterator() {
		if(!_sorted )sortResult();	
		
		return new Iterator<String>(){
			Iterator<Entry<Integer,Integer>> it;
			{it=  _sortedList.iterator();}
						
			public void remove() {
				throw new UnsupportedOperationException();				
			}		
			
			public String next() {
				Entry<Integer,Integer> ent = it.next();
				return  new String( new int[]{ent.getKey()},0,1)+
						"\t" +
						ent.getValue().toString();
			}			
			
			public boolean hasNext() {
				
				return it.hasNext();
			}
		};
	}
}

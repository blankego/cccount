using System;
using System.Collections.Generic;
using System.Collections;
using System.Globalization;
using System.IO;
using System.Linq;

namespace CCCount
{
	
	public class Counter:ITextProcessor, IEnumerable<string>,IEnumerable
	{
		#region consts
		const char L = '\u4dff', R = '\u9fa6';
		const char LA = '\u33ff', RA = '\u4db6';
		//cjk ext B guards
		const uint LB = 0x1ffff, RB = 0x2a6d7;
		const char SURROGATEHL = '\ud7ff', SURROGATEHR = '\udc00';
		const char SURROGATELL = '\udbff', SURROGATELR = '\ue000';		
		#endregion
		
		#region states
		int _nchars = 0;
		int _ncjks = 0;
		int _nfiles = 0;
		Dictionary<uint, int>[] _cjks = { 
			new Dictionary<uint, int> (),
			new Dictionary<uint, int> (), 
			new Dictionary<uint, int> () 
		};
		bool _sorted = false;
		readonly List<KeyValuePair<uint, int>>[] _sortedList = new List<KeyValuePair<uint, int>>[3];
		#endregion
		
		/// <summary>
		/// Number of characters been read
		/// </summary>
		public int NChars
		{
			get { return _nchars; }
		}
		/// <summary>
		/// Total number of Chinese characters have been read
		/// </summary>
		public int NCjks
		{
			get { return _ncjks;}
		}
		
		/// <summary>
		/// Number of distinguishable Chinese characters
		/// </summary>
		public int NDistinguishable
		{
			get { return _cjks[0].Count + _cjks[1].Count + _cjks[2].Count; }
		}
		/// <summary>
		/// Files processed
		/// </summary>
		public int NFiles
		{
			get{ return _nfiles;}
		}
		
		public void Process(string s)
		{
			IEnumerator<char> cEnum = s.GetEnumerator();
			while(cEnum.MoveNext())
			{
				uint c = cEnum.Current;
				Dictionary<uint, int> dic = null;				
				//single char glyph
				if (c > L && c < R)
				{
					dic = _cjks[0];
				}
				else if( c > LA && c < RA)
				{
					dic = _cjks[1];
				}
				else if (c > SURROGATEHL && c < SURROGATEHR && cEnum.MoveNext())
				{
					//surrogate pair test
					//The loops are used instead of TextElementEnumerator, the decision is made out of
					//considerations of efficiency
					var c2 = cEnum.Current;
					if (c2 > SURROGATELL && c2 < SURROGATELR)
					{
						c = Utils.SurrogatesToUint(Convert.ToChar(c),c2);
						if (c > LB && c < RB)
						{
							dic = _cjks[2];							
						}
					}
				}
				if(dic != null)
				{
					dic[c] = dic.ContainsKey (c) ? dic[c] + 1 : 1;
					_ncjks++;
				}
				
				//incomplete surrogate parts will be ignored as junks
				_nchars++;
			}
			_sorted = false;
		}
		
		public void Process (TextReader tr)
		{
			string s = null;
			while((s = tr.ReadLine())!= null)
			{
				Process(s);
			}		
			_nfiles++;
		}
		
		
		
		public List<KeyValuePair<uint, int>>[] SortedLists
		{
			get
			{
				if (!_sorted)
				{
					for (int i = 0; i < 3; i++)
					{
						_sortedList[i] = new List<KeyValuePair<uint, int>> (_cjks[i]);
						_sortedList[i].Sort ((x, y) =>
						{
							int o = y.Value.CompareTo (x.Value);
							return o == 0 ? x.Key.CompareTo (y.Key) : o;
						});
					}
					_sorted = true;
				}
				return _sortedList;
			}
		}
		
		public override string ToString ()
		{
			return string.Format (	"#[Summary]:\n" + 
			                      	"{0,8} -- Total number of characters have been read.\n"+
			                      	"{1,8} -- Total number of Chinese characters.\n" +
			                      	"{2,8} -- Number of distinguishabe Chinese characters.\n"+
			                      	"{3,8} -- Files processed.\n", 
			                      	_nchars,
			                      	_ncjks,
			                      	NDistinguishable,
			                      	_nfiles
			                      	
			                      );
		}
	

		#region ITextProcessor implementation
		

		public void Output(TextWriter tw)
		{
			foreach(string l in this)
			{
				tw.WriteLine(l);
			}
		
		}

		public string Statistics
		{
			get
			{
				return this.ToString();
			}
		}
		#endregion
		#region IEnumerable[System.String] implementation
		public IEnumerator<string> GetEnumerator ()
		{
			return (
			        from list in SortedLists
					from item in list
						select (item.Key < 0x10000 ?
						         ((char)item.Key).ToString() :
						         Utils.UintToSurrogates(item.Key)) + "\t" + item.Value
			        ).GetEnumerator();
		}
		IEnumerator IEnumerable.GetEnumerator(){ return GetEnumerator();}
		#endregion
	}
}

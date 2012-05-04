using System;
using NUnit.Framework;
using CCCount;
using System.Linq;
using System.Collections.Generic;
using System.IO;

namespace cccountTest
{
	[TestFixture]
	public class CCCountTest
	{
		
		[Test]
		public void CounterTest ()
		{
			Counter c = new Counter ();
			c.Process (new StringReader( "ab弓都\u9fa5弓t\u9fa5\U0002a6d6M"));
			Assert.AreEqual(10,c.NChars);
			Assert.AreEqual (6, c.NCjks);
			Assert.AreEqual (4, c.NDistinguishable);
			c.Process(new StringReader("火"));
			Assert.AreEqual(2,c.NFiles);
		}
		
		[Test]
		public void CounterListTest ()
		{
			ITextProcessor c = new Counter ();
			c.Process(new StringReader("ab弓都\u9fa5弓t\u9fa5\U0002a6d6M"));
			IEnumerable<string> en =(IEnumerable<string>) c;
			Assert.AreEqual('都',en.ElementAt(2)[0]);
			Assert.AreEqual("\U0002a6d6\t1",en.Last());
			Assert.AreEqual(4,en.Count());
			
		}
		
		[Test]
		public void UintToSurrogatesTest ()
		{
			uint i = 0x0002a6d6u;
			
			string s = Utils.UintToSurrogates (i);
			Assert.AreEqual (0xD869u, s[0]);
			Assert.AreEqual (0xDED6u, s[1]);
			
		}
		
	
		
		[Test]
		public void SurrogatesToUintTest ()
		{
			char[] cs = "\U0002a6d6".ToCharArray ();
			
			Assert.AreEqual (0x0002a6d6u, Utils.SurrogatesToUint(cs[0],  cs[1]));
		}
	}
}


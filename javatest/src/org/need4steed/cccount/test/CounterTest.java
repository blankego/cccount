package org.need4steed.cccount.test;

import static org.junit.Assert.*;

import org.junit.Test;
import org.need4steed.cccount.Counter;
import java.io.*;
public class CounterTest {

	@Test
	public void test() {
		String s ="ab弓都𪛖\u9fa5弓t\u9fa5M";
		Counter ct = new Counter();
		ct.Process(s);
		assertEquals(10	, ct.GetNChars());
		assertEquals(6, ct.GetNCjks());
		assertEquals(4, ct.GetNDistinguishable());
		String[] res= {"弓\t2","\u9fa5\t2","都\t1","𪛖\t1"};
		int i = 0;
		for(String el:ct)
		{
			System.out.println(el);
			assertEquals(res[i], el);
			i++;
		}
		
		assertEquals(4,i);
		StringWriter w = new StringWriter();
		try {
			ct.Output(w);
			assertEquals(res[3], w.toString().split("\n")[3]);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}

}

#!/usr/bin/env python
#-*- coding: utf-8 -*-
import unittest
from StringIO import StringIO
from cccount import Counter

class CCCountTest(unittest.TestCase):
    def setUp(self):
        self.s = StringIO()
        self.s.write('ab弓都龥弓t龥𢐜M*')
        
    def test_counter(self):
        self.s.seek(0)
        c = Counter().addFile(self.s)
        self.assertEqual(c.nchar,11)
        self.assertEqual(c.ncjk,6)
        self.assertEqual(len(c.cjks),4)
    
    def test_encoding(self):
        sio = StringIO()
        sio.write(self.s.getvalue())
        sio.seek(0)
        
        

if __name__=='__main__':
    unittest.main()
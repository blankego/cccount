#!/usr/bin/env python2
import sys
import os
import codecs
from optparse import OptionParser

class UI:
    def __init__ (self):
        #option settings
        op=OptionParser(usage='usage: %prog [OPTION]... [FILE]...', version="%prog 0.0.1")
        op.description="Print counts of Chinese characters in given FILE(s). If no"+\
                    " FILE is given,it will read from standard input stream"
        op.add_option('-l','--list',dest='list', 
                help = 'List all appeared Chinese characters with their counts',
                action = 'store_true')
        op.add_option('-s','--sum',dest='sum',
                help = 'Print the sum of all used Chinese characters',
                action = 'store_true')
        op.add_option('-e','--encoding',dest='encoding',default='utf8',
                help = 'Specify input encoding (default UTF-8)',
                action = 'store',type='string')
        self.ops,self.args = op.parse_args()
        
        #the default job is to print the sum 
        if self.ops.list is None and self.ops.sum is None:
            self.ops.sum = True
        
        #check up encoding
        try:
            codecs.lookup(self.ops.encoding)
        except LookupError as e:
            print( e)
            exit(1)
    
    def process(self):
        self.counter = Counter(encoding = self.ops.encoding)
        if len(self.args)==0:
            #read text from stdin
            self.counter.addFile(sys.stdin)
        else:
            #read text from file(s), whose name(s) are provided by args
            for f in self.args:
                if os.path.isfile(f):
                    with open(f) as fi:
                        self.counter.addFile(fi)
            
        self.__showResult()
        
    def __showResult(self):
        if self.ops.list:
           self.counter.lsChars()        
        if self.ops.sum:
           self.counter.showSum()
        

            
    
            
class Counter(object):
    def __init__(self,encoding='utf8'):
        self.cjks={}
        self.subject=None
        self.encoding=encoding
        self.done=False
        self.nfile=0 #number of files already processed
        self.ncjk=0 #number of Chinese characers appeared
        self.nchar=0 #number of chars already read

    
    def count(self):
        """do a counting on subject"""
        if not self.done:
            self.nfile += 1
            for l in self.subject:
                try:
                    line = unicode(l,self.encoding)
                except UnicodeDecodeError, e:
                    if hasattr(self.subject,"name"):
                        print '[%s]:' % self.subject.name
                    print  e
                    exit(1)
                for c in line:
                    self.nchar += 1
                    if (u'\u4e00' <= c and c <= u'\u9fa5') or\
                        (u'\u3400'<= c and c <= u'\u4db5') or\
                        (u'\U00020000' <= c and c <= u'\U0002a6d6'):
                        #Chars out of BMP use u'Uxxxxxxxx' notation
                        self.ncjk += 1
                        self.cjks[c]=self.cjks.get(c,0) + 1
        self.done = True            
        
    def lsChars(self):
        for c, n in sorted(self.cjks.iteritems(), key = lambda it:it[1],reverse = True):
            print c.encode('utf8'),n
   
    def addFile(self,f):
        """add one more file as the subject to be counted"""        
        self.done = False
        self.subject = f
        self.count()
        return self
         
        
    
    def showSum(self):
        """sum"""
        print "FILES: %d, CHINESE_CHARACTERS: %d, UNIQUE_APPEARANCES: %d " %\
                (self.nfile,self.ncjk,len(self.cjks))

if __name__=="__main__":
    UI().process()

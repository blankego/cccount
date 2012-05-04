/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "cccounter.h"
#include <map>
#include <iostream>
#include <string>
#include "utf8iterator.h"
#include <algorithm>
#include "utils.h"
#include <cstdio>
using namespace std;

namespace CCCount
{
  //Chinese character boundary
  enum {
    	L = 0x4dff,
	R = 0x9fa6,
	LA = 0x33ff,
	RA = 0x4db6,
	LB = 0x1ffff,
	RB = 0x2a6d7,
  };
  
    void Counter::Process(const string& s)
    {
      Utf8Iterator it(s);
      while(it.MoveNext())
      {
        _nChars++;
        unsigned c = it.Current();
        map<unsigned,int> * dic = 0;                              
        
        if (c > L && c < R) 
          dic = &_cjks[0];
        else if( c > LA && c < RA )
        {
          dic = &_cjks[1];               
        }
        else if(c > LB && c < RB)
        {
          dic = &_cjks[2];         
        }
        if(dic){
          _nCjks++;
          if(dic->count(c))
            (*dic)[c] ++;
          else
            (*dic)[c] = 1;       
        }
      }
      _sorted = false;
    }
    
    void Counter::Process(istream& is)
    {
      TextProcessor::Process(is);
      _nFiles ++;
    }
    
    void Counter::Output(ostream& os)
    {
      if(!_sorted)
      {
        _resList.clear();
        for(int i = 0;i!=3;++i)
          copy(_cjks[i].begin(),_cjks[i].end(),back_inserter(_resList));
        sort(_resList.begin(),_resList.end(),
             [](pair<unsigned,int> a,pair<unsigned,int>b){
               return
                a.second == b.second?
                a.first > b.first:
                a.second > b.second;
            });
        _sorted = true;
      }
      char buf[6];
      for(pair<unsigned,int>& it:_resList){
        u2u8( it.first,buf);
        os << buf << "\t" << it.second << endl;
      }
    }
   
   string Counter::GetStatistics()
    {
      char s [256];
      sprintf(s,
        "#[Summary]:\n"
        "%8d -- File(s) processed.\n"
        "%8d -- Total number of characters have been read.\n"
        "%8d -- Total number of Chinese characters.\n" 
        "%8d -- Number of distinguishabe Chinese characters.\n",
        _nFiles,
        _nChars,
        _nCjks,
        NDistinguishableCjks()
      );
      return s;
    }
  
}
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


#ifndef CCCOUNTER_H
#define CCCOUNTER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "textprocessor.h"
namespace CCCount{
  using namespace std; 
  
  class Counter:public TextProcessor<pair<unsigned,int> >
  {
    int _nChars, _nCjks, _nFiles;
    bool _sorted;
    map<unsigned,int> _cjks[3];
    
  public:
    Counter():_nChars(0),_nCjks(0),_nFiles(0),_sorted(false)
    {
     for(int i=0;i!=3;++i)
       _cjks[i] = map<unsigned,int>();
    }
    virtual void Process(const string& s);
    virtual void Process(istream& is);
    virtual void Output(ostream& os);
    virtual string GetStatistics();
    
    int NChars(){return _nChars;}
    int NCjks(){return _nCjks;}
    int NDistinguishableCjks(){return _cjks[0].size()+_cjks[1].size()+_cjks[2].size();}
    int NFiles(){return _nFiles;}
  };
 
}
#endif // CCCOUNTER_H

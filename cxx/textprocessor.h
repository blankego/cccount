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
#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
namespace CCCount{
  template<class T>
  class TextProcessor
  {
    
  protected:
    vector<T> _resList;
    
  public:
    virtual void Process(const string& s)=0;
    virtual void Process(istream& is);
    virtual string GetResultList();
    virtual void Output(ostream& os)=0;
    virtual string GetStatistics()=0;

  };
  
  template<class T>
  void TextProcessor<T>::Process(istream& in)
  {
    string s;
    while(!in.eof()){
      getline(in,s);
      Process(s);
      s.clear();
    }
    
  }
  
  template<class T>
  string TextProcessor<T>::GetResultList()
  {
      stringstream ss;
      Output(ss);
      return ss.str();
  }
  
}
#endif // TEXTPROCESSOR_H

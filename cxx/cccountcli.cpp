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


#include "cccountcli.h"
#include <iostream>
#include <string>
#include <cstring>
#include "cccounter.h"
#include <map>
#include <sstream>
#include <fstream>
using namespace std;
namespace CCCount{
  CCCountCLI* CCCountCLI::_inst = 0;
  
  CCCountCLI& CCCountCLI::Instance()
  {
   if(!_inst)
     _inst = new CCCountCLI;
    return *_inst;
  }
  
  void CCCountCLI::SetUp(int argc, char** argv)
  {
      po::options_description desc("USAGE: cjkcount [OPTION]+ [FILE]+\n\n"  
                                    "print the counts of total and distinguishable "
                                    "Chinese characters\nappeared in given file(s)\n\n"
                                    "If no file is provided, it reads text from stdin.\n\n" 
                                    "OPTIONS:\n");
      po::positional_options_description p;
      p.add("inputfile", -1);
      
      desc.add_options()
        ("help,h","show this message and exit")
        ("nosummary,S", "do not show the summary (default output) ")
        ("listall,l", "list all distinguishable Chinese characters appeared, with corresponding counting, "
          "sorted by the counts descendantly.")
        ("inputfile,f",po::value< vector<string> >(&_files),"input files");
      try
      {  
        po::store(po::command_line_parser(argc,argv).options(desc)
          .style(po::command_line_style::unix_style|
            po::command_line_style::allow_sticky|po::command_line_style::allow_guessing)
          .positional(p).run(),_ops);
        po::notify(_ops);      
        
      }
      catch(exception& e)
      {
        cout << e.what()<<endl;
        ShowHelp(desc,false);
      }
      if(_ops.count("help"))
        ShowHelp(desc);
  }
  void CCCountCLI::ShowHelp(po::options_description& desc,bool normal)
  {
      cout << desc <<endl;
      exit(normal? 0: 1);
  }
void CCCountCLI::DoJob()
{
  if(_files.size())
  {
    for(auto f= _files.begin(); f!= _files.end();++f)
    {
      ifstream fs((*f).c_str());
      _counter->Process(fs);
    }
  }
  else
  {
    _counter->Process(cin);
  }
}
void CCCountCLI::ShowResult()
{
  if(_ops.count("listall"))
    _counter->Output(cout);
  if(!_ops.count("nosummary"))
    cout << endl << _counter->GetStatistics();
   
}


}

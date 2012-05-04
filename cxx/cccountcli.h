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


#ifndef CCCOUNTCLI_H
#define CCCOUNTCLI_H
#include <boost/program_options.hpp>
//#include <boost/filesystem.hpp>
#include <vector>
#include "cccounter.h"
namespace CCCount{
  namespace po = boost::program_options;
  class CCCountCLI
  {
    
    static CCCountCLI* _inst;
    CCCountCLI():_counter(0){ _counter = new Counter;}
    CCCountCLI(const CCCountCLI&){}
    ~CCCountCLI(){delete _counter;}
    CCCountCLI& operator=(const CCCountCLI&){}
    
    vector<string> _files;    
    po::variables_map _ops;
    TextProcessor<pair<unsigned,int> > *_counter;
  public:
    static CCCountCLI& Instance();
    void SetUp(int argc,char** argv);
    void DoJob();
    void ShowResult();
    void ShowHelp(po::options_description&,bool normal=true);
  };
}
#endif // CCCOUNTCLI_H

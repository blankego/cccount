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


#include "utils.h"
namespace CCCount{
  string u2u8string(unsigned uni)
  {
    char buf[6];
    int i = u2u8(uni,buf);
    string s(buf);
    return s;
  }

  int u2u8(unsigned uni, char* buf )
  {
      
    int res = 0;
    if (uni<0x80)      
    {
	*buf = uni;
	*(buf+1) = 0;
	res = 1;
    }  
    else if (uni<0x0800)
    {
	*buf = uni>>6  & 0x1F | 0xC0;
	*(buf+1) = uni  & 0x3F | 0x80;
	*(buf+2) = 0;
	res = 2;
    }
    else if (uni<0x010000){
	*buf = uni>>12 & 0x0F | 0xE0;
	*(buf+1) = uni>>6  & 0x3F | 0x80;
	*(buf+2) = uni>>0  & 0x3F | 0x80;
	*(buf+3) = 0;
	res = 3;
    }
    else if (uni<0x110000)
    {
	*buf = uni>>18 & 0x07 | 0xF0;
	*(buf+1) = uni>>12 & 0x3F | 0x80;
	*(buf+2) = uni>>6  & 0x3F | 0x80;
	*(buf+3) = uni>>0  & 0x3F | 0x80;
	*(buf+4) = 0;
	res = 4;
    }
    return res;
  }
}

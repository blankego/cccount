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


#include "utf8iterator.h"
namespace CCCount{
  enum {
    C1 = 0x80,
    H2MARK = 0b110,
    H2DATA = 0b00011111,
    H3MARK = 0b1110,
    H3DATA = 0b00001111,
    H4MARK = 0b11110,
    H4DATA = 0b00000111,
    H5MARK = 0b111110,
    H5DATA = 0b00000011,
    H6MARK = 0b1111110,
    H6DATA = 0b00000001, 
    
    TMARK = 0b10,  
    TDATA = 0b00111111,
    FULL = 0xFF,
  };
  bool Utf8Iterator::MoveNext()
  {
    _curr += _nextStep;
    PeekChar();
    return _curr != _end;
  }
  
  void Utf8Iterator::PeekChar()
  {
    //one byte code
    unsigned  c =(unsigned char) *_curr;
    if(!(c&C1))
    {
      _nextStep = 1;
      _uni = c;
      return;    
    }
     //a negative return value means the current char is not a valid unicode char
    _uni = -1;
    unsigned  c2,c3,c4,c5,c6;
    
    //multi-byte code
    if(c>>5 == H2MARK &&  _end - _curr > 1 && 
      (c2 = (unsigned char)*(_curr+1))>>6 == TMARK) 
    {
        _uni = ((c&H2DATA )<< 6) | c2&TDATA;
        _nextStep = 2;
    }
    else if(c>>4 == H3MARK && _end - _curr >2 && 
      (c2 = (unsigned char)*(_curr+1))>>6 == TMARK && 
      (c3 = (unsigned char)*(_curr+2))>>6 == TMARK 
    )
    {            
        _uni = ((c&H3DATA )<< 12) | ((c2&TDATA) << 6) | c3&TDATA ;       
        _nextStep = 3;
    }
    else if(c>>3 == H4MARK && _end - _curr > 3 &&
      (c2 = (unsigned char)*(_curr+1))>>6 == TMARK && 
      (c3 = (unsigned char)*(_curr+2))>>6 == TMARK &&
      (c4 = (unsigned char)*(_curr+3))>>6 == TMARK
    )
    {
        _uni = ((c&H3DATA) << 18) | ((c2&TDATA) << 12) | ((c3&TDATA )<<6) | c4&TDATA;
        _nextStep = 4;
    }
    else if(c>>2 == H5MARK && _end - _curr > 4 &&
      (c2 = (unsigned char)*(_curr+1))>>6 == TMARK &&
      (c3 = (unsigned char)*(_curr+2))>>6 == TMARK && 
      (c4 = (unsigned char)*(_curr+3))>>6 == TMARK && 
      (c5 = (unsigned char)*(_curr+4))>>6 == TMARK 
    )
    {
        _uni = ((c&H3DATA )<< 24) | ((c2&TDATA )<< 18) | ((c3&TDATA )<<12) | ((c4&TDATA )<< 6) | c5&TDATA;
        _nextStep = 5;
    }
  }
  
  
  Utf8Iterator& Utf8Iterator::Reset()
  {
    _curr = _begin - 1;
    _nextStep = 1;
    return *this;
  }
}
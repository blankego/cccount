#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CCCOUNT
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include "../cccounter.h"
#include "../utf8iterator.h"
#include "../utils.h"
using namespace CCCount;
struct Fixture
{
  string s;
  Fixture():s(u8"a\u4e86\u0235\u0204\u5927\u624b\U0002241c_\U000201ce\u5927!"){}
};
BOOST_FIXTURE_TEST_SUITE(utf8, Fixture)
BOOST_AUTO_TEST_CASE(utf8iter)
{
    Utf8Iterator it(s);
    int i;
    BOOST_CHECK( it.MoveNext());
    BOOST_CHECK_EQUAL(it.Current(),97);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x4e86);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x0235);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x0204);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x5927);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x624b);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x2241c);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),95);
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),0x201ce);
    it.MoveNext();
    //it.Current();
    it.MoveNext();
    BOOST_CHECK_EQUAL(it.Current(),(unsigned char)'!');
    BOOST_CHECK_EQUAL(it.MoveNext(),0);
    
}
BOOST_AUTO_TEST_CASE(countertest)
{
  Counter  c;
  c.Process(s);
  BOOST_CHECK_EQUAL(c.NChars(),11);
  BOOST_CHECK_EQUAL(c.NCjks(),6);
  BOOST_CHECK_EQUAL(c.NDistinguishableCjks(),5);
}
BOOST_AUTO_TEST_CASE(uni2utf8)
{
  char buf[6];
  BOOST_CHECK_EQUAL(u2u8(0x2241c,buf),4);
  BOOST_CHECK_EQUAL(strcmp(buf,u8"\U0002241c"),0);
  BOOST_CHECK_EQUAL(u2u8(0x0235,buf),2);
  BOOST_CHECK_EQUAL(strcmp(buf,u8"ȵ"),0);
  BOOST_CHECK_EQUAL(u2u8(0x5927,buf),3);
  BOOST_CHECK_EQUAL(strcmp(buf,"大"),0);
  BOOST_CHECK_EQUAL(u2u8(97,buf),1);
  BOOST_CHECK_EQUAL(strcmp(buf,"a"),0);
  BOOST_CHECK_EQUAL(u2u8string(0x2241c).compare(u8"\U0002241c"),0);
}
BOOST_AUTO_TEST_SUITE_END()
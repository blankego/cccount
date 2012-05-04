#!/usr/bin/env ruby
# encoding: utf-8
require "stringio"

require_relative "../cccount"
require "minitest/autorun"
require "minitest/reporters"
MiniTest::Unit.runner = MiniTest::SuiteRunner.new
MiniTest::Unit.runner.reporters << MiniTest::Reporters::RubyMineReporter.new
class TestCharIterator < MiniTest::Unit::TestCase

  def setup
	  path = File.dirname(__FILE__)
	  @a = File.join(path,"a.txt")
	  @b = File.join(path,"b.txt")
    @ins = StringIO.new "a不b\n是,𢏺!"
    @s1 = "天地玄黃"
    @insGbk =StringIO.new  Encoding::Converter.new(Encoding::UTF_8,Encoding::GBK).convert @s1
	  @insCjkAll = StringIO.new "𪜁a𠀀b𫝀,㐁;且0"
	  @cjkList = "且	1\n㐁	1\n𠀀	1\n𪜁	1\n𫝀	1\n"
	  @cjkSorted = @cjkList.chomp.split("\n").map{|ent|ent.split("\t")}
	  @cjkArr = @cjkList.split(/\t1\n/)
  end
  def test_char_iterator1
    it = CharIterator.new(@ins)
    arr = it.to_a
    assert_equal 8, arr.length
    assert_equal '𢏺'.ord, arr[6]
    it = CharIterator.new(@insGbk,Encoding::GBK)
    arr = it.to_a
    assert_equal 4 ,arr.length
    assert_equal ?玄.ord, arr[2]

  end

	def test_counter1
		counter = Counter.new
		counter.process @insCjkAll
		assert_equal 10, counter.nChr
		assert_equal 5, counter.nCjks
		res = counter.listStr<<"\n"
		assert_equal @cjkList,res
		assert_equal @cjkArr, counter.Cjks.map{|c|c.chr Encoding::UTF_8}
		assert_equal @cjkSorted,
		             counter.sortedList.map{|ent|[ent[0].chr(Encoding::UTF_8),ent[1].to_s]}
	end

	def test_cli
		require "ostruct"
		require "optparse"
		oldStdout = $stdout
		$stdout = StringIO.new
		cli = CCCountCLI.new ["-d",@a,@b]
		assert_equal "𤲴\n", $stdout.string
		$stdout = StringIO.new
		cli = CCCountCLI.new ["-i",@a,@b]
		assert_equal "一\n", $stdout.string
		$stdout = oldStdout
	end
end


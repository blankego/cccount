#! /usr/bin/env ruby
# encoding: utf-8
class CharIterator

  include Enumerable

  #@param inStream [#each_line]
  #@param inEncoding [Encoding]
  def initialize(inStream, inEncoding = Encoding::UTF_8)
    @ins = inStream
    @conv = inEncoding == Encoding::UTF_8 ? lambda{|s|s} : lambda{
        conv = Encoding::Converter.new( inEncoding, Encoding::UTF_8)
        conv.method :convert
    }[]
  end

  #@yieldparam [Fixnum]
  def each
    @ins.each_line do |line|
      line = @conv[line]
      line.each_codepoint{|cp| yield cp}
    end
  end
end



class Counter

  Cjk0 = 0x4e00..0x9fcb
  CjkA = 0x3400..0x4db5
  CjkB = 0x20000..0x2a6d6
  CjkC = 0x2a700..0x2b734
  CjkD = 0x2b740..0x2b81d

  #@param inEncoding [Encoding]
  def initialize(inEncoding=Encoding::UTF_8)
    @nChr, @nFiles, @nCjks, @cjk, @others = 0,0,0,0.upto(4).map{Hash.new(0)},Hash.new(0)
    @enc = inEncoding
  end

  attr_reader :nChr,:nFiles,:others

  def nCjks
    @cjk.map{|part|part.values.reduce(0,:+)}.reduce(0,:+)
  end

  def process(inStream)
    it = CharIterator.new inStream, @enc
    @nFiles += 1
    it.each { |c|
      @nChr += 1
      if c > 127
        case c
          when Cjk0
            @cjk[0][c] += 1
          when CjkA
            @cjk[1][c] += 1
          when CjkB
            @cjk[2][c] += 1
          when CjkC
            @cjk[3][c] += 1
          when CjkD
            @cjk[4][c] += 1
	        else
		        @others[c] += 1
        end
      else
	      @others[c] += 1
      end
    }
	  self
  end

	def sortedList
		@sortedList ||= @cjk.map{|h|h.sort_by{|ent|[-ent[1],ent[0]]}}.reduce(:concat)
	end

  #@param outStream [#<<]
	def listStr
		sortedList.map{|ent|"%c\t%d" % ent}.join("\n")
	end
  def othersStr
	  @others.sort_by{|ent|[-ent[1],ent[0]]}.map{|ent| "%c\t%d" % ent}.join("\n")
  end
  def nDistinguishable
     @cjk.reduce(0){|a,b|a + b.length}
  end

  def Cjks
	  @cjk.map(&:keys).reduce(:+)
  end

  def summary
    "#[Summary]:
    %8d -- Total number of characters have been read.
    %8d -- Total number of Chinese characters.
    %8d -- Number of distinguishable Chinese characters.
    %8d -- Files processed.
    " % [nChr,nCjks, nDistinguishable,nFiles]
  end
end

class CCCountOptParser
	def self.parse (args)
		scriptName = File.basename(__FILE__)
		options = OpenStruct.new
		options.fromEncoding = Encoding::UTF_8
		options.toEncoding = Encoding::UTF_8
		options.summary = true
		options.list = false
		options.others = false
		options.difference = false
		options.intersect = false

		opts = OptionParser.new do |opts|
			opts.banner = "Usage: #{scriptName} [OPTIONS] [FILE[S]]"
			opts.separator ''
			opts.separator "Specific options:"
			opts.on("-f","--from ENCODING","Specify the encoding of input text, default is utf-8"
			){|enc|
		    options.fromEncoding = Encoding::find enc
			}
			opts.on("-t","--to ENCODING","Specify the encoding of output text, defaut is utf-8"
			){|enc|
				options.toEncoding = Encoding::find enc
			}
			opts.on("-l","--[no-]list","List all appeared distinguishable Chinese characters," +
						" with corresponding counting, sorted by the counts descendantly."
			){|l|
				options.list = l
			}
			opts.on("-s","--[no-]summary","Show counting summary."
			){|s| options.summary = s}
			opts.on("-o","--[no-]others","List other characters besides cjk"
			){|o| options.others = o}
			opts.on("-d","--[no-]difference","Make a difference between cjk charsets of 2 files"
			){|d| options.difference = d}
			opts.on("-i","--[no-]intersect","Make a intersection between cjk charsets of 2 files"
			){|i| options.intersect = i}

		end
		opts.parse!(args)
		options
	end
end
class CCCountCLI

	# @param args [Array]
	def initialize (args = nil)
		args ||= ARGV.clone
		@opts = CCCountOptParser.parse(args)
		@files = args
		if @opts.difference || @opts.intersect
			if @files.length != 2
				puts "You have give 2 files to do difference or intersection"
				exit
			end
			fs=[]
			0.upto(1){|i|
				fs << Counter.new(@opts.fromEncoding).process(File.new(@files[i]))
			}
			lst = @opts.difference ? 	fs[0].Cjks - fs[1].Cjks : fs[0].Cjks & fs[1].Cjks
			puts lst.map{|c|c.chr @opts.toEncoding}.join('\n')

		else
			counter = Counter.new(@opts.fromEncoding)
			if args.length == 0
				counter.process $stdin
			else
				@files.each { |fname|
					File.open fname do |file|
						counter.process file
					end
				}
			end



			#show result
			if @opts.list
				puts counter.listStr.encode(@opts.toEncoding)
			end
			if @opts.others
				puts counter.othersStr.encode(@opts.toEncoding)
			end
			if @opts.summary
				puts counter.summary.encode(@opts.toEncoding)
			end
		end
	end

end

if $0 == __FILE__

	require "ostruct"
	require "optparse"
	CCCountCLI.new(ARGV)

end
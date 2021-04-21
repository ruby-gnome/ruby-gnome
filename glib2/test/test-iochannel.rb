# Copyright (C) 2015-2021  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require 'test/unit'
require 'glib2'

require 'tempfile'

class TestGIOChannel < Test::Unit::TestCase
  def setup
    @content = "aaa\nbbb\nccc\nあああ\n"
    @sjis_content = @content.encode("CP932")

    @file = Tempfile.new("glib2-content")
    @file.open
    @file.binmode
    @file.print(@content)
    @file.close

    @sjis_file = Tempfile.new("glib2-sjis-content")
    @sjis_file.open
    @sjis_file.binmode
    @sjis_file.print(@sjis_content)
    @sjis_file.close
  end

  def test_open
    write_test_file = Tempfile.new("glib2-write-test")

    io = GLib::IOChannel.open(@file.path)
    io.close

    io = GLib::IOChannel.open(@file.path, "r")
    assert_equal(@content, io.read)
    io.close

    io = GLib::IOChannel.open(write_test_file.path, "w")
    assert_raises(RuntimeError){
      assert_equal(@content, io.read)
    }
    io.close

    GLib::IOChannel.open(@file.path) do |_io|
      assert_equal(@content, _io.read)
    end

    GLib::IOChannel.open(@file.path, "r") do |_io|
      assert_equal(@content, _io.read)
    end

    GLib::IOChannel.open(write_test_file.path, "w") do |_io|
      io = _io
      assert_raises(RuntimeError) do
        assert_equal(@content, io.read)
      end
    end

    assert_raises(GLib::FileError) do
      GLib::IOChannel.new("foo")
    end
  end

  def test_getc
    io = GLib::IOChannel.new(@file.path)
    ["a", "b", "c", "あ"].each do |v|
      3.times do
        assert_equal(v.unpack("U")[0], io.getc)
      end
      assert_equal("\n".unpack("U")[0], io.getc)
    end
    assert_equal(nil, io.getc)
    io.close
  end

  def test_each_char
    text = @content.split(//u)
    io = GLib::IOChannel.new(@file.path)
    i = 0
    io.each_char {|ch|
      assert_equal(text[i].unpack("U")[0], ch)
      i += 1
    }
    io.close
  end

  def test_readchar
    io = GLib::IOChannel.new(@file.path)
    text = @content.split(//u)
    text.each do |v|
      assert_equal(v.unpack("U")[0], io.readchar)
    end
    assert_raises(EOFError) {
      io.readchar
    }
    io.close
  end

  def test_gets
    io = GLib::IOChannel.new(@file.path)
    assert_equal("aaa\n", io.gets)
    assert_equal("bbb\n", io.gets)
    assert_equal("ccc\n", io.gets)
    assert_equal("あああ\n", io.gets)
    assert_equal(nil, io.gets)
    io.close

    io = GLib::IOChannel.new(@file.path)
    assert_equal("aaa\nbbb\n", io.gets("bbb\n"))
    assert_equal("ccc\nあああ\n", io.gets("bbb\n"))
    assert_equal(nil, io.gets("bbb\n"))
    io.close
  end

  def test_readline
    io = GLib::IOChannel.new(@file.path)
    assert_equal("aaa\n", io.readline)
    assert_equal("bbb\n", io.readline)
    assert_equal("ccc\n", io.readline)
    assert_equal("あああ\n", io.readline)
    assert_raises(EOFError) {
      io.readline
    }
    io.close

    io = GLib::IOChannel.new(@file.path)
    assert_equal("aaa\nbbb\n", io.readline("bbb\n"))
    assert_equal("ccc\nあああ\n", io.readline("bbb\n"))
    assert_raises(EOFError) {
      io.readline("bbb\n")
    }
    io.close
  end

  def test_each_line
    lines = ["aaa\n", "bbb\n", "ccc\n", "あああ\n"]
    io = GLib::IOChannel.new(@file.path)
    i = 0
    io.each {|line|
      assert_equal(lines[i], line)
      i += 1
    }
    io.close

    io = GLib::IOChannel.new(@file.path)
    assert_raises(RuntimeError) {
      io.each {|line|
        raise "test"
      }
    }
    io.close

    io = GLib::IOChannel.new(@file.path)
    i = 0
    io.each_line {|line|
      assert_equal(lines[i], line)
      i += 1
    }
    io.close

    #Test for Enumerable
    GLib::IOChannel.open(@file.path) do |_io|
      io = _io
      io.each_with_index do |line, _i|
        assert_equal(lines[_i], line)
      end
    end

    assert_raises(ArgumentError) do
      io.each
    end
  end

  def test_read
    io = GLib::IOChannel.new(@file.path)
    assert_equal(@content, io.read)
    io.close

    io = GLib::IOChannel.new(@file.path)
    assert_equal(@content, io.read(100))
    io.close

    io = GLib::IOChannel.new(@file.path)
    assert_equal("aaa\nbbb\n", io.read(8))
    assert_equal("ccc\n", io.read(4))
    assert_equal("あああ\n", io.read(10))
    assert_equal("", io.read(10))
    assert_equal("", io.read(10))
    io.close
  end

  def test_seek
    text = @content
    io = GLib::IOChannel.new(@file.path)
    io.seek(5)
    assert_equal(text.unpack("U*")[5], io.getc)
    io.seek(6, GLib::IOChannel::SEEK_SET)
    assert_equal(text.unpack("U*")[6], io.getc)

    io.seek(1, GLib::IOChannel::SEEK_CUR)
    assert_equal(text.unpack("U*")[8], io.getc)

    io.pos = 0
    assert_equal(text.unpack("U*")[0], io.getc)

    io.set_pos(2)
    assert_equal(text.unpack("U*")[2], io.getc)

    io.close
  end

  def test_write
    write_test_file = Tempfile.new("glib2-write-test")

    io = GLib::IOChannel.new(write_test_file.path, "w")
    io.write("a\n")
    io.write("あいう\n")
    io.printf("a%sa\n", "a")
    io.print("a", 100, "a\n")
    io.puts("b", 200, "b")
    io.putc("c")
    io.putc("c".unpack("U")[0])
    io.putc("cc".unpack("U")[0])
    io.putc("あ".unpack("U")[0])
    io.putc("あ")
    io.putc("あい")   #Ignore 2nd char
    io.putc("aあ")   #Ignore 2nd char
    io.close

    # check them
    io = GLib::IOChannel.new(write_test_file.path, "r")
    assert_equal("a\n", io.gets)
    assert_equal("あいう\n", io.gets)
    assert_equal("aaa\n", io.gets)
    assert_equal("a100a\n", io.gets)
    assert_equal("b\n", io.gets)
    assert_equal("200\n", io.gets)
    assert_equal("b\n", io.gets)
    assert_equal("c".unpack("U")[0], io.getc)
    assert_equal("c".unpack("U")[0], io.getc)
    assert_equal("c".unpack("U")[0], io.getc)
    assert_equal("あ".unpack("U")[0], io.getc)
    assert_equal("あ".unpack("U")[0], io.getc)
    assert_equal("あ".unpack("U")[0], io.getc)
    assert_equal("a".unpack("U")[0], io.getc)
    io.close
  end

  def test_encoding
    io = GLib::IOChannel.new(@file.path)
    assert_equal("UTF-8", io.encoding)
    io.encoding = "Shift_JIS"
    assert_equal("Shift_JIS", io.encoding)
    assert_raises(GLib::ConvertError) {
      io.read
    }
    io.close

    io = GLib::IOChannel.new(@sjis_file.path)
    io.encoding = "Shift_JIS"
    assert_equal("Shift_JIS", io.encoding)
    assert_equal(@content, io.read)
    io.close
  end

  def test_error
    assert_raises(GLib::FileError) {
      # No such file or directory
      GLib::IOChannel.new("foo")
    }
  end

  sub_test_case("#create_watch") do
    def setup
      super
      @context = GLib::MainContext.new
    end

    def test_with_block
      GLib::IOChannel.open(@file.path) do |channel|
        received_condition = nil
        source = channel.create_watch(GLib::IOChannel::IN) do |_, condition|
          received_condition = condition
          GLib::Source::CONTINUE
        end
        begin
          source.attach(@context)
          10.times do
            @context.iteration(false)
          end
          assert_equal(GLib::IOCondition::IN, received_condition)
        ensure
          source.destroy
        end
      end
    end

    def test_set_callback
      GLib::IOChannel.open(@file.path) do |channel|
        received_condition = nil
        source = channel.create_watch(GLib::IOChannel::IN)
        source.set_callback do |_, condition|
          received_condition = condition
          GLib::Source::CONTINUE
        end
        begin
          source.attach(@context)
          10.times do
            @context.iteration(false)
          end
          assert_equal(GLib::IOCondition::IN, received_condition)
        ensure
          source.destroy
        end
      end
    end
  end
end

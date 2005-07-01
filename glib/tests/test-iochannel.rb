require 'test/unit'
require 'glib2'

$KCODE = "U"

class TestGIOChannel < Test::Unit::TestCase
  TEST_FILE = "test-iochannel.txt"
  TEST_SJIS_FILE = "test-iochannel-sjis.txt"
  TEST_WRITE_FILE = "test-write-file.txt"

  def test_open
    io = GLib::IOChannel.open(TEST_FILE)
    io.close

    io = GLib::IOChannel.open(TEST_FILE, "r")
    assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
    io.close

    io = GLib::IOChannel.open(TEST_WRITE_FILE, "w")
    assert_raises(RuntimeError){
      assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
    }
    io.close

    GLib::IOChannel.open(TEST_FILE) {|io|
      assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
    }

    GLib::IOChannel.open(TEST_FILE, "r") {|io|
      assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
    }

    GLib::IOChannel.open(TEST_WRITE_FILE, "w") {|io|
      assert_raises(RuntimeError){
	assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
      }
    }

    assert_raises(GLib::IOChannelError){
      io.close
    }

    assert_raises(GLib::FileError){
      GLib::IOChannel.new("foo")
    }

  end

  def test_getc
    io = GLib::IOChannel.new(TEST_FILE)
    ["a", "b", "c", "あ"].each do |v|
      3.times do 
	assert_equal(v.unpack("U")[0], io.getc)
      end
      assert_equal("\n"[0], io.getc)
    end
    assert_equal(nil, io.getc)
    io.close
  end

  def test_each_char
    text = "aaa\nbbb\nccc\nあああ\n".split(//)
    io = GLib::IOChannel.new(TEST_FILE)
    i = 0
    io.each_char {|ch|
      assert_equal(text[i].unpack("U")[0], ch)
      i += 1
    }
    io.close
  end

  def test_readchar
    io = GLib::IOChannel.new(TEST_FILE)
    text = "aaa\nbbb\nccc\nあああ\n".split(//)
    text.each do |v|
      assert_equal(v.unpack("U")[0], io.readchar)
    end
    assert_raises(EOFError) {
      io.readchar
    }
    io.close
  end

  def test_gets
    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\n", io.gets)
    assert_equal("bbb\n", io.gets)
    assert_equal("ccc\n", io.gets)
    assert_equal("あああ\n", io.gets)
    assert_equal(nil, io.gets)
    io.close

    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\nbbb\n", io.gets("bbb\n"))
    assert_equal("ccc\nあああ\n", io.gets("bbb\n"))
    assert_equal(nil, io.gets("bbb\n"))
    io.close
  end

  def test_readline
    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\n", io.readline)
    assert_equal("bbb\n", io.readline)
    assert_equal("ccc\n", io.readline)
    assert_equal("あああ\n", io.readline)
    assert_raises(EOFError) {
      io.readline
    }
    io.close

    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\nbbb\n", io.readline("bbb\n"))
    assert_equal("ccc\nあああ\n", io.readline("bbb\n"))
    assert_raises(EOFError) {
      io.readline("bbb\n")
    }
    io.close
  end

  def test_each_line
    lines = ["aaa\n", "bbb\n", "ccc\n", "あああ\n"]
    io = GLib::IOChannel.new(TEST_FILE)
    i = 0
    io.each {|line|
      assert_equal(lines[i], line)
      i += 1
    }
    io.close

    io = GLib::IOChannel.new(TEST_FILE)
    assert_raises(RuntimeError) {
      io.each {|line|
	raise "test"
      }
    }
    io.close

    io = GLib::IOChannel.new(TEST_FILE)
    i = 0
    io.each_line {|line|
      assert_equal(lines[i], line)
      i += 1
    }
    io.close

    #Test for Enumerable
    GLib::IOChannel.open(TEST_FILE) {|io| 
      io.each_with_index {|line, i|
	assert_equal(lines[i], line)
      }
    }

    assert_raises(ArgumentError){
      io.each
    }
  end

  def test_read
    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
    io.close

    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\nbbb\nccc\nあああ\n", io.read(100))
    io.close

    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("aaa\nbbb\n", io.read(8))
    assert_equal("ccc\n", io.read(4))
    assert_equal("あああ\n", io.read(10))
    assert_equal("", io.read(10))
    assert_equal("", io.read(10))
    io.close
  end

  def test_seek
    text = "aaa\nbbb\nccc\nあああ\n"
    io = GLib::IOChannel.new(TEST_FILE)
    io.seek(5)
    assert_equal(text[5], io.getc)
    io.seek(6, GLib::IOChannel::SEEK_SET)
    assert_equal(text[6], io.getc)

    io.seek(1, GLib::IOChannel::SEEK_CUR)
    assert_equal(text[8], io.getc)

    io.pos = 0
    assert_equal(text[0], io.getc)

    io.set_pos(2)
    assert_equal(text[2], io.getc)

    io.close
  end

  def test_write
    io = GLib::IOChannel.new(TEST_WRITE_FILE, "w")
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
    io = GLib::IOChannel.new(TEST_WRITE_FILE, "r")
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
    io = GLib::IOChannel.new(TEST_FILE)
    assert_equal("UTF-8", io.encoding)
    io.encoding = "Shift_JIS"
    assert_equal("Shift_JIS", io.encoding)
    assert_raises(GLib::ConvertError) {
      io.read
    }
    io.close

    io = GLib::IOChannel.new(TEST_SJIS_FILE)
    io.encoding = "Shift_JIS"
    assert_equal("Shift_JIS", io.encoding)
    assert_equal("aaa\nbbb\nccc\nあああ\n", io.read)
    io.close
  end

  def test_error
    assert_raises(GLib::FileError) {
      # No such file or directory
      GLib::IOChannel.new("foo")
    }
  end
end

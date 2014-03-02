# -*- coding: utf-8 -*-

class TestBufferedInputStream < Test::Unit::TestCase
  def test_buffered_input_stream
    assert_nothing_raised do
      Gio::BufferedInputStream.new(Gio::FileInputStream.new)
    end
  end
end

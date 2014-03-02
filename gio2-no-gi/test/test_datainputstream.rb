# -*- coding: utf-8 -*-

class TestDataInputStream < Test::Unit::TestCase
  def test_byte_order_property
    assert_equal Gio::DataStreamByteOrder::BIG_ENDIAN, Gio::DataInputStream.new(Gio::MemoryInputStream.new).byte_order
  end
end

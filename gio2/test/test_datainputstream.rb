# -*- coding: utf-8 -*-

class TestDataInputStream < Test::Unit::TestCase
  def test_byte_order_property
    assert_equal GLib::DATA_STREAM_BYTE_ORDER_BIG_ENDIAN, GLib::DataInputStream.new(GLib::MemoryInputStream.new).byte_order
  end
end

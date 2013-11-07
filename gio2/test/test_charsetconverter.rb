# -*- coding: utf-8 -*-

class TestCharsetConverter < Test::Unit::TestCase
  def test_converter
    assert_nothing_raised do
      Gio::CharsetConverter.new("UTF-8", "ASCII")
    end
  end
end

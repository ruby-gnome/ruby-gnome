# coding: utf-8
# Copyright (C) 2012-2014  Ruby-GNOME2 Project Team
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

class ClutterTextBufferTest < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    @text_buffer = Clutter::TextBuffer.new
  end

  def test_text
    string = "clutter test"
    @text_buffer.text = string
    assert_equal(string, @text_buffer.text)
  end

  def test_max_length
    length = 80
    @text_buffer.max_length = length
    assert_equal(length, @text_buffer.max_length)
  end

  def test_length_read_property
    string = "clutter test"
    length = string.length
    @text_buffer.text = string
    assert_equal(length, @text_buffer.length)
  end

  def test_insert_text
    string = "clutter test"
    @text_buffer.text = string

    append_string = "append"
    append_string_length = append_string.length
    result_string = string + append_string

    assert_equal(append_string_length,
                 @text_buffer.insert_text(string.length, append_string))
    assert_equal(result_string, @text_buffer.text)
  end

  def test_delete_text
    string = "clutter test string"
    @text_buffer.text = string

    delete_text = "clutter"
    delete_position = delete_text.length
    delete_string = " test"
    delete_string_length = delete_string.length
    result_string = "clutter string"

    assert_equal(delete_string_length,
                 @text_buffer.delete_text(delete_position,
                                          delete_string_length))
    assert_equal(result_string, @text_buffer.text)
  end
end

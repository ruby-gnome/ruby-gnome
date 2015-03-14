# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestGtkTextBuffer < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case "instance methods" do
    def setup
      @text_buffer = Gtk::TextBuffer.new
      @text_buffer.text = "Hello World!"
    end

    sub_test_case "#insert" do
      test "no options" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert(iter, "Ruby ")
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test ":interactive" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert(iter, "Ruby ", :interactive => true)
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test ":default_editable => true" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert(iter, "Ruby ",
                            :interactive => true,
                            :default_editable => true)
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test ":default_editable => false" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert(iter, "Ruby ",
                            :interactive => true,
                            :default_editable => false)
        assert_equal("Hello World!", @text_buffer.text)
      end
    end

    sub_test_case "#insert_at_cursor" do
      test "no options" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.place_cursor(iter)
        @text_buffer.insert_at_cursor("Ruby ")
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test ":interactive" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.place_cursor(iter)
        @text_buffer.insert_at_cursor("Ruby ",
                                      :interactive => true)
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test ":default_editable => true" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.place_cursor(iter)
        @text_buffer.insert_at_cursor("Ruby ",
                                      :interactive => true,
                                      :default_editable => true)
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test ":default_editable => false" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.place_cursor(iter)
        @text_buffer.insert_at_cursor("Ruby ",
                                      :interactive => true,
                                      :default_editable => false)
        assert_equal("Hello World!", @text_buffer.text)
      end
    end
  end
end

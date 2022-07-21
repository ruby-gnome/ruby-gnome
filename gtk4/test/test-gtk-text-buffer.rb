# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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

      test "GLib::Bytes" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert(iter, GLib::Bytes.new("Ruby "))
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

    sub_test_case "#insert_markup" do
      test "String" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert_markup(iter, "<b>Ruby</b> ")
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test "GLib::Bytes" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert_markup(iter, GLib::Bytes.new("<b>Ruby</b> "))
        assert_equal("Hello Ruby World!", @text_buffer.text)
      end

      test "n_bytes" do
        iter = @text_buffer.get_iter_at(:offset => "Hello ".bytesize)
        @text_buffer.insert_markup(iter, "<b>Ruby</b> language", 12)
        assert_equal("Hello Ruby World!", @text_buffer.text)
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

    test "#text=" do
      @text_buffer.text = "Hello Ruby World!"
      assert_equal("Hello Ruby World!", @text_buffer.text)
    end

    sub_test_case "#selection_bounds" do
      test "selected" do
        insert_iter =
          @text_buffer.get_iter_at(:offset => "Hel".bytesize)
        selection_bound_iter =
          @text_buffer.get_iter_at(:offset => "Hello Wor".bytesize)
        @text_buffer.select_range(insert_iter, selection_bound_iter)
        assert_equal([insert_iter.offset, selection_bound_iter.offset],
                     @text_buffer.selection_bounds.collect(&:offset))
      end

      test "not selected" do
        assert_nil(@text_buffer.selection_bounds)
      end
    end

    sub_test_case "#create_tag" do
      test "default" do
        tag = @text_buffer.create_tag
        assert_nil(tag.name)
      end

      test "named" do
        tag = @text_buffer.create_tag("new-tag")
        assert_equal("new-tag", tag.name)
      end
    end
  end
end

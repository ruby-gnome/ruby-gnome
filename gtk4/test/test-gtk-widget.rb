# Copyright (C) 2008-2022  Ruby-GNOME Project Team
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

class TestGtkWidget < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @widget = Gtk::Box.new(:horizontal)
  end

  sub_test_case "#set_size_request" do
    test("{width:, height:}") do
      entry = Gtk::Entry.new
      entry.set_size_request(width: 50, height: 100)
      assert_equal([50, 100], entry.size_request)
    end

    test("{width:}") do
      entry = Gtk::Entry.new
      assert_raise(ArgumentError.new(":height is missing")) do
        entry.set_size_request(width: 50)
      end
    end

    test("{height:}") do
      entry = Gtk::Entry.new
      assert_raise(ArgumentError.new(":width is missing")) do
        entry.set_size_request(height: 100)
      end
    end

    test("(width, height)") do
      entry = Gtk::Entry.new
      entry.set_size_request(50, 100)
      assert_equal([50, 100], entry.size_request)
    end

    test("nothing") do
      entry = Gtk::Entry.new
      assert_raise(ArgumentError.new("wrong number of arguments " +
                                     "(given 0, expected 1..2)")) do
        entry.set_size_request
      end
    end
  end

  sub_test_case "#translate_coordinates" do
    test "no common toplevel" do
      win1 = Gtk::Window.new
      label1 = Gtk::Label.new("one")
      win1.child = label1
      win1.present

      win2 = Gtk::Window.new
      label2 = Gtk::Label.new("one")
      win2.child = label2
      win2.present

      assert_nil(label1.translate_coordinates(label2, 0, 0))
    end

    test "translated" do
      win1 = Gtk::Window.new
      label1 = Gtk::Label.new("one")
      win1.child = label1
      win1.present
      assert_equal([0, 0], label1.translate_coordinates(win1, 0, 0))
    end
  end

  sub_test_case "predicates" do
    test "#in_destruction?" do
      entry = Gtk::Entry.new
      assert do
        not entry.in_destruction?
      end
    end
  end

  sub_test_case ".bind_template_child" do
    setup do
      @resource_data = File.read(fixture_path("simple_window.ui"))
    end

    test "nothing" do
      data = @resource_data
      widget_class = Class.new(Gtk::Window) do
        type_register "BindTemplateChildNothing"

        singleton_class.send(:define_method, :init) do
          super()
          set_template(:data => data)
        end
      end
      widget = widget_class.new
      assert_nil(widget.get_template_child(widget_class, :label))
    end

    test "bind" do
      data = @resource_data
      widget_class = Class.new(Gtk::Window) do
        type_register "BindTemplateChildBind"

        singleton_class.send(:define_method, :init) do
          super()
          set_template(:data => data)
          bind_template_child(:label)
        end
      end
      widget = widget_class.new
      assert_equal(Gtk::Label,
                   widget.get_template_child(widget_class, :label).class)
    end
  end
end

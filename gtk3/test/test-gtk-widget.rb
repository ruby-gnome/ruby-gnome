# Copyright (C) 2008-2020  Ruby-GNOME Project Team
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
    @widget = Gtk::Invisible.new
  end

  sub_test_case "class methods" do
    test ".style_properties" do
      assert_equal("cursor-aspect-ratio",
                   Gtk::Label.style_properties.collect(&:name).sort.first)
    end
  end

  def test_set_window
    attrs = Gdk::WindowAttr.new(100, 100, :input_only, :temp)
    window = Gdk::Window.new(nil, attrs, 0)
    @widget.window = window
    assert_equal(window, @widget.window)
  end

  def test_modify_color_component_with_nil_color
    assert_nothing_raised do
      @widget.override_color(:normal, nil)
    end

    assert_nothing_raised do
      @widget.override_background_color(:normal, nil)
    end
  end

  sub_test_case "#drag_source_set" do
    def setup
      only_gtk_version(3, 9, 16)
      super
    end

    test "targets - TargetEntry" do
      target = "text/uri-list"
      info = 0
      @widget.drag_source_set([:shift_mask, :control_mask],
                              [Gtk::TargetEntry.new(target, 0, info)],
                              :copy)
      list = @widget.drag_source_get_target_list
      assert_equal(info, list.find(target))
    end

    test "targets - Array" do
      target = "text/uri-list"
      info = 0
      @widget.drag_source_set([:shift_mask, :control_mask],
                              [[target, 0, info]],
                              :copy)
      list = @widget.drag_source_get_target_list
      assert_equal(info, list.find(target))
    end
  end

  sub_test_case "#drag_dest_set" do
    def setup
      only_gtk_version(3, 9, 16)
      super
    end

    test "targets - TargetEntry" do
      target = "text/uri-list"
      info = 0
      @widget.drag_dest_set(:all,
                            [Gtk::TargetEntry.new(target, 0, info)],
                            :copy)
      list = @widget.drag_dest_get_target_list
      assert_equal(info, list.find(target))
    end

    test "targets - Array" do
      target = "text/uri-list"
      info = 0
      @widget.drag_dest_set(:all,
                            [[target, 0, info]],
                            :copy)
      list = @widget.drag_dest_get_target_list
      assert_equal(info, list.find(target))
    end
  end

  sub_test_case "events" do
    def setup
      @widget = Gtk::EventBox.new
    end

    test "get" do
      assert_equal(Gdk::EventMask.new(0), @widget.events)
    end

    test "add" do
      @widget.add_events([:exposure_mask, :pointer_motion_mask])
      assert_equal(Gdk::EventMask::EXPOSURE_MASK |
                   Gdk::EventMask::POINTER_MOTION_MASK,
                   @widget.events)
    end

    test "set" do
      @widget.set_events([:exposure_mask, :pointer_motion_mask])
      assert_equal(Gdk::EventMask::EXPOSURE_MASK |
                   Gdk::EventMask::POINTER_MOTION_MASK,
                   @widget.events)
    end

    test "assign" do
      @widget.events = [:exposure_mask, :pointer_motion_mask]
      assert_equal(Gdk::EventMask::EXPOSURE_MASK |
                   Gdk::EventMask::POINTER_MOTION_MASK,
                   @widget.events)
    end
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

  sub_test_case "#style_get_property" do
    test("existent") do
      entry = Gtk::Entry.new
      assert do
        entry.style_get_property("focus-padding").is_a?(Integer)
      end
    end

    test("nonexistent") do
      entry = Gtk::Entry.new
      name = "nonexistent"
      assert_raise(ArgumentError.new("unknown style: #{name.inspect}")) do
        entry.style_get_property(name)
      end
    end
  end

  sub_test_case "#translate_coordinates" do
    test "no common toplevel" do
      win1 = Gtk::Window.new(:toplevel)
      label1 = Gtk::Label.new("one")
      win1.add(label1)
      win1.show_all

      win2 = Gtk::Window.new(:toplevel)
      label2 = Gtk::Label.new("one")
      win2.add(label2)
      win2.show_all

      assert_nil(label1.translate_coordinates(label2, 0, 0))
    end

    test "not realized" do
      win1 = Gtk::Window.new(:toplevel)
      label1 = Gtk::Label.new("one")
      win1.add(label1)
      assert_nil(label1.translate_coordinates(win1, 0, 0))
    end

    test "translated" do
      win1 = Gtk::Window.new(:toplevel)
      label1 = Gtk::Label.new("one")
      win1.add(label1)
      win1.show_all
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

  sub_test_case "#render_icon_pixbuf" do
    test "size: symbol" do
      window = Gtk::Window.new
      window.render_icon_pixbuf(:directory, :menu)
    end
  end

  sub_test_case ".bind_template_child" do
    setup do
      only_gtk_version(3, 12, 0)
      @resource_data = File.read(fixture_path("simple_window.ui"))
    end

    test "nothing" do
      data = @resource_data
      widget_class = Class.new(Gtk::Window) do
        type_register "BindTemplateChildNothing"

        singleton_class.send(:define_method, :init) do
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
          set_template(:data => data)
          bind_template_child(:label)
        end
      end
      widget = widget_class.new
      assert_equal(Gtk::Label,
                   widget.get_template_child(widget_class, :label).class)
    end
  end

  sub_test_case ".set_connect_func" do
    setup do
      only_gtk_version(3, 12, 0)
      @resource_data = File.read(fixture_path("simple_window.ui"))
    end

    test "connect" do
      data = @resource_data
      handler_names = []
      widget_class = Class.new(Gtk::Window) do
        type_register "SetConnectFunc"

        singleton_class.send(:define_method, :init) do
          set_template(:data => data)
          bind_template_child(:label)
          set_connect_func do |handler_name|
            handler_names << handler_name
            lambda do
              # Do nothing
            end
          end
        end
      end
      widget_class.new
      assert_equal(["on_move_cursor"], handler_names)
    end
  end
end

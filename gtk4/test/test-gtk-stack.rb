# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

class TestGtkStack < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 10, 0)
    @stack = Gtk::Stack.new
  end

  class TestAdd < self
    def setup
      super
      @child = Gtk::EventBox.new
    end

    def test_return_value
      assert_equal(@stack, @stack.add(@child))
    end

    def test_added
      @stack.add(@child)
      assert_equal([@child], @stack.children)
    end

    def test_name
      widget_name = "set widget name"
      @stack.add(@child, widget_name)
      assert_equal(widget_name,
                   @stack.child_get_property(@child, "name"))
    end

    def test_name_add_title
      widget_name = "set widget name"
      widget_title = "set widget title"
      @stack.add(@child, widget_name, widget_title)
      assert_equal([
                     widget_name,
                     widget_title,
                   ],
                   [
                     @stack.child_get_property(@child, "name"),
                     @stack.child_get_property(@child, "title"),
                   ])
    end
  end

  def test_homogeneous_accessors
    @stack.homogeneous = false
    assert_false(@stack.homogeneous?)
  end

  def test_transition_duration_accessors
    duration = 500
    @stack.transition_duration = duration
    assert_equal(duration, @stack.transition_duration)
  end

  def test_transition_type_accessors
    stack_transition_type = Gtk::Stack::TransitionType::SLIDE_UP
    @stack.transition_type = stack_transition_type
    assert_equal(stack_transition_type, @stack.transition_type)
  end

  class TestVisibleChild < self
    def setup
      super
      @visible_widget = Gtk::EventBox.new
      @visible_widget.show
      @visible_widget_name = "visible widget"
      @stack.add(@visible_widget, @visible_widget_name)
    end

    def test_assign
      @stack.visible_child = @visible_widget
      assert_equal(@visible_widget, @stack.visible_child)
    end

    def test_widget
      @stack.set_visible_child(@visible_widget)
      assert_equal(@visible_widget, @stack.visible_child)
    end

    def test_name
      @stack.set_visible_child(@visible_widget_name)
      assert_equal(@visible_widget_name,
                   @stack.visible_child_name)
    end

    def test_name_and_transition_type
      @stack.set_visible_child(@visible_widget_name, :crossfade)
      assert_equal(@visible_widget_name,
                   @stack.visible_child_name)
    end

    def test_child_by_name
      only_gtk_version(3, 12, 0)
      assert_kind_of(Gtk::EventBox,
                     @stack.get_child_by_name(@visible_widget_name))
    end
  end

  class TestEnum < self
    def test_transition_type
      assert_const_defined(Gtk::Stack::TransitionType, :CROSSFADE)
    end
  end
end

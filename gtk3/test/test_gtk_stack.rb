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

  def test_set_add_named
    widget = Gtk::EventBox.new
    widget_name = "set widget name"
    assert_nothing_raised do
      @stack.set_add_named(widget, widget_name)
    end
  end

  def test_set_add_titled
    widget = Gtk::EventBox.new
    widget_name = "set widget name"
    widget_title = "set widget title"
    assert_nothing_raised do
      @stack.set_add_titled(widget, widget_name, widget_title)
    end
  end

  def test_homogeneous_accessors
    @stack.homogeneous = false
    assert_equal(false, @stack.homogeneous?)
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

  def test_visible_child_accessors
    widget = Gtk::EventBox.new
    @stack.visible_child = widget
    assert_nil(@stack.visible_child)
  end

  class TestEnum < self
    def test_transition_type
      assert_const_defined(Gtk::Stack::TransitionType, :CROSSFADE)
    end
  end
end

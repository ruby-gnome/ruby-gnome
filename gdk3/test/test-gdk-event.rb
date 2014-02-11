# -*- coding: utf-8 -*-
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

class TestGdkEvent < Test::Unit::TestCase
  class TestAny < self
    def setup
      @event = Gdk::EventAny.new(:delete)
    end

    def test_delete
      assert_equal("GDK_DELETE",
                   Gdk::EventAny.new(:delete).event_type.name)
    end

    def test_destroy
      assert_equal("GDK_DESTROY",
                   Gdk::EventAny.new(:destroy).event_type.name)
    end

    def test_window
      assert_nothing_raised do
        @event.window
      end
    end

    def test_send_event
      assert_false(@event.send_event?)
    end
  end

  class TestKey < self
    def setup
      @key = Gdk::EventKey.new(:key_press)
    end

    def test_key_press
      assert_equal("GDK_KEY_PRESS",
                   Gdk::EventKey.new(:key_press).event_type.name)
    end

    def test_key_release
      assert_equal("GDK_KEY_RELEASE",
                   Gdk::EventKey.new(:key_release).event_type.name)
    end

    def test_time
      assert_kind_of(Integer, @key.time)
    end

    def test_state
      assert_not_nil(@key.state)
    end

    def test_keyval
      assert_kind_of(Integer, @key.keyval)
    end
  end

  class TestButton < self
    def setup
      @button = Gdk::EventButton.new(:button_press)
    end

    def test_button_press
      assert_equal("GDK_BUTTON_PRESS",
                   Gdk::EventButton.new(:button_press).event_type.name)
    end

    def test_button2_press
      assert_equal("GDK_2BUTTON_PRESS",
                   Gdk::EventButton.new(:button2_press).event_type.name)
    end

    def test_button3_press
      assert_equal("GDK_3BUTTON_PRESS",
                   Gdk::EventButton.new(:button3_press).event_type.name)
    end

    def test_button_release
      assert_equal("GDK_BUTTON_RELEASE",
                   Gdk::EventButton.new(:button_release).event_type.name)
    end

    def test_time
      assert_kind_of(Integer, @button.time)
    end

    def test_x
      assert_kind_of(Float, @button.x)
    end

    def test_y
      assert_kind_of(Float, @button.y)
    end

    def test_state
      assert_not_nil(@button.state)
    end

    def test_button
      assert_kind_of(Integer, @button.button)
    end

    def test_x_root
      assert_kind_of(Float, @button.x_root)
    end

    def test_y_root
      assert_kind_of(Float, @button.y_root)
    end
  end

  class TestTouch < self
    def setup
      @touch = Gdk::EventTouch.new(:touch_begin)
    end

    def test_window
      assert_nothing_raised do
        @touch.window
      end
    end

    def test_send_event
      assert_false(@touch.send_event?)
    end

    def test_time
      assert_kind_of(Integer, @touch.time)
    end

    def test_x
      assert_kind_of(Float, @touch.x)
    end

    def test_y
      assert_kind_of(Float, @touch.y)
    end

    def test_axes
      assert_nothing_raised do
        @touch.axes
      end
    end

    def test_state
      assert_not_nil(@touch.state)
    end

    def test_touch_begin
      assert_equal("GDK_TOUCH_BEGIN",
                   Gdk::EventTouch.new(:touch_begin).event_type.name)
    end

    def test_touch_update
      assert_equal("GDK_TOUCH_UPDATE",
                   Gdk::EventTouch.new(:touch_update).event_type.name)
    end

    def test_touch_cancel
      assert_equal("GDK_TOUCH_CANCEL",
                   Gdk::EventTouch.new(:touch_cancel).event_type.name)
    end

    def test_touch_end
      assert_equal("GDK_TOUCH_END",
                   Gdk::EventTouch.new(:touch_end).event_type.name)
    end

    def test_emulating_pointer
      assert_nothing_raised do
        @touch.emulating_pointer
      end
    end

    def test_device
      assert_nothing_raised do
        @touch.device
      end
    end

    def test_x_root
      assert_kind_of(Float, @touch.x_root)
    end

    def test_y_root
      assert_kind_of(Float, @touch.y_root)
    end
  end

  class TestScroll < self
    def setup
      @scroll = Gdk::EventScroll.new
    end

    def test_time
      assert_kind_of(Integer, @scroll.time)
    end

    def test_x
      assert_kind_of(Float, @scroll.x)
    end

    def test_y
      assert_kind_of(Float, @scroll.y)
    end

    def test_state
      assert_not_nil(@scroll.state)
    end

    def test_direction
      assert_kind_of(Gdk::EventScroll::Direction, @scroll.direction)
    end

    def test_x_root
      assert_kind_of(Float, @scroll.x_root)
    end

    def test_y_root
      assert_kind_of(Float, @scroll.y_root)
    end
  end

  class TestMotion < self
    def setup
      @motion = Gdk::EventMotion.new
    end

    def test_time
      assert_kind_of(Integer, @motion.time)
    end

    def test_x
      assert_kind_of(Float, @motion.x)
    end

    def test_y
      assert_kind_of(Float, @motion.y)
    end

    def test_state
      assert_not_nil(@motion.state)
    end

    def test_x_root
      assert_kind_of(Float, @motion.x_root)
    end

    def test_y_root
      assert_kind_of(Float, @motion.y_root)
    end

    def test_request
      assert_nothing_raised do
        @motion.request
      end
    end
  end

  class TestVisibility < self
    def setup
      @visibility = Gdk::EventVisibility.new(:visibility_notify)
    end

    def test_state
      assert_kind_of(Gdk::EventVisibility::State, @visibility.state)
    end
  end

  class TestCrossing < self
    def setup
      @crossing = Gdk::EventCrossing.new(:enter_notify)
    end

    def test_enter_notify
      assert_equal("GDK_ENTER_NOTIFY",
                   Gdk::EventCrossing.new(:enter_notify).event_type.name)
    end

    def test_leave_notify
      assert_equal("GDK_LEAVE_NOTIFY",
                   Gdk::EventCrossing.new(:leave_notify).event_type.name)
    end

    def test_time
      assert_kind_of(Integer, @crossing.time)
    end

    def test_x
      # TODO: Is Float better?
      assert_kind_of(Integer, @crossing.x)
    end

    def test_y
      # TODO: Is Float better?
      assert_kind_of(Integer, @crossing.y)
    end

    def test_x_root
      assert_kind_of(Float, @crossing.x_root)
    end

    def test_y_root
      assert_kind_of(Float, @crossing.y_root)
    end

    def test_mode
      assert_kind_of(Gdk::EventCrossing::Mode, @crossing.mode)
    end

    def test_detail
      assert_not_nil(@crossing.detail)
    end

    def test_state
      assert_not_nil(@crossing.state)
    end
  end

  class TestFocus < self
    def setup
      @focus = Gdk::EventFocus.new
    end

    def test_in
      assert_false(@focus.in?)
    end
  end

  class TestConfigure < self
    def setup
      @configure = Gdk::EventConfigure.new
    end

    def test_x
      assert_kind_of(Integer, @configure.x)
    end

    def test_y
      assert_kind_of(Integer, @configure.y)
    end

    def test_width
      assert_kind_of(Integer, @configure.width)
    end

    def test_height
      assert_kind_of(Integer, @configure.height)
    end
  end

  class TestProperty < self
    def setup
      @property = Gdk::EventProperty.new
    end

    def test_atom
      assert_nothing_raised do
        @property.atom
      end
    end

    def test_time
      assert_kind_of(Integer, @property.time)
    end

    def test_state
      assert_equal(Gdk::EventProperty::State::NEW_VALUE, @property.state)
    end
  end

  class TestSelection < self
    def setup
      @selection = Gdk::EventSelection.new(:selection_clear)
    end

    def test_selection_clear
      assert_equal("GDK_SELECTION_CLEAR",
                   Gdk::EventSelection.new(:selection_clear).event_type.name)
    end

    def test_selection_notify
      assert_equal("GDK_SELECTION_NOTIFY",
                   Gdk::EventSelection.new(:selection_notify).event_type.name)
    end

    def test_selection_request
      assert_equal("GDK_SELECTION_REQUEST",
                   Gdk::EventSelection.new(:selection_request).event_type.name)
    end

    def test_selection
      assert_nothing_raised do
        @selection.selection
      end
    end

    def test_target
      assert_nothing_raised do
        @selection.target
      end
    end

    def test_property
      assert_nothing_raised do
        @selection.property
      end
    end

    def test_time
      assert_kind_of(Integer, @selection.time)
    end
  end

  class TestDND < self
    def setup
      @dnd = Gdk::EventDND.new(:drag_enter)
    end

    def test_drag_enter
      assert_equal("GDK_DRAG_ENTER",
                   Gdk::EventDND.new(:drag_enter).event_type.name)
    end

    def test_drag_leave
      assert_equal("GDK_DRAG_LEAVE",
                   Gdk::EventDND.new(:drag_leave).event_type.name)
    end

    def test_drag_motion
      assert_equal("GDK_DRAG_MOTION",
                   Gdk::EventDND.new(:drag_motion).event_type.name)
    end

    def test_drag_status
      assert_equal("GDK_DRAG_STATUS",
                   Gdk::EventDND.new(:drag_status).event_type.name)
    end

    def test_drop_start
      assert_equal("GDK_DROP_START",
                   Gdk::EventDND.new(:drop_start).event_type.name)
    end

    def test_drop_finished
      assert_equal("GDK_DROP_FINISHED",
                   Gdk::EventDND.new(:drop_finished).event_type.name)
    end

    def test_context
      assert_nothing_raised do
        @dnd.context
      end
    end

    def test_time
      assert_kind_of(Integer, @dnd.time)
    end

    def test_x_root
      assert_kind_of(Integer, @dnd.x_root)
    end

    def test_y_root
      assert_kind_of(Integer, @dnd.y_root)
    end
  end

  class TestProximity < self
    def setup
      @proximity = Gdk::EventProximity.new(:proximity_in)
    end

    def test_proximity_in
      assert_equal("GDK_PROXIMITY_IN",
                   Gdk::EventProximity.new(:proximity_in).event_type.name)
    end

    def test_proximity_out
      assert_equal("GDK_PROXIMITY_OUT",
                   Gdk::EventProximity.new(:proximity_out).event_type.name)
    end

    def test_time
      assert_kind_of(Integer, @proximity.time)
    end

    def test_device
      assert_nothing_raised do
        @proximity.device
      end
    end
  end

  class TestWindowState < self
    def setup
      @window_state = Gdk::EventWindowState.new
    end

    def test_changed_mask
      assert_nothing_raised do
        @window_state.changed_mask
      end
    end

    def test_new_window_state
      assert_nothing_raised do
        @window_state.new_window_state
      end
    end
  end

  class TestSetting < self
    def setup
      @setting = Gdk::EventSetting.new
    end

    def test_action
      assert_nothing_raised do
        @setting.action
      end
    end

    def test_name
      assert_nothing_raised do
        @setting.name
      end
    end
  end

  class TestOwnerChange < self
    def setup
      @owner_change = Gdk::EventOwnerChange.new
    end

    def test_owner
      assert_nothing_raised do
        @owner_change.owner
      end
    end

    def test_reason
      assert_nothing_raised do
        @owner_change.reason
      end
    end

    def test_selection
      assert_nothing_raised do
        @owner_change.selection
      end
    end

    def test_time
      assert_kind_of(Integer, @owner_change.time)
    end

    def test_selection_time
      assert_kind_of(Integer, @owner_change.selection_time)
    end
  end

  class TestGrabBroken < self
    def setup
      @grab_broken = Gdk::EventGrabBroken.new
    end

    def test_keyboard
      assert_boolean(@grab_broken.keyboard?)
    end

    def test_implicit
      assert_boolean(@grab_broken.implicit?)
    end

    def test_grab_window
      assert_nothing_raised do
        @grab_broken.grab_window
      end
    end
  end
end

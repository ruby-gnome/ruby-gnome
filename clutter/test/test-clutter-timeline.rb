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

class ClutterTimelineTest < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    default_duration = 100
    @timeline = Clutter::Timeline.new(default_duration)
  end

  def test_auto_reverse_accessors
    @timeline.auto_reverse = true
    assert_true(@timeline.auto_reverse?)
  end

  def test_delay_accessors
    delay_msec = 2000
    @timeline.delay = delay_msec
    assert_equal(delay_msec, @timeline.delay)
  end

  def test_direction_accessors
    timeline_direction = Clutter::TimelineDirection::FORWARD
    @timeline.direction = timeline_direction
    assert_equal(timeline_direction, @timeline.direction)
  end

  def test_duration_accessors
    duration_msec = 1500
    @timeline.duration = duration_msec
    assert_equal(duration_msec, @timeline.duration)
  end

  def test_repeat_count_accessors
    count = 5
    @timeline.repeat_count = count
    assert_equal(count, @timeline.repeat_count)
  end

  def test_progress_mode_accessors
    animation_mode = Clutter::AnimationMode::CUBIC_BEZIER
    @timeline.progress_mode = animation_mode
    assert_equal(animation_mode, @timeline.progress_mode)
  end
end

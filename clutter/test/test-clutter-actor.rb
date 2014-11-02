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

class ClutterActorTest < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    @actor = Clutter::Actor.new
  end

  def test_flags
    actor_flag = Clutter::ActorFlags::REACTIVE
    @actor.flags = actor_flag
    assert_equal(actor_flag, @actor.flags)

    @actor.unset_flags(actor_flag)
    assert_true(@actor.flags.empty?)
  end

  def test_scaled
    assert_false(@actor.scaled?)

    scale_x = 5.5
    scale_y = 7.5
    @actor.set_scale(scale_x, scale_y)
    assert_true(@actor.scaled?)
  end

  def test_rotation_angle
    only_clutter_version(1, 12, 0)
    rotate_angle = 0.5
    @actor.set_rotation_angle(Clutter::RotateAxis::X_AXIS, rotate_angle)
    assert_equal(rotate_angle,
                 @actor.get_rotation_angle(Clutter::RotateAxis::X_AXIS))
  end

  def test_name
    actor_name = "ClutterActor"
    @actor.name = actor_name
    assert_equal(actor_name, @actor.name)
  end

  def test_fixed_position_set
    @actor.fixed_position_set = true
    assert_true(@actor.fixed_position_set?)
  end

  def test_x_align
    align = Clutter::ActorAlign::FILL
    @actor.x_align = align
    assert_equal(align, @actor.x_align)
  end

  def test_y_align
    align = Clutter::ActorAlign::CENTER
    @actor.y_align = align
    assert_equal(align, @actor.y_align)
  end

  def test_content_gravity
    gravity = Clutter::ContentGravity::CENTER
    @actor.content_gravity = gravity
    assert_equal(gravity, @actor.content_gravity)
  end

  def test_opacity_accessors
    opacity_value = 220
    @actor.opacity = opacity_value
    assert_equal(opacity_value, @actor.opacity)
  end

  def test_easing_delay
    delay_msec = 300
    @actor.easing_delay = delay_msec
    assert_equal(delay_msec, @actor.easing_delay)
  end

  def test_easing_mode
    only_clutter_version(1, 12, 0)
    mode = Clutter::AnimationMode::EASE
    @actor.easing_mode = mode
    assert_equal(mode, @actor.easing_mode)
  end

  def test_pango_context
    @actor.create_pango_context
    assert_kind_of(Pango::Context, @actor.pango_context)
  end

  def test_width_accessors
    actor_width = 20
    @actor.width = actor_width
    assert_equal(actor_width, @actor.width)
  end

  def test_height_accessors
    actor_height = 30
    @actor.height = actor_height
    assert_equal(actor_height, @actor.height)
  end

  def test_x_accessors
    actor_x = 35
    @actor.x = actor_x
    assert_equal(actor_x, @actor.x)
  end

  def test_y_accessors
    actor_y = 15
    @actor.y = actor_y
    assert_equal(actor_y, @actor.y)
  end

  def test_z_position_accessors
    only_clutter_version(1, 12, 0)
    actor_z_position = 55
    @actor.z_position = actor_z_position
    assert_equal(actor_z_position, @actor.z_position)
  end

  def test_pivot_point_accessors
    only_clutter_version(1, 12, 0)
    pivot_x = 1.0
    pivot_y = 3.0
    @actor.set_pivot_point(pivot_x, pivot_y)
    assert_equal([pivot_x, pivot_y], @actor.pivot_point)
  end

  def test_pivot_point_z_accessors
    only_clutter_version(1, 12, 0)
    pivot_z = 5.0
    @actor.pivot_point_z = pivot_z
    assert_equal(pivot_z, @actor.pivot_point_z)
  end

  def test_reactive_accessors
    @actor.reactive = true
    assert_true(@actor.reactive?)
  end

  def test_scale_accessors
    scale_x = 1.5
    scale_y = 3.5
    @actor.set_scale(scale_x, scale_y)
    assert_equal([scale_x, scale_y], @actor.scale)
  end

  def test_scale_z_accessors
    only_clutter_version(1, 12, 0)
    scale_z = 4.5
    @actor.scale_z = scale_z
    assert_equal(scale_z, @actor.scale_z)
  end

  def test_text_direction_accessors
    direction = Clutter::TextDirection::LTR
    @actor.text_direction = direction
    assert_equal(direction, @actor.text_direction)
  end

  def test_clutter_transform
    # TODO: !!needs CoglMatrix!!
    # Clutter::Matrix equals Cogl::Matrix
    # ref: https://developer.gnome.org/clutter/stable/clutter-Base-geometric-types.html#ClutterMatrix
    # Cogl::Matrix type is returned by Cogl::Matrix.init_identity API in clutter 1.18
  end

  class TestMargin < self
    def test_margin_top
      margin = 3.0
      @actor.margin_top = margin
      assert_equal(margin, @actor.margin_top)
    end

    def test_margin_right
      margin = 5.0
      @actor.margin_right = margin
      assert_equal(margin, @actor.margin_right)
    end

    def test_margin_left
      margin = 3.5
      @actor.margin_left = margin
      assert_equal(margin, @actor.margin_left)
    end

    def test_margin_bottom
      margin = 4.5
      @actor.margin_bottom = margin
      assert_equal(margin, @actor.margin_bottom)
    end
  end

  class TestEnum < self
    def test_allocation_flags
      assert_const_defined(Clutter::AllocationFlags, :DELEGATE_LAYOUT)
    end

    def test_actor_align
      assert_const_defined(Clutter::ActorAlign, :FILL)
    end

    def test_actor_flags
      assert_const_defined(Clutter::ActorFlags, :REACTIVE)
    end

    def test_content_gravity
      assert_const_defined(Clutter::ContentGravity, :CENTER)
    end

    def test_content_repeat
      only_clutter_version(1, 12, 0)
      assert_const_defined(Clutter::ContentRepeat, :BOTH)
    end

    def test_scaling_fliter
      assert_const_defined(Clutter::ScalingFilter, :LINEAR)
    end

    def test_request_mode
      assert_const_defined(Clutter::RequestMode, :WIDTH_FOR_HEIGHT)
    end

    def test_offscreen_redirect
      assert_const_defined(Clutter::OffscreenRedirect, :ALWAYS)
    end
  end
end

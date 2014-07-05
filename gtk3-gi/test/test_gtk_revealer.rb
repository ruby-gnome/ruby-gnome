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

class TestGtkRevealer < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 10, 0)
    @revealer = Gtk::Revealer.new
  end

  class RevealChild < self
    def setup
      super
      @revealer.reveal_child = true
    end

    def test_reveal_child
      assert_true(@revealer.reveal_child?)
    end

    def test_child_revealed
      assert_true(@revealer.child_revealed?)
    end
  end

  def test_transition_duration_accessors
    duration = 500
    @revealer.transition_duration = duration
    assert_equal(duration, @revealer.transition_duration)
  end

  def test_transition_type_accessors
    revealer_transition_type = Gtk::Revealer::TransitionType::SLIDE_UP
    @revealer.transition_type = revealer_transition_type
    assert_equal(revealer_transition_type, @revealer.transition_type)
  end

  class TestEnum < self
    def test_transition_type
      assert_const_defined(Gtk::Revealer::TransitionType, :CROSSFADE)
    end
  end
end

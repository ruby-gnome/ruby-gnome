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
 
class TestSignal < Test::Unit::TestCase
  def test_signal_flags
    assert_const_defined(GLib, :SignalFlags)
    assert_kind_of(GLib::SignalFlags, GLib::Signal::RUN_FIRST)
    assert_equal(GLib::SignalFlags::MASK, GLib::Signal::FLAGS_MASK)
  end

  def test_connect_flags
    assert_const_defined(GLib, :ConnectFlags)
    assert_kind_of(GLib::ConnectFlags, GLib::Signal::CONNECT_AFTER)
  end

  def test_signal_match_type
    assert_const_defined(GLib, :SignalMatchType)
    assert_kind_of(GLib::SignalMatchType, GLib::Signal::MATCH_ID)
    assert_equal(GLib::SignalMatchType::MASK, GLib::Signal::MATCH_MASK)
  end
end

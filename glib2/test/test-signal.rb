# Copyright (C) 2015-2018  Ruby-GNOME2 Project Team
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

  class CustomSignalObject < GLib::Object
    type_register

    define_signal(:changed, :run_first, nil, nil)

    attr_reader :n_changed
    def initialize
      super
      @n_changed = 0
    end

    def signal_do_changed
      @n_changed += 1
    end
  end

  def test_gc
    n = 10
    custom_signal_object = CustomSignalObject.new
    n.times do
      custom_signal_object.signal_emit(:changed)
      GC.start
    end
    assert_equal(n, custom_signal_object.n_changed)
  end

  def test_inspect
    signal = CustomSignalObject.signal(:changed)
    assert_equal("\#<GLib::Signal TestSignal::CustomSignalObject\#changed>",
                 signal.inspect)
  end
end

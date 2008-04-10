# -*- coding: utf-8 -*-

class TestSignal < Test::Unit::TestCase
  def test_signal_flags
    assert_operator(GLib, :const_defined?, :SignalFlags)
    assert_kind_of(GLib::SignalFlags, GLib::Signal::RUN_FIRST)
    assert_equal(GLib::SignalFlags::MASK, GLib::Signal::FLAGS_MASK)
  end

  def test_connect_flags
    assert_operator(GLib, :const_defined?, :ConnectFlags)
    assert_kind_of(GLib::ConnectFlags, GLib::Signal::CONNECT_AFTER)
  end

  def test_signal_match_type
    assert_operator(GLib, :const_defined?, :SignalMatchType)
    assert_kind_of(GLib::SignalMatchType, GLib::Signal::MATCH_ID)
    assert_equal(GLib::SignalMatchType::MASK, GLib::Signal::MATCH_MASK)
  end
end

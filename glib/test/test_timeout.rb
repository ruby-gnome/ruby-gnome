# -*- coding: utf-8 -*-

class TestGLibTimeout < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    @id = nil
  end

  def teardown
    GLib::Source.remove(@id) if @id
  end

  def test_timeout_add_priority
    priority = GLib::PRIORITY_HIGH
    @id = GLib::Timeout.add(10, priority)
    source = GLib::MainContext.default.find_source(@id)
    assert_equal(priority, source.priority)
  end

  def test_timeout_add_seconds_priority
    only_glib_version(2, 14, 0)
    priority = GLib::PRIORITY_HIGH
    @id = GLib::Timeout.add_seconds(10, priority)
    source = GLib::MainContext.default.find_source(@id)
    assert_equal(priority, source.priority)
  end
end

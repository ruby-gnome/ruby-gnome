# -*- coding: utf-8 -*-

class TestFileMonitor < Test::Unit::TestCase
  class Flags < self
    def test_file_monitor_flags
      assert_nothing_raised do
        Gio::FileMonitorFlags.new(Gio::FileMonitorFlags::SEND_MOVED)
      end
    end
  end

  class Event < self
    def test_file_monitor_event
      assert_nothing_raised do
        Gio::FileMonitorEvent.new(Gio::FileMonitorEvent::CHANGED)
      end
    end
  end
end

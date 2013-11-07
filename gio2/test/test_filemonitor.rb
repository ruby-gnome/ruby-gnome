# -*- coding: utf-8 -*-

class TestFileMonitor < Test::Unit::TestCase
  class Flags < self
    def test_file_monitor_flags
      assert_nothing_raised do
        Gio::FileMonitor::Flags.new(Gio::FileMonitor::Flags::SEND_MOVED)
      end
    end

    def test_file_monitor_event
      assert_nothing_raised do
        Gio::FileMonitor::Event.new(Gio::FileMonitor::Event::CREATED)
      end
    end
  end
end

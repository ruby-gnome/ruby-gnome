# Copyright (C) 2016-2021  Ruby-GNOME Project Team
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

class TestDateTime < Test::Unit::TestCase
  include GLibTestUtils

  def test_now_local
    now = GLib::DateTime.now(:local)
    format = "%Y-%m-%d-%H-%M"
    assert_equal(Time.now.strftime(format), now.format(format))
  end

  sub_test_case "new" do

    test "unix: :local" do
      time = Time.now
      format = "%Y-%m-%d-%H-%M"
      datetime = GLib::DateTime.new(:unix => time.to_i,
                                    :timezone => :local)
      assert_equal(time.strftime(format), datetime.format(format))
    end

    test "unix: :utc" do
      time = Time.now.utc
      format = "%Y-%m-%d-%H-%M"
      datetime = GLib::DateTime.new(:unix => time.to_i,
                                    :timezone => :utc)
      assert_equal(time.strftime(format), datetime.format(format))
    end

    test "timezone: :local" do
      time = Time.now
      datetime = GLib::DateTime.new(:timezone => :local,
                                    :year => time.year,
                                    :month => time.month,
                                    :day => time.day,
                                    :hour => time.hour,
                                    :minute => time.min,
                                    :second => time.sec)
      assert_equal(time.year, datetime.year)
      assert_equal(time.month, datetime.month)
      assert_equal(time.day, datetime.day_of_month)
      assert_equal(time.hour, datetime.hour)
      assert_equal(time.min, datetime.minute)
      assert_equal(time.sec, datetime.second)
    end

    test "timezone: :utc" do
      time = Time.now.utc
      datetime = GLib::DateTime.new(:timezone => :utc,
                                    :year => time.year,
                                    :month => time.month,
                                    :day => time.day,
                                    :hour => time.hour,
                                    :minute => time.min,
                                    :second => time.sec)
      assert_equal(time.year, datetime.year)
      assert_equal(time.month, datetime.month)
      assert_equal(time.day, datetime.day_of_month)
      assert_equal(time.hour, datetime.hour)
      assert_equal(time.min, datetime.minute)
      assert_equal(time.sec, datetime.second)
    end

    test "timezone: local time zone" do
      time = Time.now
      tz = GLib::TimeZone.local
      datetime = GLib::DateTime.new(:timezone => tz,
                                    :year => time.year,
                                    :month => time.month,
                                    :day => time.day,
                                    :hour => time.hour,
                                    :minute => time.min,
                                    :second => time.sec)
      assert_equal(time.year, datetime.year)
      assert_equal(time.month, datetime.month)
      assert_equal(time.day, datetime.day_of_month)
      assert_equal(time.hour, datetime.hour)
      assert_equal(time.min, datetime.minute)
      assert_equal(time.sec, datetime.second)
    end

    test "timezone: UTC time zone" do
      time = Time.now.utc
      tz = GLib::TimeZone.utc
      datetime = GLib::DateTime.new(:timezone => tz,
                                    :year => time.year,
                                    :month => time.month,
                                    :day => time.day,
                                    :hour => time.hour,
                                    :minute => time.min,
                                    :second => time.sec)
      assert_equal(time.year, datetime.year)
      assert_equal(time.month, datetime.month)
      assert_equal(time.day, datetime.day_of_month)
      assert_equal(time.hour, datetime.hour)
      assert_equal(time.min, datetime.minute)
      assert_equal(time.sec, datetime.second)
    end
  end
end

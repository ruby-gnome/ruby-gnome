# Copyright (C) 2022  Ruby-GNOME Project Team
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

class TestResolver < Test::Unit::TestCase
  include GioTestUtils::Fixture
  include GioTestUtils::Omissions

  def test_lookup_records_async
    loop = GLib::MainLoop.new
    timeout_id = GLib::Timeout.add(5000) do
      loop.quit
      GLib::Source::REMOVE
    end
    resolver = Gio::Resolver.default
    records = nil
    resolver.lookup_records_async("clear-code.com", :mx) do |_, result|
      records = resolver.lookup_records_finish(result)
      GLib::Source.remove(timeout_id)
      loop.quit
    end
    loop.run
    assert_equal([[10, "mail.clear-code.com"]],
                 records)
  end
end

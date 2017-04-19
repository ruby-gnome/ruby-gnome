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


class TestResources < Test::Unit::TestCase
  include GioTestUtils::Fixture

  def fixture_path(*components)
    super("resource", *components)
  end

  def setup
    gresource = fixture_path("ruby-gio2.gresource")
    @resource = Gio::Resource.load(gresource)
    Gio::Resources.register(@resource)
  end

  def teardown
    Gio::Resources.unregister(@resource)
  end

  def test_lookup_data
    data = Gio::Resources.lookup_data("/org/ruby-gnome2/test/logo.png")
    assert_equal(File.open(fixture_path("logo.png"), "rb", &:read),
                 data.to_s)
  end

  def test_open_stream
    Gio::Resources.open_stream("/org/ruby-gnome2/test/logo.png") do |input|
      assert_equal(File.open(fixture_path("logo.png"), "rb", &:read),
                   input.read)
    end
  end

  def test_enumerate_children
    assert_equal(["logo.png"],
                 Gio::Resources.enumerate_children("/org/ruby-gnome2/test"))
  end

  def test_get_info
    data = File.open(fixture_path("logo.png"), "rb", &:read)
    assert_equal([true, data.bytesize, 0],
                 Gio::Resources.get_info("/org/ruby-gnome2/test/logo.png"))
  end
end

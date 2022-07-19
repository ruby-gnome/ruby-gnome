# Copyright (C) 2008-2022  Ruby-GNOME Project Team
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

class TestGtkImage < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "empty" do
      image = Gtk::Image.new
      assert_equal(nil, image.paintable)
    end

    test "icon_name" do
      icon_name = "dialog-password"
      image = Gtk::Image.new(icon_name: icon_name)
      assert_equal(icon_name,
                   image.icon_name)
    end

    test "icon" do
      icon = Gio::ThemedIcon.new("document-open")
      image = Gtk::Image.new(icon: icon)
      assert_equal(icon, image.icon)
    end

    test "resource" do
      resource = Gio::Resource.load(fixture_path("image.gresource"))
      Gio::Resources.register(resource)
      begin
        resource_path = "/org/ruby/gnome/gnome-logo-icon.png"
        image = Gtk::Image.new(resource: resource_path)
        assert_equal(resource_path,
                     image.resource)
      ensure
        Gio::Resources.unregister(resource)
      end
    end
  end
end

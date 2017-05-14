# Copyright (C) 2013-2017  Ruby-GNOME2 Project Team
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

class TestGtkCssProvider < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @provider = Gtk::CssProvider.new
  end

  sub_test_case "#load" do
    test ":data" do
      assert do
        @provider.load(:data => "GtkWindow {background-color: red;}")
      end
      assert_equal(<<-CSS, @provider.to_s)
GtkWindow {
  background-color: rgb(255,0,0);
}
      CSS
    end

    test ":file" do
      file = Tempfile.new(["ruby-gtk3", ".css"])
      file.puts("GtkWindow {background-color: red;}")
      file.close
      assert do
        @provider.load(:file => Gio::File.open(path: file.path))
      end
      assert_equal(<<-CSS, @provider.to_s)
GtkWindow {
  background-color: rgb(255,0,0);
}
      CSS
    end

    test ":path" do
      file = Tempfile.new(["ruby-gtk3", ".css"])
      file.puts("GtkWindow {background-color: red;}")
      file.close
      assert do
        @provider.load(:path => file.path)
      end
      assert_equal(<<-CSS, @provider.to_s)
GtkWindow {
  background-color: rgb(255,0,0);
}
      CSS
    end

    test ":resource_path" do
      only_gtk_version(3, 16, 0)

      css = Tempfile.new(["ruby-gtk3", ".css"])
      css.puts("GtkWindow {background-color: red;}")
      css.close
      resource_xml = Tempfile.new(["ruby-gtk3", ".gresource.xml"])
      resource_xml.puts(<<-RESOURCE_XML)
<?xml version="1.0" encoding="UTF-8"?>
<gresources>
  <gresource prefix="/style">
    <file alias="style.css">#{css.path}</file>
  </gresource>
</gresources>
      RESOURCE_XML
      resource_xml.close
      resource_path = resource_xml.path.gsub(/\.xml\z/, "")
      resource = nil
      begin
        system("glib-compile-resources", resource_xml.path)
        resource = Gio::Resource.load(resource_path)
        Gio::Resources.register(resource)
        assert do
          @provider.load(:resource_path => "/style/style.css")
        end
      ensure
        Gio::Resources.unregister(resource) if resource
        FileUtils.rm_f(resource_path)
      end

      assert_equal(<<-CSS, @provider.to_s)
GtkWindow {
  background-color: rgb(255,0,0);
}
      CSS
    end
  end
end

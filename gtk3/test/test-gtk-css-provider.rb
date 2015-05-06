# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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
        @provider.load(:file => Gio::File.path(file.path))
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
  end
end

# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

class TestWebKit2GtkWebContext < Test::Unit::TestCase
  include WebKit2GtkTestUtils

  sub_test_case(".new") do
    sub_test_case("Hash form") do
      setup do
        only_webkit2_gtk_version(2, 16)
      end

      test "ephemeral: true" do
        context = WebKit2Gtk::WebContext.new(ephemeral: true)
        assert do
          context.ephemeral?
        end
      end

      test "ephemeral: false" do
        context = WebKit2Gtk::WebContext.new(ephemeral: false)
        assert do
          not context.ephemeral?
        end
      end
    end
  end
end

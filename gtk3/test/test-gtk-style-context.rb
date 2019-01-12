# Copyright (C) 2013-2019  Ruby-GNOME2 Project Team
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

class TestGtkStyleContext < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @style_context = Gtk::StyleContext.new
  end

  sub_test_case("#add_provider") do
    def setup
      super
      @provider = Gtk::CssProvider.new
    end

    test("provider, Integer") do
      @style_context.add_provider(@provider,
                                  Gtk::StyleProvider::PRIORITY_APPLICATION)
    end

    test("provider, Symbol") do
      @style_context.add_provider(@provider, :application)
    end

    test("provider") do
      @style_context.add_provider(@provider)
    end
  end

  test("#remove_provider") do
    provider = Gtk::CssProvider.new
    @style_context.add_provider(provider,
                                Gtk::StyleProvider::PRIORITY_APPLICATION)
    @style_context.remove_provider(provider)
  end
end

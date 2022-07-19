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

class TestGtkIconTheme < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @widget = Gtk::Label.new("label")
    @theme = @widget.display.icon_theme
  end

  sub_test_case("#has_icon?") do
    def test_true
      assert do
        @theme.has_icon?("face-cool-symbolic")
      end
    end

    def test_false
      assert do
        not @theme.has_icon?("nonexistent")
      end
    end
  end

  def test_lookup_icon
    icon = @theme.lookup_icon("face-cool-symbolic", 48)
    assert_match(/face-cool-symbolic/, icon.icon_name)
  end

  def test_icon_names
    assert do
      @theme.icon_names.include?("face-cool-symbolic")
    end
  end

  sub_test_case "#lookup_icon" do
    sub_test_case "icon" do
      test "String" do
        assert_equal("edit-find",
                     @theme.lookup_icon("edit-find", 16).icon_name)
      end

      test "Symbol" do
        assert_equal("edit-find",
                     @theme.lookup_icon(:"edit-find", 16).icon_name)
      end

      test "Gio::Icon" do
        icon = Gio::ThemedIcon.new("edit-find")
        assert_equal("edit-find",
                     @theme.lookup_icon(icon, 16).icon_name)
      end
    end
  end
end

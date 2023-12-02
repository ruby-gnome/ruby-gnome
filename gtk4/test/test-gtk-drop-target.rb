# Copyright (C) 2023  Ruby-GNOME Project Team
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

class TestGtkDropTarget < Test::Unit::TestCase
  sub_test_case("#initialize") do
    def test_type
      target = Gtk::DropTarget.new(GLib::Type::STRING, Gdk::DragAction::COPY)
      assert_equal([GLib::Type::STRING],
                   target.gtypes)
    end

    def test_types
      target = Gtk::DropTarget.new([GLib::Type::STRING, GLib::Type::INT],
                                   Gdk::DragAction::COPY)
      assert_equal([GLib::Type::STRING, GLib::Type::INT],
                   target.gtypes)
    end
  end
end

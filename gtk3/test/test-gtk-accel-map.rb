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

class TestGtkAccelMap < Test::Unit::TestCase
  class << self
    def startup
      Gtk::AccelMap.add_entry("<category1>/a",
                              Gdk::Keyval::KEY_a,
                              0)
      Gtk::AccelMap.add_entry("<category2>/b",
                              Gdk::Keyval::KEY_b,
                              0)
      Gtk::AccelMap.add_filter("*category1*")
    end
  end

  sub_test_case("#each") do
    def test_default
      values = []
      Gtk::AccelMap.each do |*args|
        values << args
      end
      assert_equal([
                     [
                       "<category2>/b",
                       Gdk::Keyval::KEY_b,
                       Gdk::ModifierType.new(0),
                       false,
                     ],
                   ],
                   values)
    end

    test("filter: true") do
      values = []
      Gtk::AccelMap.each(filter: true) do |*args|
        values << args
      end
      assert_equal([
                     [
                       "<category2>/b",
                       Gdk::Keyval::KEY_b,
                       Gdk::ModifierType.new(0),
                       false,
                     ],
                   ],
                   values)
    end

    test("filter: false") do
      values = []
      Gtk::AccelMap.each(filter: false) do |*args|
        values << args
      end
      assert_equal([
                     [
                       "<category2>/b",
                       Gdk::Keyval::KEY_b,
                       Gdk::ModifierType.new(0),
                       false,
                     ],
                     [
                       "<category1>/a",
                       Gdk::Keyval::KEY_a,
                       Gdk::ModifierType.new(0),
                       false,
                     ],
                   ],
                   values)
    end

    def test_enumerator
      assert_equal([
                     [
                       "<category2>/b",
                       Gdk::Keyval::KEY_b,
                       Gdk::ModifierType.new(0),
                       false,
                     ],
                     [
                       "<category1>/a",
                       Gdk::Keyval::KEY_a,
                       Gdk::ModifierType.new(0),
                       false,
                     ],
                   ],
                   Gtk::AccelMap.each(filter: false).to_a)
    end
  end
end

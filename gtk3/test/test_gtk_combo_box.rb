# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestGtkComboBox < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do 
    test "no argument" do
      combobox = Gtk::ComboBox.new()
      assert_equal(false, combobox.has_entry?)
      assert_nil(combobox.model)
    end    
    
    test ":entry" do
      entry = true 
      combobox = Gtk::ComboBox.new(:entry => entry)
      assert_equal(entry, combobox.has_entry?)
    end

    test ":model" do
      model = Gtk::ListStore.new(Gdk::Pixbuf, String)
      combobox = Gtk::ComboBox.new(:model => model)
      assert_equal(model, combobox.model)
    end
    
    test ":entry and :model" do
      entry = true 
      model = Gtk::ListStore.new(Gdk::Pixbuf, String)
      combobox = Gtk::ComboBox.new(:model => model, :entry => entry)
      assert_equal(entry, combobox.has_entry?)
      assert_equal(model, combobox.model)
    end 
    
    test ":area" do
      area = Gtk::CellAreaBox.new
      combobox = Gtk::ComboBox.new(:area => area)
      assert_equal(area, combobox.area)
    end

    test ":entry and :area" do
      entry = true
      area = Gtk::CellAreaBox.new
      combobox = Gtk::ComboBox.new(:area => area, :entry => entry)
      assert_equal(area, combobox.area)
      assert_equal(entry, combobox.has_entry?)
    end
  end 
end

# -*- coding: utf-8 -*-
#
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

class TestGtkTreeSelection < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @tree = Gtk::TreeView.new
    @selection =  @tree.selection
  end

  def test_mode_accessors
    @selection.mode = Gtk::SelectionMode::MULTIPLE
    assert_equal(Gtk::SelectionMode::MULTIPLE, @selection.mode)
  end
end

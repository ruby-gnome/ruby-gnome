# -*- coding: utf-8 -*-
#
# Copyright (C) 2011-2014  Ruby-GNOME2 Project Team
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

class TestSourceView < Test::Unit::TestCase
  def setup
    @view = GtkSource::View.new
  end

  def test_auto_indent
    @view.auto_indent = true
    assert_true(@view.auto_indent?)
  end

  def test_line_number
    @view.show_line_numbers = true
    assert_true(@view.show_line_numbers?)
  end

  def test_indent_width
    indent = 4
    @view.indent_width = indent
    assert_equal(indent, @view.indent_width)
  end

  def test_insert_spaces_instead_of_tabs
    insert_spaces = true
    @view.insert_spaces_instead_of_tabs = insert_spaces
    assert_true(@view.insert_spaces_instead_of_tabs?)
  end

  def test_right_margin_position
    margin = 100
    @view.right_margin_position = margin
    assert_equal(margin, @view.right_margin_position)
  end

  def test_show_right_margin
    show = true
    @view.show_right_margin = show
    assert_true(@view.show_right_margin?)
  end
end

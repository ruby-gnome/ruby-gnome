# -*- coding: utf-8 -*-
#
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

class TestGLibValue < Test::Unit::TestCase
  include GLibTestUtils

  def test_type
    value = GLib::Value.new(GLib::Type::UINT, 29)
    assert_equal(GLib::Type::UINT, value.type)
  end

  def test_value
    value = GLib::Value.new(GLib::Type::UINT, 29)
    assert_equal(29, value.value)
  end

  def test_no_value
    value = GLib::Value.new(GLib::Type::UINT)
    assert_equal(0, value.value)
  end
end

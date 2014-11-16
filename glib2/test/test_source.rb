# -*- coding: utf-8 -*-
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

class TestGLibSource < Test::Unit::TestCase
  include GLibTestUtils

  def test_time
    context = GLib::MainContext.default
    source = GLib::Idle.source_new
    source.attach(context)
    time = source.time
    assert_operator(0, :<, time)
  end

  def test_name
    only_glib_version(2, 26, 0)
    context = GLib::MainContext.default
    source = GLib::Idle.source_new
    source.attach(context)
    assert_nil(source.name)

    source_name = "glib source"
    source.name = source_name
    assert_equal(source_name, source.name)
  end
end

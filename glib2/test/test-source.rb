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

class TestGLibSource < Test::Unit::TestCase
  include GLibTestUtils

  def test_time
    context = GLib::MainContext.default
    source = GLib::Idle.source_new
    id = source.attach(context)
    begin
      time = source.time
      assert_operator(0, :<, time)
    ensure
      GLib::Source.remove(id)
    end
  end

  def test_destroy
    # only_glib_version(2, 35, 4)
    # GMainContext may be freed before GSource by Ruby's GC.
    # If GMainContext is freed before GSource, GLib 2.35.3 or earlier is
    # crashed.
    #
    # See also:
    #   * https://bugzilla.gnome.org/show_bug.cgi?id=661767
    #   * https://git.gnome.org/browse/glib/commit/?id=26056558be4656ee6e891a4fae5d4198de7519cf

    context = GLib::MainContext.new
    source = GLib::Idle.source_new
    id = source.attach(context)
    assert_not_nil(context.find_source(id))
    source.destroy
    assert_nil(context.find_source(id))
    source = nil
    GC.start
  end

  def test_name
    only_glib_version(2, 26, 0)

    source = GLib::Idle.source_new

    source_name = "glib source"
    source.name = source_name
    assert_equal(source_name, source.name)
  end

  def test_ready_time
    only_glib_version(2, 36, 0)

    source = GLib::Idle.source_new

    ready_time = 5
    source.ready_time = 5
    assert_equal(ready_time, source.ready_time)
  end
end

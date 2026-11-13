# Copyright (C) 2012-2026  Ruby-GNOME Project Team
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

class TestSignalInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("Gio")
    @interface_info = @repository.find("Gio", "ListModel")
    @info = @interface_info.get_signal("items-changed")
  end

  def test_name
    assert_equal("items-changed", @info.name)
  end

  def test_n_args
    assert_equal(3, @info.n_args)
  end

  def test_flags
    assert_equal(GLib::SignalFlags::RUN_LAST, @info.flags)
  end

  def test_class_closure
    assert_nil(@info.class_closure)
  end

  def test_true_stops_emit?
    assert do
      not @info.true_stops_emit?
    end
  end
end

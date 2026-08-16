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

class TestTypeInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("GObject")
    @function_info = @repository.find("GObject", "signal_list_ids")
    @info = @function_info.return_type
  end

  def test_pointer?
    assert_true(@info.pointer?)
  end

  def test_tag
    assert_kind_of(GLib::TypeTag,
                   @info.tag)
  end

  def test_param_type
    assert_kind_of(GLib::TypeInfo,
                   @info.get_param_type(0))
  end

  def test_interface
    function_info = @repository.find("Gio", "app_info_create_from_commandline")
    info = function_info.return_type
    assert_kind_of(GLib::InterfaceInfo,
                   info.interface)
  end

  def test_array_length_index
    assert_equal(1, @info.array_length_index)
  end

  def test_array_fixed_size
    assert_nil(@info.array_fixed_size)
  end

  def test_zero_terminated?
    assert_false(@info.zero_terminated?)
  end

  def test_array_type
    assert_equal(GLib::ArrayType::C,
                 @info.array_type)
  end
end

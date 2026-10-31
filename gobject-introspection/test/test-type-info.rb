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
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @function_info = @repository.find("GObject", "signal_list_ids")
    @info = @function_info.return_type
  end

  def test_pointer?
    assert_true(@info.pointer?)
  end

  def test_tag
    assert_kind_of(GObjectIntrospection::TypeTag,
                   @info.tag)
  end

  def test_param_type
    assert_kind_of(GObjectIntrospection::TypeInfo,
                   @info.get_param_type(0))
  end

  def test_interface
    function_info = @repository.find("Gio", "app_info_create_from_commandline")
    info = function_info.return_type
    assert_kind_of(GObjectIntrospection::InterfaceInfo,
                   info.interface)
  end

  def test_array_length
    assert_equal(1, @info.array_length)
  end

  def test_array_fixed_size
    assert_equal(-1, @info.array_fixed_size)
  end

  def test_zero_terminated?
    assert_false(@info.zero_terminated?)
  end

  def test_array_type
    assert_equal(GObjectIntrospection::ArrayType::C,
                 @info.array_type)
  end

  sub_test_case("#match?") do
    def test_uint8_array_string
      info = @repository.find("Gio", "content_type_guess").get_arg(1).type
      assert do
        info.match?("data")
      end
    end

    def test_uint8_array_bytes
      info = @repository.find("Gio", "content_type_guess").get_arg(1).type
      assert do
        not info.match?(GLib::Bytes.new("data"))
      end
    end

    def test_utf8_string
      function_info = @repository.find("Gio", "content_type_get_description")
      info = function_info.get_arg(0).type
      assert do
        info.match?("text/plain")
      end
    end

    def test_utf8_symbol
      function_info = @repository.find("Gio", "content_type_get_description")
      info = function_info.get_arg(0).type
      assert do
        not info.match?(:text)
      end
    end

    def test_interface_enum
      info = @repository.find("Gio", "bus_get_sync").get_arg(0).type
      assert do
        info.match?(Gio::BusType::SESSION)
      end
    end

    def test_interface_enum_symbol
      info = @repository.find("Gio", "bus_get_sync").get_arg(0).type
      assert do
        not info.match?(:session)
      end
    end
  end
end

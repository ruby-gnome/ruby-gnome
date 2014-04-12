# Copyright (C) 2012  Ruby-GNOME2 Project Team
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

class TestObjectInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("Gio")
    @info = @repository.find("Gio", "FileOutputStream")
  end

  def test_type_name
    assert_equal("GFileOutputStream", @info.type_name)
  end

  def test_type_init
    assert_equal("g_file_output_stream_get_type", @info.type_init)
  end

  def test_abstract?
    assert_false(@info.abstract?)
  end

  def test_fundamental?
    assert_false(@info.fundamental?)
  end

  def test_parent
    assert_equal("OutputStream", @info.parent.name)
  end

  def test_n_interfaces
    assert_equal(1, @info.n_interfaces)
  end

  def test_get_interface
    assert_kind_of(GObjectIntrospection::InterfaceInfo,
                   @info.get_interface(0))
  end

  def test_n_fields
    assert_equal(2, @info.n_fields)
  end

  def test_get_field
    assert_kind_of(GObjectIntrospection::FieldInfo,
                   @info.get_field(0))
  end

  def test_n_properties
    info = @repository.find("Gio", "BufferedOutputStream")
    assert_equal(2, info.n_properties)
  end

  def test_get_property
    info = @repository.find("Gio", "BufferedOutputStream")
    assert_kind_of(GObjectIntrospection::PropertyInfo,
                   info.get_property(0))
  end

  def test_n_methods
    assert_equal(4, @info.n_methods)
  end

  def test_get_method_n
    assert_kind_of(GObjectIntrospection::FunctionInfo,
                   @info.get_method(0))
  end

  def test_get_method_name
    assert_kind_of(GObjectIntrospection::FunctionInfo,
                   @info.get_method("get_etag"))
  end

  def test_n_signals
    info = @repository.find("Gio", "Application")
    assert_operator(5, :<=, info.n_signals)
  end

  def test_get_signal
    info = @repository.find("Gio", "Application")
    assert_kind_of(GObjectIntrospection::SignalInfo,
                   info.get_signal(0))
  end

  def test_n_vfuncs
    assert_equal(9, @info.n_vfuncs)
  end

  def test_get_vfunc_n
    assert_kind_of(GObjectIntrospection::VFuncInfo,
                   @info.get_vfunc(0))
  end

  def test_get_vfunc_name
    assert_kind_of(GObjectIntrospection::VFuncInfo,
                   @info.get_vfunc("can_seek"))
  end

  def test_n_constants
    assert_equal(0, @info.n_constants)
  end

  def test_unref_function
    assert_nil(@info.unref_function)
  end

  def test_ref_function
    assert_nil(@info.ref_function)
  end

  def test_set_value_function
    assert_nil(@info.set_value_function)
  end

  def test_get_value_function
    assert_nil(@info.get_value_function)
  end
end

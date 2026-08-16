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

class TestInterfaceInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("Gio")
    @info = @repository.find("Gio", "TlsServerConnection")
  end

  def test_n_prerequisites
    assert_equal(1, @info.n_prerequisites)
  end

  def test_prerequisite
    assert_kind_of(GLib::ObjectInfo,
                   @info.get_prerequisite(0))
  end

  def test_n_properties
    assert_equal(1, @info.n_properties)
  end

  def test_property
    assert_kind_of(GLib::PropertyInfo,
                   @info.get_property(0))
  end

  def test_n_methods
    assert_equal(1, @info.n_methods)
  end

  def test_get_method_n
    assert_kind_of(GLib::FunctionInfo,
                   @info.get_method(0))
  end

  def test_get_method_name
    assert_kind_of(GLib::FunctionInfo,
                   @info.get_method("new"))
  end

  def test_n_signals
    info = @repository.find("Gio", "Volume")
    assert_equal(2, info.n_signals)
  end

  def test_get_signal_n
    info = @repository.find("Gio", "Volume")
    assert_kind_of(GLib::SignalInfo,
                   info.get_signal(0))
  end

  def test_get_signal_name
    info = @repository.find("Gio", "Volume")
    assert_kind_of(GLib::SignalInfo,
                   info.get_signal("changed"))
  end

  def test_n_vfuncs
    info = @repository.find("Gio", "Volume")
    assert do
      info.n_vfuncs > 0
    end
  end

  def test_get_vfunc_n
    info = @repository.find("Gio", "Volume")
    assert_kind_of(GLib::VFuncInfo,
                   info.get_vfunc(0))
  end

  def test_get_vfunc_name
    info = @repository.find("Gio", "Volume")
    assert_kind_of(GLib::VFuncInfo,
                   info.get_vfunc("can_eject"))
  end

  def test_n_constants
    omit("Find an interface info that has constants")
    info = @repository.find("Gio", "XXX")
    assert do
      info.n_constants > 0
    end
  end

  def test_get_constant
    omit("Find an interface info that has constants")
    info = @repository.find("Gio", "XXX")
    assert_kind_of(GLib::ConstantInfo,
                   info.get_constant(0))
  end

  def test_iface_struct
    assert_kind_of(GLib::StructInfo,
                   @info.iface_struct)
  end
end

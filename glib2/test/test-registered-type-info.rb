# Copyright (C) 2026  Ruby-GNOME Project Team
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

class TestRegisteredTypeInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("Gio")
    @info = @repository.find("Gio", "TlsServerConnection")
  end

  def test_ancestors
    assert do
      GLib::InterfaceInfo < GLib::RegisteredTypeInfo
    end
  end

  def test_type_name
    assert_equal("GTlsServerConnection", @info.type_name)
  end

  def test_type_init_function_name
    assert_equal("g_tls_server_connection_get_type",
                 @info.type_init_function_name)
  end

  def test_gtype
    assert_equal("GTlsServerConnection", @info.gtype.name)
  end

  def test_boxed_false
    assert do
      not @info.boxed?
    end
  end

  def test_boxed_true
    info = @repository.find("GLib", "Bytes")
    assert do
      info.boxed?
    end
  end

  def test_enum_info
    info = @repository.find("Gio", "FileAttributeType")
    assert_equal("GFileAttributeType", info.type_name)
  end

  def test_flags_info
    info = @repository.find("Gio", "FileCopyFlags")
    assert_equal("GFileCopyFlags", info.type_name)
  end

  def test_union_info
    info = @repository.find("GLib", "Mutex")
    assert_nil(info.type_name)
  end

  def test_match
    info = @repository.find("GLib", "Bytes")
    assert do
      info.match?(GLib::Bytes.new("data"))
    end
  end

  def test_match_cast
    info = @repository.find("GLib", "Bytes")
    assert do
      not info.match?("data")
    end
  end

  def test_match_unresolved
    info = @repository.find("GLib", "Cond")
    assert do
      not info.match?(Object.new)
    end
  end
end

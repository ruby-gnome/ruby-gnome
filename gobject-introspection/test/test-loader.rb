# Copyright (C) 2012-2021  Ruby-GNOME Project Team
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

class TestLoaderInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
  end

  def test_define_class
    info = @repository.find("Gio", "Application")
    gtype = info.gtype
    assert_equal(gtype, Gio::Application.gtype)
  end

  sub_test_case("virtual function") do
    def test_without_prefix
      active_vfs_class = Class.new(Gio::Vfs) do
        type_register("ActiveVfsWithoutPrefix")

        def virtual_do_is_active
          true
        end
      end
      active_vfs = active_vfs_class.new
      assert do
        active_vfs.active?
      end
    end

    def test_with_prefix
      active_vfs_class = Class.new(Gio::Vfs) do
        type_register("ActiveVfsWithPrefix")

        def virtual_do_gvfs_is_active
          true
        end
      end
      active_vfs = active_vfs_class.new
      assert do
        active_vfs.active?
      end
    end

    def test_interface
      resettable_converter_class = Class.new(GLib::Object) do
        type_register("ResettableConverter")

        include Gio::Converter

        attr_reader :resetted

        def virtual_do_reset
          @resetted = true
        end
      end
      resettable_converter = resettable_converter_class.new
      resettable_converter.reset
      assert do
        resettable_converter.resetted
      end
    end
  end
end

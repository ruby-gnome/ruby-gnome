# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

class TestTypeInterface < Test::Unit::TestCase
  sub_test_case("#append_features") do
    def test_not_instantiatable
      plain_class = Class.new
      assert_raise(TypeError.new("Not a subclass of GLib::Instantiatable")) do
        plain_class.class_eval do
          include GLib::TypePlugin
        end
      end
    end

    def test_instantiatable
      gobject_class = Class.new(GLib::Object)
      gobject_class.class_eval do
        include GLib::TypePlugin
      end
      assert do
        gobject_class.method_defined?(:use)
      end
    end
  end
end

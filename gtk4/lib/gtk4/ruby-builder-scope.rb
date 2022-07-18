# Copyright (C) 2022  Ruby-GNOME Project Team
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

module Gtk
  class RubyBuilderScope < GLib::Object
    type_register

    include Gtk::BuilderScope

    def virtual_do_get_type_from_name(builder, type_name)
      GLib::Type.try_convert(type_name)
    end

    def virtual_do_get_type_from_function(builder, function_name)
      builder.__send__(normalize_name(function_name))
    end

    def virtual_do_create_closure(builder,
                                  function_name,
                                  flags,
                                  object)
      object ||= builder.current_object || builder
      method_name = normalize_name(function_name)
      GLib::Closure.new do |*args|
        object.__send__(method_name, *args)
      end
    end

    private
    def normalize_name(name)
      name.gsub(/[-\s]+/, "_")
    end
  end
end

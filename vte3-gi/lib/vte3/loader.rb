# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

module Vte
  class Loader < GObjectIntrospection::Loader
    private
    def terminal_class
      @terminal_class ||= @base_module.const_get(:Terminal)
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init"
        # ignore
      else
        super
      end
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def define_enum(info)
      case info.name
      when /\ATerminal/
        self.class.define_class(info.gtype, $POSTMATCH, terminal_class)
      else
        super
      end
    end

    def require_libraries
      require "vte3/deprecated"
    end

    def rubyish_method_name(info)
      name = super
      case name
      when "fork_command_full"
        "fork_command"
      else
        name
      end
    end
  end
end

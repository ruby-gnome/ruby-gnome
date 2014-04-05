# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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

module Gdk
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      @pending_constants = []
    end

    def post_load(repository, namespace)
      apply_constants
      require_libraries
    end

    def apply_constants
      @pending_constants.each do |info|
        case info.name
        when /\AEVENT_/
          Gdk::Event.const_set($POSTMATCH, info.value)
        end
      end
    end

    def require_libraries
      require "gdk3/color"
      require "gdk3/event"
      require "gdk3/rgba"
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

    def load_struct_info(info)
      return if info.gtype_struct?

      options = {}
      case info.name
      when /\AEvent/
        options[:parent] = Gdk::Event
      end

      define_struct(info, options)
    end

    def load_constant_info(info)
      case info.name
      when /\AEVENT_/
        @pending_constants << info
      else
        super
      end
    end
  end
end

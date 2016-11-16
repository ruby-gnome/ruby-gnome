# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

module Gio
  class SettingsSchemaSource
    if method_defined?(:list_schemas)
      alias_method :list_schemas_raw, :list_schemas
      def list_schemas(recursive_or_options=nil)
        case recursive_or_options
        when true, false
          recursive = recursive_or_options
        else
          options = recursive_or_options || {}
          recursive = options[:recursive]
          recursive = true if recursive.nil?
        end

        list_schemas_raw(recursive)
      end
    end
  end
end

# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

module Gst
  class Registry
    alias_method :plugins, :plugin_list
    def get_features(name_or_type)
      if name_or_type.is_a?(GLib::Type)
        type = name_or_type
        get_feature_list(type)
      else
        name = name_or_type
        get_feature_list_by_plugin(name)
      end
    end

    alias_method :find_feature_raw, :find_feature
    def find_feature(name, type_or_class)
      if type_or_class.is_a?(GLib::Type)
        type = type_or_class
      else
        type = type_or_class.gtype
      end
      find_feature_raw(name, type)
    end
  end
end

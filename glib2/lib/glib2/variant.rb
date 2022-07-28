# Copyright (C) 2015-2022  Ruby-GNOME2 Project Team
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

module GLib
  class Variant
    class << self
      alias_method :parse_raw, :parse

      def parse(string, type = nil)
        parse_raw(string, type)
      end
    end

    alias_method :variant_print_raw, :variant_print

    def variant_print(type_annotate = false)
      variant_print_raw(type_annotate)
    end
  end
end

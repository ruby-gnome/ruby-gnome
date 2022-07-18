# Copyright (C) 2014-2022  Ruby-GNOME Project Team
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
  class Button
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      label = options[:label]
      use_underline = options[:use_underline]
      if use_underline.nil?
        mnemonic = options[:mnemonic]
        if mnemonic
          label = mnemonic
          use_underline = true
        end
      end
      icon_name = options[:icon_name]

      if label
        if use_underline
          initialize_new_with_mnemonic(label)
        else
          initialize_new_with_label(label)
        end
      elsif icon_name
        initialize_new_from_icon_name(icon_name)
      else
        initialize_raw
      end
    end
  end
end

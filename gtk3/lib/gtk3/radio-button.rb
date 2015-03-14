# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
  class RadioButton
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      group         = options[:group]
      label         = options[:label]
      member        = options[:member]
      use_underline = options[:use_underline]

      if label
        if member
          if use_underline
            initialize_new_with_mnemonic_from_widget(member, label)
          else
            initialize_new_with_label_from_widget(member, label)
          end
        else
          if use_underline
            initialize_new_with_mnemonic(group, label)
          else
            initialize_new_with_label(group, label)
          end
        end
      else
        if member
          initialize_new_from_widget(member)
        else
          initialize_raw
        end
      end
    end
  end
end

# Copyright (C) 2012  Ruby-GNOME2 Project Team
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

module Clutter
  class Actor
    # TODO: use prepend after Ruby 1.9 support is dropped.
    alias_method :save_easing_state_without_block, :save_easing_state
    def save_easing_state
      if block_given?
        save_easing_state_without_block
        begin
          yield
        ensure
          restore_easing_state
        end
      else
        save_easing_state_without_block
      end
    end
  end
end

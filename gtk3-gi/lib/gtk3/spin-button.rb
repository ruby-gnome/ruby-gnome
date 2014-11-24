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

module Gtk
  class SpinButton
    alias_method :initialize_raw, :initialize
    def initialize(arg0, arg1=nil, arg2=nil)
      if arg0.is_a? Gtk::Adjustment
        arg1 = arg1 || 0.0
        arg2 = arg2 || 0
        initialize_raw arg0, arg1, arg2
      else
        initialize_new_with_range arg0, arg1, arg2
      end
    end
  end
end

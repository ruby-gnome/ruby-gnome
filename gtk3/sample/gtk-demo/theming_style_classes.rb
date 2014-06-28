#
# This sample code is a port of gtk3/demos/gtk-demo/theming_style_classes.c.
# The UI file used in this sample code is copied from gtk3/demos/gtk-demo.
# This is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
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
=begin
= CSS Theming/Style Classes

GTK+ uses CSS for theming. Style classes can be associated
with widgets to inform the theme about intended rendering.

This demo shows some common examples where theming features
of GTK+ are used for certain effects: primary toolbars,
inline toolbars and linked buttons.
=end

require "common"

module Demo
  class ThemingStyleClasses < BasicWindow
    def initialize
      super("Style Claases")
      self.border_width = 12

      builder = Gtk::Builder.new
      builder.add("theming.ui")

      grid = builder.get_object("grid")
      add(grid)
    end
  end
end

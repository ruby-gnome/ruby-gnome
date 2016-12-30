#
# This sample code is a port of gtk3/demos/gtk-demo/theming_style_classes.c.
# The UI file used in this sample code is copied from gtk3/demos/gtk-demo.
# This is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# Copyright (C) 2014-2016  Ruby-GNOME2 Project Team
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
= Theming/Style Classes

GTK+ uses CSS for theming. Style classes can be associated
with widgets to inform the theme about intended rendering.

This demo shows some common examples where theming features
of GTK+ are used for certain effects: primary toolbars,
inline toolbars and linked buttons.
=end

class ThemingStyleClassesDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Style Classes"
    @window.resizable = false

    builder = Gtk::Builder.new(:resource => "/theming_style_classes/theming.ui")
    grid = builder["grid"]
    grid.show_all
    @window.add(grid)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end
end

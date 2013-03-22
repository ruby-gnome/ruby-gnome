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

require 'webkit-gtk2'

WebKitGtk2.init

window = Gtk::Window.new
window.signal_connect("destroy") do
  Gtk.main_quit
end


sw = Gtk::ScrolledWindow.new

view = WebKitGtk2::WebView.new
view.load_uri("http://webkitgtk.org/")

sw.add(view)
window.add(sw)
window.show_all

Gtk.main

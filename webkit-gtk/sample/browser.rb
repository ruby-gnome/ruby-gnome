# Copyright (C) 2013-2024  Ruby-GNOME Project Team
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

require "webkit-gtk"

app = Gtk::Application.new("com.github.ruby-gnome.webkit-gtk.Browser",
                           [:non_unique])
app.signal_connect("activate") do
  window = Gtk::ApplicationWindow.new(app)
  window.set_default_size(1200, 800)
  view = WebKitGtk::WebView.new
  view.load_uri("https://webkitgtk.org/")
  window.child = view
  window.present
end
app.run([$0] + ARGV)

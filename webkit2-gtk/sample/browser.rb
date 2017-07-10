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

require "uri"
require "webkit2-gtk"

uri = URI.parse(ARGV.shift || "https://webkitgtk.org/")

window = Gtk::Window.new
window.set_default_size(800, 600)
window.signal_connect("destroy") do
  Gtk.main_quit
end

view = WebKit2Gtk::WebView.new
view.signal_connect("load-changed") do |_, load_event|
  p [:load_changed, view.uri, load_event]
end
view.signal_connect("load-failed") do |_, _, failed_uri, error|
  message = "failed to load URI: #{failed_uri}: "
  message << "#{error.class}(#{error.code}): #{error.message}"
  puts(message)
  true
end

if uri.class == URI::Generic
  view.load_html(File.read(uri.path), uri.to_s)
else
  view.load_uri(uri.to_s)
end

window.add(view)
window.show_all

Gtk.main

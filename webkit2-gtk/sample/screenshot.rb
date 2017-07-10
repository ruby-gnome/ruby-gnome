# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

require "cgi/util"
require "webkit2-gtk"

if ARGV.empty?
  puts("Usage: #{$0} URL...")
  puts(" e.g.: #{$0} https://webkitgtk.org/ https://www.gtk.org/")
  exit(false)
end

main_context = GLib::MainContext.default

view_context = WebKit2Gtk::WebContext.new(ephemeral: true)
view = WebKit2Gtk::WebView.new(context: view_context)
window = Gtk::OffscreenWindow.new
window.add(view)
window.set_default_size(800, 600)
window.show_all
ARGV.each do |uri|
  view.load_uri(uri)
  finished = false
  view.signal_connect("load-changed") do |_, load_event|
    case load_event
    when WebKit2Gtk::LoadEvent::FINISHED
      view.get_snapshot(:full_document, :none) do |_, result|
        finished = true
        snapshot_surface = view.get_snapshot_finish(result)
        base_path = CGI.escape(uri)
        snapshot_surface.write_to_png("#{base_path}.png")

        width = 200
        height = 200
        thumbnail_surface = Cairo::ImageSurface.new(:argb32, width, height)
        context = Cairo::Context.new(thumbnail_surface)
        context.set_source_color(:white)
        context.paint
        ratio = width.to_f / snapshot_surface.width
        context.scale(ratio, ratio)
        context.set_source(snapshot_surface)
        context.paint
        thumbnail_surface.write_to_png("#{base_path}.thumbnail.png")
      end
    end
  end
  view.signal_connect("load-failed") do |_, _, failed_uri, error|
    finished = true
    message = "failed to load URI: #{failed_uri}: "
    message << "#{error.class}(#{error.code}): #{error.message}"
    puts(message)
    true
  end
  until finished
    main_context.iteration(true)
  end
end

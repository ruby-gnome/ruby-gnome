#!/usr/bin/env ruby
#
# Copyright (C) 2019  Ruby-GNOME Project Team
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

require "gst"
require "gtk3"

bin = Gst::Pipeline.new("pipeline")
src = Gst::ElementFactory.make("videotestsrc", nil)
raise "need audiotestsrc from gst-plugins-base" if src.nil?
sink = Gst::ElementFactory.make("gtksink", nil)
raise "need autoaudiosink from gst-plugins-good" if sink.nil?

bin << src << sink
src >> sink

app = Gtk::Application.new(nil, :handles_command_line)
app.signal_connect(:command_line) do
  app.activate
  0
end
app.signal_connect(:activate) do
  window = Gtk::ApplicationWindow.new(app)
  window.set_default_size 200, 200
  window.add(sink.widget)
  bin.state = :playing
  window.show_all
end
app.run

bin.state = :null

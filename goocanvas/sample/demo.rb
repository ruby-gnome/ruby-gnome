#  -*- indent-tabs-mode: nil -*-
=begin header

  demo.rb - Canvas test rewritten in Ruby/GooCanvas

  Rewritten by  Emmanuel Pinault <seatmanu@yahoo.com>

Original Copyright:

  Author :  Richard Hestilow <hestgray@ionet.net>

  Copyright (C) 1998 Free Software Foundation

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

=end

base_dir = File.dirname(__FILE__)
src_dir = File.expand_path(File.join(base_dir, "..", "src"))
lib_dir = File.join(src_dir, "lib")

$LOAD_PATH.unshift(src_dir)
$LOAD_PATH.unshift(lib_dir)
$LOAD_PATH.unshift(base_dir)

require 'goocanvas'
require 'demo-primitives'
require 'demo-arrowhead'
require 'demo-fifteen'

class GooCanvasSample < Gtk::Window
  def initialize
    super(Gtk::Window::TOPLEVEL)

    signal_connect("destroy") do |widget, event|
      Gtk::main_quit()
    end

    signal_connect("key_press_event") do |widget, event|
      if event.state.control_mask? and event.keyval == Gdk::Keyval::GDK_q
        destroy
        true
      else
        false
      end
    end

    set_default_size(640, 600)
    notebook = Gtk::Notebook.new
    add(notebook)
    notebook.show

    notebook.append_page(CanvasSamplePrimitives.new(false),
                         Gtk::Label.new("Primitives"))
    notebook.append_page(CanvasSampleArrowhead.new,
                         Gtk::Label.new("Arrowhead"))
    notebook.append_page(CanvasSampleFifteen.new,
                         Gtk::Label.new("Fifteen"))
#     notebook.append_page(CanvasSampleFeatures.new,
#                          Gtk::Label.new("Feature"))
#     notebook.append_page(CanvasSampleRichText.new,
#                          Gtk::Label.new("Rich Text"))
#     notebook.append_page(CanvasSampleBezierCurve.new,
#                          Gtk::Label.new("Bezier Curve"))
    show
  end
end

canvas = GooCanvasSample.new

Gtk.main

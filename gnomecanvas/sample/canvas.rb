=begin header

  canvas.rb - Canvas test rewritten in Ruby/GNOME

  Rewritten by KUBO Takehiro <kubo@jiubao.org>

Original Copyright:
 
  Author : Richard Hestilow <hestgray@ionet.net>

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

require 'gtk2'
require 'gdk_pixbuf2'
require 'libart2'
require 'gnomecanvas2'

require 'canvas-arrowhead'
require 'canvas-primitives'
require 'canvas-fifteen'
require 'canvas-features'

class CanvasSample < Gtk::Window
  def initialize
    super(Gtk::WINDOW_TOPLEVEL)
    self.signal_connect("delete_event") do |widget, event|
      Gtk::main_quit()
    end

    notebook = Gtk::Notebook.new()
    self.add(notebook)
    notebook.show()

    notebook.append_page(CanvasSamplePrimitives.new(false), Gtk::Label.new("Primitives"))
    notebook.append_page(CanvasSamplePrimitives.new(true), Gtk::Label.new("Antialias"))
    notebook.append_page(CanvasSampleArrowhead.new(), Gtk::Label.new("Arrowhead"))
    notebook.append_page(CanvasSampleFifteen.new(), Gtk::Label.new("Fifteen"))
    notebook.append_page(CanvasSampleFeatures.new(), Gtk::Label.new("Feature"))
    show()
  end
end

canvas = CanvasSample.new()
Gtk::main()


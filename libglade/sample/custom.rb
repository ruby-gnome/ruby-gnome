#!/usr/bin/env ruby
=begin
  custom.rb - Ruby/Libglade2 custom widget sample script.

  (Notice)You need Ruby/GtkHtml2 for this sample.

  Copyright (c) 2004 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: custom.rb,v 1.2 2004/03/05 16:34:24 mutoh Exp $
=end

require 'libglade2'
require 'gtkhtml2'

def create_test
  Gtk::Button.new("Custom1")
end

class Custom
  def Custom.create_test2(str)
    Gtk::Label.new(str)
  end
  
  def initialize(path)
    @glade = GladeXML.new(path) {|handler| method(handler)}

    htmldoc = @glade["custom1"].document
    htmldoc.open_stream('text/html')
    htmldoc.write_stream('<html><body><h1>TEST 2</h1></body></html>')
    htmldoc.close_stream
  end
end

Gtk.init
Custom.new("custom.glade")
Gtk.main


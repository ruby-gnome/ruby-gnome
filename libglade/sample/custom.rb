#!/usr/bin/env ruby
=begin
  custom.rb - Ruby/Libglade2 custom widget sample script.

  Note: You need Ruby/GtkHtml2 for this sample.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: custom.rb,v 1.4 2006/06/17 14:14:55 mutoh Exp $
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

Custom.new("custom.glade")
Gtk.main


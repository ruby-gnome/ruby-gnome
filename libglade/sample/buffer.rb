#!/usr/bin/env ruby
=begin
  buffer.rb - Ruby/Libglade2 sample script.
                                                                                
  Copyright (c) 2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
                                                                                
  $Id: buffer.rb,v 1.2 2006/06/17 14:14:55 mutoh Exp $
=end
                                                                                
require 'libglade2'

class Test
  def initialize
    data = IO.read("test.glade")
    @glade = GladeXML.new(data, nil, nil, GladeXML::BUFFER) {|handler| method(handler)}
    @entry = @glade["entrybox"]
  end
  
  def say_hello(widget)
    @entry.set_text("Hello World")
  end
  
  def quit
    Gtk.main_quit
  end
end

Test.new
Gtk.main

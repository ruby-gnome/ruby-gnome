=begin
  test.rb - Ruby/Libglade2 sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: test.rb,v 1.6 2005/09/19 14:05:21 mutoh Exp $
=end

require 'libglade2'

class Test
  def initialize
    @glade = GladeXML.new("test.glade") {|handler| method(handler)}
    @entry = @glade["entrybox"]
p @glade.get_widget_prefix("foo")
  end
  
  def say_hello(widget)
    @entry.set_text("Hello World")
  end
  
  def quit
    Gtk.main_quit
  end
end

Gtk.init
Test.new
Gtk.main

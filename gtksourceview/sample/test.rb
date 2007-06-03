#!/usr/bin/env ruby
=begin
  test.rb - Ruby/GtkSourceView sample script.

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: test.rb,v 1.4 2007/06/03 02:11:07 mutoh Exp $
=end

require 'gtksourceview'

w = Gtk::Window.new
w.signal_connect("delete-event"){Gtk::main_quit}

view = Gtk::SourceView.new
w.add(Gtk::ScrolledWindow.new.add(view))
view.show_line_numbers = true

lang = Gtk::SourceLanguagesManager.new.get_language('text/x-ruby')
view.buffer.language = lang
view.buffer.highlight = true

w.set_default_size(400,300)
# customize style
lang.tags.each do |tag|
  if tag.id == "Keywords"
    style = tag.style
    style.mask |= Gtk::SourceTagStyle::USE_FOREGROUND
    style.foreground = Gdk::Color.parse("blue")
    lang.set_tag_style(tag.id, style)
  end
end
w.show_all

Gtk.main

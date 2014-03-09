#!/usr/bin/env ruby
=begin
  test.rb - Ruby/GtkSourceView3 sample script.

  Copyright (c) 2006-2014  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: test.rb,v 1.4 2007/06/03 02:11:07 mutoh Exp $
=end

require 'gtksourceview3'

window = Gtk::Window.new
window.signal_connect("delete-event") {Gtk::main_quit}

view = GtkSource::View.new
window.add(Gtk::ScrolledWindow.new.add(view))
view.show_line_numbers = true
view.insert_spaces_instead_of_tabs = true
view.indent_width = 4
view.show_right_margin = true
view.right_margin_position = 80

lang = GtkSource::LanguageManager.new.get_language('ruby')
view.buffer.language = lang
view.buffer.highlight_syntax = true
view.buffer.highlight_matching_brackets = true

window.set_default_size(450,300)
window.show_all

Gtk.main

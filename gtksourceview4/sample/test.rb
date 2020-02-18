#!/usr/bin/env ruby
#
# test.rb - Ruby/GtkSourceView4 sample script.
#
# Copyright (c) 2006-2020 Ruby-GNOME Project Team
# This program is licenced under the same licence as Ruby-GNOME.
#
# $Id: test.rb,v 1.4 2007/06/03 02:11:07 mutoh Exp $

require 'gtksourceview4'

window = Gtk::Window.new
window.signal_connect('delete-event') { Gtk.main_quit }

view = GtkSource::View.new
window.add(Gtk::ScrolledWindow.new.add(view))
view.show_line_numbers = true
view.insert_spaces_instead_of_tabs = true
view.indent_width = 2
view.show_right_margin = true
view.right_margin_position = 80

lang = GtkSource::LanguageManager.new.get_language('ruby')
view.buffer.language = lang
view.buffer.highlight_syntax = true
view.buffer.highlight_matching_brackets = true
view.buffer.text = File.read(__FILE__)
provider = Gtk::CssProvider.new
provider.load(data: 'textview { font-family: Monospace; font-size: 8pt; }')
view.style_context.add_provider(provider)

window.set_default_size(480, 480)
window.show_all

Gtk.main

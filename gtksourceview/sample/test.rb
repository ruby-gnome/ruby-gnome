require 'gtk2'
$:.push(File::join(File.dirname(__FILE__),'/src'))
require 'gtksourceview'


def sw(a) 
	w = Gtk::ScrolledWindow.new
	w.add(a)
	w
end

Gtk.init

w = Gtk::Window.new
w.signal_connect("delete-event"){Gtk::main_quit}
w.add(sw(b = Gtk::SourceView.new))
b.show_line_numbers = 1
#p b.buffer.methods
lang = Gtk::SourceLanguagesManager.new.get_language('text/x-ruby')
b.buffer.language = lang
b.buffer.highlight = true
w.set_default_size(400,300)
#p b.methods
#lang.get_tags.each { |t| p t }
w.show_all

Gtk.main

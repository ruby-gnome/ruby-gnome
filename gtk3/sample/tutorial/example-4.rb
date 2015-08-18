require "gtk3"

builder_file = "#{File.expand_path(File.dirname(__FILE__))}/builder.ui"

# Construct a Gtk::Builder instance and load our UI description
builder = Gtk::Builder.new(:file => builder_file)

# Connect signal handlers to the constructed widgets
window = builder.get_object("window")
window.signal_connect("destroy") { Gtk.main_quit }

button = builder.get_object("button1")
button.signal_connect("clicked") { puts "Hello World" }

button = builder.get_object("button2")
button.signal_connect("clicked") { puts "Hello World" }

button = builder.get_object("quit")
button.signal_connect("clicked") { Gtk.main_quit }

Gtk.main

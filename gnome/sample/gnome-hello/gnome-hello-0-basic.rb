#!/usr/local/bin/ruby

# gnome-hello-basic.rb -- Example for the "Starting with GNOME" section
# of the Gnome Developers' Tutorial (that's is included in the
# Gnome Developers' Documentation in devel-progs/)
#
#Includes:
#   Basic stuff
#
# Copyright (C) 1998 Mark Galassi, Horacio J. Pe, all rights reserved

require 'gtk'
require 'gdk_imlib'
require 'gnome'

class Hello0 < Gnome::App
    def initialize

	# Makes the main window and binds the delete event so you can close
	# the program from your WM

	super("hello", "Hello World Gnomified")
	realize
	signal_connect("delete_event") {
	    Gtk::main_quit
	}

	# We make a button, bind the 'clicked' signal to hello and setting it
	# to be the content of the main window

	button = Gtk::Button.new("Hello GNOME")
	button.signal_connect("clicked") {
	    puts("Hello GNOME")
	    Gtk::main_quit
	}
	button.border_width = 60

	set_contents(button)

	# We now show the widgets, the order doesn't matter, but i suggests 
	# showing the main window last so the whole window will popup at
	# once rather than seeing the window pop up, and then the button form
	# inside of it. Although with such simple example, you'd never notice.

	button.show
	show
    end
end

app = Hello0.new
Gtk::main()

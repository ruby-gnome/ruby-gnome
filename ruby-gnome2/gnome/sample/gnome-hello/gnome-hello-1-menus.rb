#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'
require 'gnome'
require 'gnoconfig'

class Hello1 < Gnome::App
    def initialize

	# Makes the main window and binds the delete event so you can close
	# the program from your WM

	super("hello", "Hello World Gnomified")
	realize
	signal_connect("delete_event") {
	    Gtk::main_quit
	}

	# Now that we've the main window we'll make the menues

	file_menu = [ Gnome::UIInfo::item_stock("Exit",
			nil, proc { Gtk::main_quit },
			Gnome::Stock::MENU_QUIT) ]
	help_menu = [ Gnome::UIInfo::item_stock("About Gnome Hello...",
			nil, proc { about_cb },
			Gnome::Stock::MENU_ABOUT) ]
	main_menu = [ Gnome::UIInfo::subtree("File", file_menu),
		      Gnome::UIInfo::subtree("Help", help_menu) ]
	create_menus(main_menu)

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

    private
    def about_cb
	about = Gnome::About.new(
		    "The Hello World Gnomified",
		    GVERSION,
		    "(C) 1998 the Free Software Foundation",
		    [ "Minoru Inachi (inachi@earth.interq.or.jp)" ],
		    "GNOME is a civilized software system so we've a \"hello world\" program")
	about.show
    end

end

app = Hello1.new
Gtk::main()


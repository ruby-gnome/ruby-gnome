#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'
require 'gnome'
require 'gnoconfig'

class Hello2 < Gnome::App
    include Gnome::I18n

    def initialize

	# Makes the main window and binds the delete event so you can close
	# the program from your WM

	super("hello", _("Hello World Gnomified"))
	realize
	signal_connect("delete_event") {
	    Gtk::main_quit
	}

	# Now that we've the main window we'll make the menues

	file_menu = [ Gnome::UIInfo::item_stock(N_("Exit"),
			nil, proc { Gtk::main_quit },
			Gnome::Stock::MENU_QUIT) ]
	help_menu = [ Gnome::UIInfo::item_stock(N_("About GnomeHello..."),
			nil, proc { about_cb },
			Gnome::Stock::MENU_ABOUT) ]
	main_menu = [ Gnome::UIInfo::subtree(N_("File"), file_menu),
		      Gnome::UIInfo::subtree(N_("Help"), help_menu) ]
	create_menus(main_menu)

	# We make a button, bind the 'clicked' signal to hello and setting it
	# to be the content of the main window

	button = Gtk::Button.new(_("Hello GNOME"))
	button.signal_connect("clicked") {
	    puts(_("Hello GNOME"))
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
		    _("The Hello World Gnomified"),
		    GVERSION,
		    _("(C) 1998 the Free Software Foundation"),
		    [ "Minoru Inachi (inachi@earth.interq.or.jp)" ],
		    _("GNOME is a civilized software system so we've a \"hello world\" program"))
	about.show
    end

end

Gnome.init(PACKAGE, "1.0")

# Initialize the i18n stuff
Gnome::I18n::bindtextdomain(PACKAGE, GNOMELOCALEDIR)
Gnome::I18n::textdomain(PACKAGE)

app = Hello2.new
Gtk::main()


#!/usr/local/bin/ruby

require 'gtk'
require 'gdk_imlib'
require 'gnome'
require 'gnoconfig'

class Hello < Gnome::App
    include Gnome::I18n

    def initialize

	# Makes the main window and binds the delete event so you can close
	# the program from your WM

	super("hello", _("Hello World Gnomified"))
	realize
	signal_connect("delete_event") {
	    Gtk::main_quit
	}

#	if restarted then
#	    app.set_uposition(os_x, os_y)
#	    app.set_usize(os_w, os_h)
#	end

	# Now that we've the main window we'll make the menues

	file_menu = [ [ Gnome::App::UI_ITEM,
			N_("Exit"),
			N_("Exit GNOME hello"),
			proc { Gtk::main_quit },
			nil,
			Gnome::App::PIXMAP_STOCK,
			Gnome::Stock::MENU_QUIT,
			?Q, Gdk::CONTROL_MASK, nil ] ]
	help_menu = [ Gnome::UIInfo::item_stock(N_("About..."),
			N_("Info about GNOME hello"),
			proc { about_cb },
			Gnome::Stock::MENU_ABOUT),
		      Gnome::UIInfo::separator,
		      Gnome::UIInfo::help("hello") ]
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

app = Hello.new

# Get the master client, that was hopefully connected to the
# session manager int the 'gnome_init' call.  All communication to
# the session manager will be done with this master client.
client = Gnome::master_client

puts client.config_prefix
puts client.global_config_prefix
puts Gnome::Config::get_real_path(client.config_prefix)

# Arrange to be told when something interesting happens.
client.signal_connect("save_yourself") { # save_state
    | cl, phase, save_style, shutdown, interact_style, fast |

    prefix = client.config_prefix

    # The only state that gnome-hello has is the window geometry.
    # Get it.
    x, y, w, h, depth = app.get_geometry

    # Save the state using gnome-config stuff.
    Gnome::Config::push_prefix(prefix)

    Gnome::Config::set_int("Geometry/x", x)
    Gnome::Config::set_int("Geometry/y", y)
    Gnome::Config::set_int("Geometry/w", w)
    Gnome::Config::set_int("Geometry/h", h)
    Gnome::Config::set_bool("General/restarted", true)

    Gnome::Config::pop_prefix
    Gnome::Config::sync

    # Here is the real SM code. We set the argv to the parameters needed
    # to restart/discard the session that we've just saved and call
    # the gnome_session_set_*_command to tell the session manager it.
    real_prefix = GnomeConfig::get_real_path(prefix)
    client.set_discard_command("rm", "-f", prefix)

    # Set commands to clone and restart this application.  Note that we
    # use the same values for both -- the session management code will
    # automatically add whatever magic option is required to set the
    # session id on startup.
#    client.set_clone_command(...)
#    client.set_restart_command(...)

    true
}

client.signal_connect("die") { exit }

Gtk.main

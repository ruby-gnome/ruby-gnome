=begin header

  sample.rb - a part of test-gnome.c rewritten in Ruby/GNOME

=end

require 'gtk'
require 'gdk_imlib'
require 'gnome'

AUTHORS = [
 "Richard Hestilow",
  "Federico Mena",
  "Eckehard Berns",
  "Havoc Pennington",
  "Miguel de Icaza",
  "Jonathan Blandford",
]

GVERSION = "0.0.1"

class SampleApp < Gnome::App
  include Gnome::I18n

  def initialize(normal, appname, title)
    super(appname, title)
    unless normal then
      signal_connect("delete_event") { quit_test }
    end

    file_menu = [
      [ Gnome::App::UI_ITEM, N_("Test"), nil, proc{ Gtk::main_quit }, nil,
	Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_EXIT, ?A,
	Gdk::SHIFT_MASK, nil ],
      [ Gnome::App::UI_ITEM, N_("Exit"), nil, proc{ quit_test }, nil,
	Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_EXIT, ?X,
	Gdk::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, N_("Close"), nil, proc{ window_close }, nil,
	Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_EXIT, ?X,
	Gdk::CONTROL_MASK, nil ]
    ]

    help_menu = [
      Gnome::UIInfo::item_stock(N_("About..."), nil, proc { create_about },
			      Gnome::Stock::MENU_ABOUT)
    ]

    main_menu = [
      Gnome::UIInfo::subtree(N_("File"), file_menu),
      Gnome::UIInfo::subtree(N_("Help"), help_menu)
    ]

    create_menus(main_menu)
  end

  private
  def quit_test
    Gtk::main_quit
  end

  private
  def window_close
    destroy
  end

  private
  def create_about
    about = Gnome::About.new("GNOME Test Program", GVERSION ,
                             "(C) 1998 The Free Software Foundation",
                             AUTHORS,
                             "Program to display GNOME functions.",
                             nil);
    about.show
  end

end # SampleApp

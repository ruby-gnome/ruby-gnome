=begin
  gnome-init-popt.rb - Gnome::init options sample script.

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME.

  $Date: 2002/07/25 12:12:37 $
  $Id: gnome-init-popt.rb,v 1.1 2002/07/25 12:12:37 mutoh Exp $
=end

require 'gnome'

class Test < Gnome::App
  def initialize(label, w, h, border, title)
	title = "Hello World Gnomified" unless title
	super("gnome-init-popt", title)
	realize
	signal_connect("delete_event") {
	  Gtk::main_quit
	}
	button = Gtk::Button.new(label)
	button.signal_connect(Gtk::Button::SIGNAL_CLICKED) {
	  puts(label)
	  Gtk::main_quit
	}
	set_contents(button)
	button.show
	set_usize(w, h)
	window.set_decorations(0) if border
	show
  end
end

options = [
	["label", 'l', Gnome::POPT_ARG_STRING, "Hello World", "Set button label", "label"],
	["width", 'w', Gnome::POPT_ARG_INT, 300, "Set window width", "width"],
	["height", 'h', Gnome::POPT_ARG_INT,200, "Set window height", "height"],
	["border", 'b', Gnome::POPT_ARG_NONE, nil, "Remove window border"]
  ]

args = Gnome.init("gnome-init-popt", "1.0", options)

p args[Gnome::POPT_ARGS]
p args

app = Test.new(args["label"], args["width"], args["height"], args["border"],
			   args[Gnome::POPT_ARGS][0])

Gtk.main()

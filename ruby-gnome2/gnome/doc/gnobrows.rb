require 'rbbr_gnome'
require 'gtk'
require 'gnome'

def about_cb
  about = Gnome::About.new(
	    "Ruby/GNOME Class Browser",
	    "0.0.1",
	    "(C) 1999 The Free Software Foundation",
	    [ "Minoru Inachi",
	      "Hiroshi IGARASHI",
	      "Dai.K",
	      "matz" ],
	    "Class browser for Ruby/GNOME extension library")
  about.show
end

file_menu = [ Gnome::UIInfo::item_stock("Exit",
		nil, proc { Gtk::main_quit },
		Gnome::Stock::MENU_QUIT) ]
help_menu = [ Gnome::UIInfo::item_stock("About...",
		nil, proc { about_cb },
		Gnome::Stock::MENU_ABOUT) ]
main_menu = [ Gnome::UIInfo::subtree("File", file_menu),
	      Gnome::UIInfo::subtree("Help", help_menu) ]

app = Gnome::App.new("gnobrows", "ruby browser")
app.realize

app.signal_connect("delete_event") { false }
app.signal_connect("destroy") { Gtk::main_quit }

app.create_menus(main_menu)

box = Gtk::HBox::new(false,0)
app.set_contents(box)
box.show

$descendant = Gtk::ScrolledWindow::new(nil, nil)
$method     = Gtk::ScrolledWindow::new(nil, nil)
$constant   = Gtk::ScrolledWindow::new(nil, nil)
[$descendant, $method, $constant].each do |w|
  w.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
  w.set_usize(200,400)
  box.pack_start(w, true, true, 0)
  w.show
end

module Gtk
  class TreeItem
    alias initialize0 initialize
    def initialize(*arg)
      initialize0(*arg)
      signal_connect("select") { |w| cb_select(w) }
    end
  end
end
$walker = ClassTree::new()
def cb_select(item)
  cname = item.child.get
  cls = eval(cname)
  if cls.is_a? Module
    $walker.changeConstants(cls)
    $walker.changeMethods(cls)
  end
end

roots = initialize_args({"gnome" => ["Gnome", "Gtk", "Gdk", "GdkImlib"]})
STDERR.print roots.inspect+"\n"
$walker.visit(roots)
root = $walker.createDescendantsTree()
root.set_selection_mode(Gtk::SELECTION_SINGLE)
$descendant.add_with_viewport(root)
root.show

l = $walker.createConstantsList(nil)
$constant.add(l)
l.show

l = $walker.createMethodsList(nil)
$method.add(l)
l.show

app.show
Gtk::main()

require 'rbbr_gtk'
require 'gtk2'

window = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)
window.set_title("ruby browser")
window.signal_connect("delete_event") { exit }
window.signal_connect("destroy_event") { exit }

box = Gtk::HBox::new(false,0)
window.add(box)
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

roots = initialize_args({"gtk" => ["Gtk", "Gdk"]})
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

window.show
Gtk::main()

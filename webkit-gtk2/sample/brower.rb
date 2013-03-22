require 'webkit-gtk2'

WebKitGtk2.init

window = Gtk::Window.new
window.signal_connect("destroy") do
	Gtk.main_quit
end


sw = Gtk::ScrolledWindow.new

view = WebKitGtk2::WebView.new
view.load_uri("http://webkitgtk.org/")

sw.add(view)
window.add(sw)
window.show_all

Gtk.main

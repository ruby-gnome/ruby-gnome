require 'gtk2'

Gtk.init

expander = Gtk::Expander.new("Expander Sample")
expander.expanded = true
button = Gtk::Button.new("Hide this button.")
button.signal_connect("clicked") do
  expander.expanded = false
end

button.set_size_request(200, 200)
expander.add(button)

Gtk::Window.new.add(expander).show_all

Gtk.main


require 'gtk'

window = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
window.signal_connect("delete_event") do exit end
window.signal_connect("destroy_event") do exit end
window.realize

box = Gtk::VBox.new(FALSE, 0)
window.add(box)
box.show

$str = "Hello, world."
$text = Gtk::Text.new(Gtk::Adjustment.new(0,0,0,0,0,0),
		      Gtk::Adjustment.new(0,0,0,0,0,0))
box.pack_start($text)
$text.show

button = Gtk::Button.new("append")
box.pack_start(button)
button.show
button.signal_connect("clicked") do |w|
  $text.insert_text($str + "\n", $text.get_point)
end

window.show

Gtk.main

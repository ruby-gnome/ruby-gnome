require 'gtk2'
require 'gconf2'

Gtk.init

client = GConf::Client.new
window = Gtk::Window.new
str = client.get "/apps/gnome-cd/device"
label = Gtk::Label.new(str ? str : '<unset>')
window.add label
client.add_dir "/apps/gnome-cd"
client.notify_add "/apps/gnome-cd/device" do |client, entry|
	label.text = entry.value ? entry.value : '<unset>'
end
window.show_all
Gtk.main

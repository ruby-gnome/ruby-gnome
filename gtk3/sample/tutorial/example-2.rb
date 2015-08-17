require "gtk3"

app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do |application|
  # create a new window, and set its title
  window = Gtk::ApplicationWindow.new(application)
  window.set_title("Window")
  window.set_border_width(10)

  # Here we construct the container that is going pack our buttons 
  grid = Gtk::Grid.new

  # Pack the container in the window
  window.add(grid)

  button = Gtk::Button.new(:label => "Button 1")
  button.signal_connect("clicked") { puts "Hello World" }
  # Place the first button in the grid cell (0, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 0, 0, 1, 1)

  button = Gtk::Button.new(:label => "Button 2")
  button.signal_connect("clicked") { puts "Hello World" }
  # Place the second button in the grid cell (1, 0), and make it fill
  # just 1 cell horizontally and vertically (ie no spanning)
  grid.attach(button, 1, 0, 1, 1)

  button = Gtk::Button.new(:label => "Quit")
  button.signal_connect("clicked") { window.destroy }
  # Place the Quit button in the grid cell (0, 1), and make it
  # span 2 columns.
  grid.attach(button, 0, 1, 2, 1)

  # Now that we are done packing our widgets, we show them all
  # in one go, by calling Gtk::Widget#show_all on the window.
  # This call recursively calls Gtk::Widget#show on all widgets
  # that are contained in the window, directly or indirectly
  window.show_all
end

status = app.run(ARGV)

puts status

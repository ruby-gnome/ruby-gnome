#!/usr/local/bin/ruby

#
# example-start helloworld helloworld.rb
# see Gtk+ tutorial.
#

require 'gtk2'

def hello(widget)
  print ("Hello World\n")
end

def delete_event(widget, event)
  print ("delete event occurred\n")
  TRUE
# FALSE
end

def destroy(widget)
  p "destroy"
  exit
end

def main
  window = Gtk::Window::new  # create a toplevel window when argument is omitted.
  window.signal_connect("delete_event") {|*args| delete_event(*args) }
  window.signal_connect("destroy") {|*args| destroy(*args) } # x io error..?_?
  window.border_width = 10

  button = Gtk::Button::new("Hello World")
  button.signal_connect("clicked") {|*args| hello(*args) }
  button.signal_connect("clicked") {|*args| window.destroy }

  window.add(button)
  button.show()
  window.show()
           
  Gtk::main()
end

main()


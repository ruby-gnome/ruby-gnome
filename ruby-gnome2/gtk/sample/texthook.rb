require 'gtk'

w = Gtk::Window::new(Gtk::WINDOW_TOPLEVEL)
w.signal_connect("delete_event") { exit }
w.signal_connect("destroy") { exit }


text = Gtk::Text.new
text.set_editable(true)
text.signal_connect("key_press_event") do |widget, event|
  p event

  ret = false
  if widget.editable?
    if event.length == 1 and ?a <= event.keyval and event.keyval <= ?z
      widget.delete_selection()
      widget.insert_text(event.string.capitalize, widget.position)

      widget.signal_emit_stop_by_name("key_press_event")
      ret = true
    end
  end
p ret
  ret
end
w.add(text)
text.show

w.show
Gtk::main()


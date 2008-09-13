#!/usr/bin/env ruby
require 'goocanvas'

num_added_widgets = 0
added_widget_items = []
canvas = nil
move_item = nil

def create_focus_box(canvas, x, y, width, height, color)
  root = canvas.root_item
  item = Goo::CanvasRect.new(root, x, y, width, height, :stroke_pattern => nil, :fill_color => color, :line_width => 5.0, :can_focus => true)

  item.signal_connect('focus_in_event') do
    puts "#{color} received focus-in event"
    # Note that this is only for testing. Setting item properties to indicate
    # focus isn't a good idea for real apps, as there may be multiple views.
    item.stroke_color = 'black'
    false
  end

  item.signal_connect('focus_out_event') do
    puts "#{color} received focus-out event"
    # Note that this is only for testing. Setting item properties to indicate
    # focus isn't a good idea for real apps, as there may be multiple views.
    item.stroke_pattern = nil
    false
  end

  item.signal_connect('button_press_event') do
    puts "#{color} received button-press event"
    item.canvas.grab_focus(item)
    true
  end

  item.signal_connect('key_press_event') do
    puts "#{color} received key-press event"
    false
  end
end

def add_button(hbox, button_name, &click_action)
  w = Gtk::Button.new(button_name)
  hbox.pack_start(w, false, false, 0)
  w.show
  w.signal_connect('clicked', &click_action)
end

# Create the window and widgets.
window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

vbox = Gtk::VBox.new(false, 4)
vbox.border_width = 4
vbox.show
window.add(vbox)

hbox = Gtk::HBox.new(false, 4)
vbox.pack_start(hbox, false, false, 0)
hbox.show

add_button(hbox, 'Add Widget') do
  if num_added_widgets % 2 == 1
    widget = Gtk::Label.new("Hello World")
  else
    widget = Gtk::Entry.new
  end

  root = canvas.root_item
  witem = Goo::CanvasWidget.new(root, widget, num_added_widgets * 50, num_added_widgets * 50, 200, 50, nil)

  added_widget_items.push(witem)
  num_added_widgets += 1
end

add_button(hbox, 'Remove Widget') do
  root = canvas.root_item
  witem = added_widget_items.pop
  if witem
    root.remove_child(witem)
    num_added_widgets -= 1
  end
end

move_index = 0
Moves = [
  [ 50, 50, -1, -1 ],
  [ 300, 100, -1, -1 ],
  [ 200, 200, -1, -1 ],
  [ 400, 100, -1, -1 ],
]

add_button(hbox, 'Move Widget') do
  move_item.x = Moves[move_index][0]
  move_item.y = Moves[move_index][1]
  move_item.width = Moves[move_index][2]
  move_item.height = Moves[move_index][3]
  move_index = (move_index + 1) % 4
end

# here we have to do a little more complicated than in C because Ruby/Gtk+'s enums are not ints and we can't do + 1 on them
moving_anchor = Gtk::ANCHOR_CENTER.to_i
add_button(hbox, 'Change Anchor') do
  puts "Setting anchor to: #{Gtk::AnchorType.new(moving_anchor).name}"
  move_item.anchor = moving_anchor
  moving_anchor += 1
  moving_anchor = Gtk::ANCHOR_CENTER.to_i if moving_anchor > Gtk::ANCHOR_EAST.to_i
end

add_button(hbox, 'Change Widget') do
  witem = added_widget_items.last
  if witem
    if witem.widget.instance_of?(Gtk::Entry)
      witem.widget = Gtk::Label.new("Hello World")
    else
      witem.widget = Gtk::Entry.new
    end
  end
end

hbox = Gtk::HBox.new(false, 4)
vbox.pack_start(hbox, false, false, 0)
hbox.show

add_button(hbox, 'Hide Canvas') { canvas.hide }
add_button(hbox, 'Show Canvas') { canvas.show }
add_button(hbox, 'Hide Item') { move_item.visibility = Goo::CanvasItem::INVISIBLE }
add_button(hbox, 'Show Item') { move_item.visibility = Goo::CanvasItem::VISIBLE }
add_button(hbox, 'Change Transform') { move_item.translate(25, 25) }

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.shadow_type = Gtk::SHADOW_IN
scrolled_win.show
vbox.pack_start(scrolled_win, true, true, 0)

canvas = Goo::Canvas.new
canvas.flags = Gtk::Widget::CAN_FOCUS
canvas.set_size_request(600, 450)
canvas.set_bounds(0, 0, 1000, 1000)
scrolled_win.add(canvas)

root = canvas.root_item

# Add a few simple items.
label = Gtk::Label.new("Hello World")
witem = Goo::CanvasWidget.new(root, label, 50, 50, 200, 100, nil)

entry = Gtk::Entry.new
move_item = Goo::CanvasWidget.new(root, entry, 50, 250, 200, 50, nil)

entry = Gtk::Entry.new
entry.text = "Size: -1 x -1"
witem = Goo::CanvasWidget.new(root, entry, 50, 300, -1, -1, nil)

entry = Gtk::Entry.new
entry.text = "Size: 100 x -1"
witem = Goo::CanvasWidget.new(root, entry, 50, 350, 100, -1, nil)

# Use a textview so we can see the width & height of the widget.
scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.shadow_type = Gtk::SHADOW_IN
scrolled_win.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER)
textview = Gtk::TextView.new
buffer = textview.buffer
buffer.text = "Size: -1 x 100"
textview.show
scrolled_win.add(textview)
scrolled_win.set_size_request(160, 50)
witem = Goo::CanvasWidget.new(root, scrolled_win, 50, 400, -1, 100, nil)

# Create a vbox item with several child entry widgets to check focus traversal.
vbox = Gtk::VBox.new(false, 4)

entry = Gtk::Entry.new
entry.show
vbox.pack_start(entry, false, false, 0)

entry = Gtk::Entry.new
entry.show
vbox.pack_start(entry, false, false, 0)

entry = Gtk::Entry.new
entry.show
vbox.pack_start(entry, false, false, 0)

witem = Goo::CanvasWidget.new(root, vbox, 50, 600, -1, -1, nil)

# Create a few normal canvas items that take keyboard focus.
create_focus_box(canvas, 110, 80, 50, 30, 'red')
create_focus_box(canvas, 300, 160, 50, 30, 'orange')
create_focus_box(canvas, 500, 50, 50, 30, 'yellow')

canvas.show

# Pass control to the GTK+ main event loop.
Gtk.main

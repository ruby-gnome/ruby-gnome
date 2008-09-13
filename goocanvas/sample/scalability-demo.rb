#!/usr/bin/env ruby
require 'goocanvas'

# the values behind the comments are the ones used in the C version
# but as with them it takes more than 20 sec on my computer for the window
# to show upm I decreased them a little
N_GROUP_COLS = 15 #25
N_GROUP_ROWS = 10 #20
N_COLS = 10
N_ROWS = 10

PADDING = 10

ONLY_ONE = false

def setup_canvas(canvas)
  root = canvas.root_item
  root.font = 'Sans 8'

  item_width = 400
  item_height = 19
	
  cell_width = item_width + PADDING * 2
  cell_height = item_height + PADDING * 2

  group_width = N_COLS * cell_width
  group_height = N_ROWS * cell_height

  total_width = N_GROUP_COLS * group_width
  total_height = N_GROUP_ROWS * group_height

  # We use -ve offsets to test if -ve coords are handled correctly.
  left_offset = -total_width / 2
  top_offset = -total_height / 2

  style = Goo::CanvasStyle.new
  color = Gdk::Color.parse("mediumseagreen")
  pattern = Cairo::SolidPattern.new(color.red / 65535.0, color.green / 65535.0, color.blue / 65535.0)
  style.fill_pattern = pattern

  style2 = Goo::CanvasStyle.new
  color = Gdk::Color.parse("steelblue")
  pattern = Cairo::SolidPattern.new(color.red / 65535.0, color.green / 65535.0, color.blue / 65535.0)
  style2.fill_pattern = pattern

  total_items = 0

  N_GROUP_COLS.times do |group_i|
    N_GROUP_ROWS.times do |group_j|
      group_x = left_offset + (group_i * group_width);
      group_y = top_offset + (group_j * group_height);

      group = Goo::CanvasGroup.new(root)
      total_items += 1
      group.translate(group_x, group_y)
      
      N_COLS.times do |i|
        N_ROWS.times do |j|
          item_x = (i * cell_width) + PADDING
          item_y = (j * cell_height) + PADDING
          rotation = i % 10 * 2
          rotation_x = item_x + item_width / 2
          rotation_y = item_y + item_height / 2
          
          current_id = "#{group_x + item_x}, #{group_y + item_y}"
          
          item = Goo::CanvasRect.new(group, item_x, item_y, item_width, item_height)
          item.style = (j % 2) == 1 ? style : style2
          item.rotate(rotation, rotation_x, rotation_y)
          
          item.signal_connect('motion_notify_event') do
            puts "#{current_id} item received 'motion-notify' signal"
          end

          item = Goo::CanvasText.new(group, current_id,
            item_x + item_width / 2, item_y + item_height / 2, -1, Gtk::ANCHOR_CENTER)
          item.rotate(rotation, rotation_x, rotation_y)
          
          total_items += 2
          break if ONLY_ONE
        end
        break if ONLY_ONE
      end
      break if ONLY_ONE
    end
    break if ONLY_ONE
  end

  puts "Total items: #{total_items}"
  [ total_width, total_height, left_offset, top_offset ]
end

def create_canvas
  # Create the canvas.
  canvas = Goo::Canvas.new
  canvas.set_size_request(600, 450)
  
  start = Time.new
  total_width, total_height, left_offset, top_offset = setup_canvas(canvas)
  puts "Create Canvas Time Used: #{Time.new - start}"

  start = Time.new
  canvas.set_bounds(left_offset, top_offset, left_offset + total_width, top_offset + total_height)
  canvas.show

  first_time = true
  canvas.signal_connect("expose_event") do
    if first_time
      puts "Update Canvas Time Used: #{Time.new - start}"
      first_time = false
    end
    false
  end
  
  canvas
end

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.show
window.add(scrolled_win)

canvas = create_canvas
scrolled_win.add(canvas)

Gtk.main

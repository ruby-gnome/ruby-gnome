#!/usr/bin/env ruby
=begin
  tooltips.rb - Demonstrates the new tooltip API of Gtk+ 2.12;
                rg2 translation from gtk/tests/testtooltips.c.

  Copyright (c) 2007 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: tooltips.rb,v 1.1 2007/07/10 13:17:34 ggc Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 12, 0)
    puts "This sample requires GTK+ 2.12.0 or later"
    puts str
    exit
end

def treeview_query_tooltip(treeview, keyboard_tip, x, y, tooltip)
    if keyboard_tip
        # Keyboard mode
        path, = treeview.cursor
        if not path
            return false
        end
    else
        bin_x, bin_y = treeview.convert_widget_to_bin_window_coords(x, y)
        # Mouse mode
        path, = treeview.get_path_at_pos(bin_x, bin_y)
        if not path
            return false
        end
    end
    data = treeview.model.get_value(treeview.model.get_iter(path), 0)
    tooltip.markup = "<b>Path #{path}:</b> #{data}"
    return true
end

def textview_query_tooltip(textview, keyboard_tip, x, y, tooltip, tag)
    if keyboard_tip
        iter = textview.buffer.get_iter_at_offset(textview.buffer.cursor_position)
    else
        bx, by = textview.window_to_buffer_coords(Gtk::TextView::WINDOW_TEXT, x, y)
        iter, = textview.get_iter_at_position(bx, by)
    end
    if iter.has_tag?(tag)
        tooltip.text = 'Tooltip on text tag'
        return true
    else
        return false
    end
end

def drawingarea_query_tooltip(keyboard_tip, x, y, tooltip, rectangles)
    if keyboard_tip
        return false
    end
    for r in rectangles
        if r.x < x && x < r.x + 50 && r.y < y && y < r.y + 50
            tooltip.markup = r.tooltip
            return true
        end
    end
    return false
end

Gtk.init

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.title = 'Tooltips test'
window.border_width = 10
window.signal_connect('delete-event') { Gtk.main_quit }

box = Gtk::VBox.new(false, 3)
window.add(box)

# A check button using the tooltip-markup property
button = Gtk::CheckButton.new('This one uses the tooltip-markup property')
button.tooltip_text = 'Hello, I am a static tooltip.'
box.pack_start(button, false, false, 0)

raise if button.tooltip_text != 'Hello, I am a static tooltip.'
raise if button.tooltip_markup != 'Hello, I am a static tooltip.'

# A check button using the query-tooltip signal
button = Gtk::CheckButton.new('I use the query-tooltip signal')
button.has_tooltip = true
button.signal_connect('query-tooltip') { |widget, x, y, keyboard_tip, tooltip|
    tooltip.markup = widget.label
    tooltip.set_icon_from_stock(Gtk::Stock::DELETE, Gtk::IconSize::MENU)
    true
}
box.pack_start(button, false, false, 0)

# A label
label = Gtk::Label.new('I am just a label')
label.selectable = false
label.tooltip_text = 'Label & and tooltip'
box.pack_start(label, false, false, 0)

raise if label.tooltip_text != "Label & and tooltip"
raise if label.tooltip_markup != "Label &amp; and tooltip"

# A selectable label
label = Gtk::Label.new('I am a selectable label')
label.selectable = true
label.tooltip_markup = '<b>Another</b> Label tooltip'
box.pack_start(label, false, false, 0)

raise if label.tooltip_text != 'Another Label tooltip'
raise if label.tooltip_markup != '<b>Another</b> Label tooltip'

# Another one, with a custom tooltip window
button = Gtk::CheckButton.new('This one has a custom tooltip window!')
box.pack_start(button, false, false, 0)

tooltip_window = Gtk::Window.new(Gtk::Window::POPUP)
tooltip_button = Gtk::Label.new('blaat!')
tooltip_window.add(tooltip_button)
tooltip_button.show

button.tooltip_window = tooltip_window
button.signal_connect('query-tooltip') { |widget, x, y, keyboard_tip, tooltip|
    widget.tooltip_window.modify_bg(Gtk::StateType::NORMAL, Gdk::Color.new(0, 65535, 0))
    true
}
button.has_tooltip = true

# An insensitive button
button = Gtk::Button.new('This one is insensitive')
button.sensitive = false
button.tooltip_text = 'Insensitive!'
box.pack_start(button, false, false, 0)

# Tree view
store = Gtk::TreeStore.new(String)
iter = store.insert(nil, 0, ['File Manager'])
iter = store.insert(nil, 0, ['Gossip'])
iter = store.insert(nil, 0, ['System Settings'])
iter = store.insert(nil, 0, ['The GIMP'])
iter = store.insert(nil, 0, ['Terminal'])
iter = store.insert(nil, 0, ['Word Processor'])
treeview = Gtk::TreeView.new(store)
treeview.set_size_request(200, 240)
treeview.append_column(Gtk::TreeViewColumn.new('Test', Gtk::CellRendererText.new, { :text => 0 }))
treeview.has_tooltip = true
treeview.signal_connect('query-tooltip') { |widget, x, y, keyboard_tip, tooltip|
    treeview_query_tooltip(widget, keyboard_tip, x, y, tooltip)
}
treeview.selection.signal_connect('changed') { treeview.trigger_tooltip_query }
# Set a tooltip on the column
column = treeview.get_column(0)
column.clickable = true
#column.button.tooltip_text = 'Header'   .button not available
box.pack_start(treeview, false, false, 2)

# Text view
buffer = Gtk::TextBuffer.new
buffer.insert(buffer.end_iter, 'Hello, the text ')
tag = buffer.create_tag('bold', { 'weight' => Pango::WEIGHT_BOLD })
buffer.insert(buffer.end_iter, 'in bold', tag)
buffer.insert(buffer.end_iter, ' has a tooltip!')
textview = Gtk::TextView.new(buffer)
textview.set_size_request(200, 50)
textview.has_tooltip = true
textview.signal_connect('query-tooltip') { |widget, x, y, keyboard_tip, tooltip|
    textview_query_tooltip(widget, keyboard_tip, x, y, tooltip, tag)
}
box.pack_start(textview, false, false, 2)

# Drawing area
if Gdk.cairo_available?
    Rectangle = Struct.new("Rectangle", :x, :y, :r, :g, :b, :tooltip)
    rectangles = [ Rectangle.new(10, 10, 0.0, 0.0, 0.9, "Blue box!"),
                   Rectangle.new(200, 170, 1.0, 0.0, 0.0, "Red thing"),
                   Rectangle.new(100, 50, 0.8, 0.8, 0.0, "Yellow thing") ]
    drawingarea = Gtk::DrawingArea.new
    drawingarea.set_size_request(320, 240)
    drawingarea.has_tooltip = true
    drawingarea.signal_connect('expose_event') {
        cr = drawingarea.window.create_cairo_context
        cr.rectangle(0, 0, drawingarea.allocation.width, drawingarea.allocation.height)
        cr.set_source_rgb(1.0, 1.0, 1.0)
        cr.fill
        rectangles.each { |r|
            cr.rectangle(r.x, r.y, 50, 50)
            cr.set_source_rgb(r.r, r.g, r.b)
            cr.stroke
            cr.rectangle(r.x, r.y, 50, 50)
            cr.set_source_rgba(r.r, r.g, r.b, 0.5)
            cr.fill
        }
    }
    drawingarea.signal_connect('query-tooltip') { |widget, x, y, keyboard_tip, tooltip|
        drawingarea_query_tooltip(keyboard_tip, x, y, tooltip, rectangles)
    }
    box.pack_start(drawingarea, false, false, 2)
else
    warn "Part of this sample needs Cairo support. Make sure your ruby-gtk2 is compiled with Cairo support, and rcairo is installed."
end

window.show_all
Gtk.main

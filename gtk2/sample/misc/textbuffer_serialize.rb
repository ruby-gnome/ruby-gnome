#!/usr/bin/env ruby
=begin
  textbuffer_serialize.rb - Ruby/GTK sample script.

  Copyright (c) 2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: textbuffer_serialize.rb,v 1.1 2006/11/23 08:39:13 mutoh Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 10, 0)
  puts "This sample requires GTK+ 2.10.0 or later"
  puts str
  exit
end

current_folder = ENV['HOME'] || "."
file_name = "serialized.dat"

textview = Gtk::TextView.new
textview.set_size_request(600, 400)
buffer = textview.buffer
buffer.text = DATA.read
format = buffer.serialize_formats[0]
buffer.register_serialize_target(nil)
buffer.register_deserialize_target(nil)

window = Gtk::Window.new("Gtk::TextBuffer Serialize Demo")
vbox = Gtk::VBox.new

serialize_button = Gtk::FileChooserButton.new("Serialize to a file", 
                                              Gtk::FileChooser::ACTION_OPEN)
deserialize_button = Gtk::FileChooserButton.new("Serialize to a file", 
                                                Gtk::FileChooser::ACTION_OPEN)

serialize_button.current_folder = current_folder
deserialize_button.current_folder = current_folder

toolbar = Gtk::Toolbar.new

toolbar.append(Gtk::Stock::OPEN, "Deserialize from a file") do
  dialog = Gtk::FileChooserDialog.new("Deserialize from a file",
                                      window,
                                      Gtk::FileChooser::ACTION_OPEN,
                                      nil,
                                      [Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL],
                                      [Gtk::Stock::OPEN, Gtk::Dialog::RESPONSE_ACCEPT])
  
  dialog.filename = File.expand_path(file_name)
  if dialog.run == Gtk::Dialog::RESPONSE_ACCEPT
    file_name = dialog.filename
    File.open(file_name, "rb") {|io|
      buffer.delete(buffer.start_iter, buffer.end_iter)
      buffer.deserialize(buffer, format, buffer.start_iter, io.read)
    }
  end
  dialog.destroy
end

toolbar.append(Gtk::Stock::SAVE, "Serialize to a file") do
  dialog = Gtk::FileChooserDialog.new("Serialize from a file",
                                      window,
                                      Gtk::FileChooser::ACTION_SAVE,
                                      nil,
                                      [Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL],
                                      [Gtk::Stock::OPEN, Gtk::Dialog::RESPONSE_ACCEPT])
  dialog.current_name = file_name
  if dialog.run == Gtk::Dialog::RESPONSE_ACCEPT
    file_name = dialog.filename
    File.open(file_name, "wb") {|io|
      io.write(buffer.serialize(buffer, format, buffer.start_iter, buffer.end_iter))
    }
  end
  dialog.destroy
end

toolbar.append(Gtk::Stock::CLEAR, "Clear all") do
  buffer.delete(buffer.start_iter, buffer.end_iter)
end
toolbar.append_space
toolbar.append(Gtk::Stock::SELECT_COLOR, "Color the region") do
  dialog = Gtk::ColorSelectionDialog.new("Color the region")
  if dialog.run == Gtk::Dialog::RESPONSE_OK
    bounds = buffer.selection_bounds
    color = dialog.colorsel.current_color
    tag_name = color.to_a.inspect
    unless tag = buffer.tag_table.lookup(tag_name)
      tag = Gtk::TextTag.new(tag_name).set_foreground_gdk(color)
    end
    buffer.tag_table.add(tag)
    buffer.apply_tag(tag, bounds[0], bounds[1])
  end
  dialog.destroy  
end

toolbar.append(Gtk::Stock::SELECT_FONT, "Set a font to the region") do
  dialog = Gtk::FontSelectionDialog.new("Set font to the region")
  if dialog.run == Gtk::Dialog::RESPONSE_OK
    bounds = buffer.selection_bounds
    font = dialog.font_name
    unless tag = buffer.tag_table.lookup(font)
      tag = Gtk::TextTag.new(font).set_font(font)
    end
    buffer.tag_table.add(tag)
    buffer.apply_tag(tag, bounds[0], bounds[1])
  end
  dialog.destroy  
end

toolbar.append_space
toolbar.append(Gtk::Stock::QUIT, "Quit this application") do
  Gtk.main_quit
end

vbox.pack_start(toolbar, false, false).add(Gtk::ScrolledWindow.new.add(textview))
window.add(vbox)
window.show_all
window.signal_connect("destroy") { Gtk.main_quit }

Gtk.main

__END__
This is a sample script for rich text serialization/deserialization.

1. Edit this text using font/color buttons.
2. Click save button and save this text (= serialize this as rich text to a file)
3. Click clear button and clear this text buffer.
4. Click load button and load the file which you save (= deserialize this).

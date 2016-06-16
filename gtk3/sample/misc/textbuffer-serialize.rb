#!/usr/bin/env ruby
=begin
  textbuffer_serialize.rb - Ruby/GTK sample script.

  Copyright (c) 2006-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

unless Gtk::Version.or_later?(3, 4, 2)
  puts "This sample requires GTK+ 3.4.2 or later: #{Gtk::Version::STRING}"
  exit
end

current_folder = ENV["HOME"] || "."
file_name = "serialized.dat"

textview = Gtk::TextView.new
textview.set_size_request(600, 400)
buffer = textview.buffer
buffer.text = <<TEXT
This is a sample script for rich text serialization/deserialization.

1. Edit this text using font/color buttons.
2. Click save button and save this text (= serialize this as rich text to a file)
3. Click clear button and clear this text buffer.
4. Click load button and load the file which you save (= deserialize this).
TEXT

deserialize_format = buffer.register_deserialize_tagset(nil)
format = buffer.serialize_formats[0]

window = Gtk::Window.new("Gtk::TextBuffer Serialize Demo")
vbox = Gtk::Box.new(:vertical, 0)

toolbar = Gtk::Toolbar.new
toolbar.set_style(Gtk::ToolbarStyle::BOTH)
button_open = Gtk::ToolButton.new(:icon_widget => nil, :label => "Deserialize from a file",:stock_id => Gtk::Stock::OPEN)
button_open.signal_connect "clicked" do
  dialog = Gtk::FileChooserDialog.new(:title => "Deserialize from a file",
                                      :parent => window,
                                      :actions => :open,
                                      :buttons => [
                                      [Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL],
                                      [Gtk::Stock::OPEN, Gtk::ResponseType::ACCEPT]])
  
  dialog.filename = File.expand_path(file_name)
  if dialog.run == Gtk::ResponseType::ACCEPT
    file_name = dialog.filename
    File.open(file_name, "rb") {|io|
      buffer.delete(buffer.start_iter, buffer.end_iter)
      buffer.deserialize(buffer, deserialize_format, buffer.start_iter, io.read)
    }
  end
  dialog.destroy
end

button_save = Gtk::ToolButton.new(:icon_widget => nil, :label => "Serialize to a file",:stock_id => Gtk::Stock::SAVE)
button_save.signal_connect "clicked" do 
  dialog = Gtk::FileChooserDialog.new(:title => "Serialize to a file",
                                      :parent => window,
                                      :action => :save, #or Gtk::FileChooserAction::SAVE,
                                      :buttons => [
                                        [Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL],
                                        [Gtk::Stock::OPEN, Gtk::ResponseType::ACCEPT]
                                      ])
  dialog.current_name = file_name
  if dialog.run == Gtk::ResponseType::ACCEPT
    file_name = dialog.filename
    File.open(file_name, "wb") {|io|
      data = buffer.serialize(buffer, format, buffer.start_iter, buffer.end_iter)
      io.write(data)
    }
  end
  dialog.destroy
end

button_clear = Gtk::ToolButton.new(:icon_widget => nil, :label => "Clear all",:stock_id => Gtk::Stock::CLEAR)
button_clear.signal_connect("clicked") { buffer.delete(buffer.start_iter, buffer.end_iter) }
toolbar.insert(button_clear, 0)

button_color = Gtk::ToolButton.new(:icon_widget => nil, :label => "Color the region",:stock_id => Gtk::Stock::SELECT_COLOR)
button_color.signal_connect "clicked" do 
  dialog = Gtk::ColorChooserDialog.new(:title => "Color the region", :parent => window)
  if dialog.run == Gtk::ResponseType::OK
    bounds = buffer.selection_bounds # returns an array [Gtk::TextIter, Gtk::TextIter] or nil
    if bounds
      rgba = dialog.rgba
      color = Gdk::Color.new(rgba.red*65535, rgba.green*65535, rgba.blue*65535)
      tag_name = color.to_s 
      unless tag = buffer.tag_table.lookup(tag_name)
        tag = Gtk::TextTag.new(tag_name)
        tag.set_foreground_gdk(color)
      end
      buffer.tag_table.add(tag)
      buffer.apply_tag(tag, bounds[0], bounds[1])
    end
  end
  dialog.destroy  
end

button_font = Gtk::ToolButton.new(:label => "Set a font to the region", :stock_id => Gtk::Stock::SELECT_FONT)
button_font.signal_connect "clicked" do
  dialog = Gtk::FontChooserDialog.new(:label => "Set font to the region", :parent => window)
  if dialog.run == Gtk::ResponseType::OK
    bounds = buffer.selection_bounds
    if bounds
      font = dialog.font
      unless tag = buffer.tag_table.lookup(font)
        tag = Gtk::TextTag.new(font).set_font(font)
      end
      buffer.tag_table.add(tag)
      buffer.apply_tag(tag, bounds[0], bounds[1])
    end
  end
  dialog.destroy  
end

button_quit = Gtk::ToolButton.new(:label => "Quit this application", :stock_id => Gtk::Stock::QUIT)
button_quit.signal_connect("clicked") {Gtk.main_quit}

toolbar.insert(button_font, 0)
toolbar.insert(button_color,0)
toolbar.insert(button_open, 0)
toolbar.insert(button_save, 0)
toolbar.insert(button_quit, -1)
toolbar.set_style(Gtk::ToolbarStyle::BOTH)
vbox.pack_start(toolbar, :expand => false, :fill => false)
vbox.pack_start(textview, :expand => true, :fill => true)

window.add(vbox)
window.show_all
window.set_default_size 400,600
window.signal_connect("destroy") { Gtk.main_quit }

Gtk.main

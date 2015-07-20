=begin
  xbm_cursor.rb - Gdk::Cursor sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  Copyright (c) 2001-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("xbm Cursor example")
window.signal_connect("destroy") { Gtk.main_quit }

width = 19
height = 19
format = Cairo::FORMAT_A1
stride = Cairo::Format.stride_for_width(format, width)

cursor_xbm_data = [
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xf0, 0x3f, 0x00, 0xe0, 0x78, 0x00, 0xe0, 0x70, 0x00, 0xe0, 0x70, 0x00,
  0xe0, 0x38, 0x00, 0xe0, 0x1f, 0x00, 0xe0, 0x1d, 0x00, 0xe0, 0x38, 0x00,
  0xe0, 0x78, 0x00, 0xe0, 0xf0, 0x00, 0xf0, 0xf3, 0x01, 0xf0, 0xe3, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
].pack("C*")

cursor_mask_xbm_data = [
  0x63, 0x8e, 0x0f, 0x67, 0x8e, 0x0f, 0x06, 0xc6, 0x07, 0x04, 0x00, 0x00,
  0xf0, 0x3f, 0x08, 0xe7, 0x78, 0x0e, 0xe7, 0x70, 0x0f, 0xe4, 0x70, 0x03,
  0xe0, 0x38, 0x00, 0xe7, 0x1f, 0x0c, 0xe7, 0x1d, 0x0f, 0xe0, 0x38, 0x0e,
  0xe0, 0x78, 0x08, 0xe7, 0xf0, 0x00, 0xf7, 0xf3, 0x01, 0xf0, 0xe3, 0x01,
  0x04, 0x00, 0x00, 0x46, 0x24, 0x06, 0x67, 0x66, 0x0e, 0x67, 0x66, 0x0e
].pack("C*")

background_color = [0, 1, 0, 1]
foreground_color = [1, 0, 0, 1]

def xbm_data_to_cairo_data(data, width, stride)
  n_bytes_per_row = (width / 8.0).ceil

  cairo_data_bytes = []
  data.unpack("C*").each_slice(n_bytes_per_row) do |row|
    row.each do |byte|
      cairo_data_bytes << byte
    end
    (stride - n_bytes_per_row).times do
      cairo_data_bytes << 0b0
    end
  end
  cairo_data_bytes.pack("C*")
end

cursor_data = xbm_data_to_cairo_data(cursor_xbm_data,
                                     width, stride)

cursor_mask_data = xbm_data_to_cairo_data(cursor_mask_xbm_data,
                                          width, stride)

source = Cairo::ImageSurface.new(cursor_data, format, width, height, stride)
mask = Cairo::ImageSurface.new(cursor_mask_data, format, width, height, stride)

# Append mask on the source
Cairo::Context.new(source) do |context|
  context.mask(mask, 0, 0)
end

# Convert the xbm FORMAT_A1 to FORMAT_ARGB32
cursor_surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32, width, height)
Cairo::Context.new(cursor_surface) do |context|
  context.set_source(*background_color)
  context.paint
  context.set_source(*foreground_color)
  context.mask(source, 0, 0)
end

offset_x = 10
offset_y = 10

cursor = Gdk::Cursor.new(cursor_surface, offset_x, offset_y)
window.realize
window.window.set_cursor(cursor)
window.add(Gtk::Label.new("Put your cursor on this window."))
window.set_default_size(200, 100)
window.show_all

Gtk.main

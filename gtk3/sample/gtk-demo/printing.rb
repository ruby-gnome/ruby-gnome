# Copyright (c) 2008 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
=begin
= Printing

Gtk::PrintOperation offers a simple API to support printing
in a cross-platform way.
=end

require 'common'

module Demo
  class Printing < BasicWindow
    Data = Struct.new(:font_size, :lines_per_page, :lines, :n_pages)
    HEADER_HEIGHT = 10 * 72 / 25.4
    HEADER_GAP = 3 * 72 / 25.4

    def initialize
      super('Printing')

      button = Gtk::Button.new("Print...")
      button.signal_connect("clicked") do
        begin
          run_print_operation
        rescue
          dialog = Gtk::MessageDialog.new(self, :destroy_with_parent, :error,
                                          :close, $!.message)
          dialog.signal_connect("response") do
            dialog.destroy
            true
          end
          dialog.show
        end
        true
      end
      add(button)
    end

    private
    def run_print_operation
      operation = Gtk::PrintOperation.new

      data = Data.new
      data.font_size = 12.0
      operation.signal_connect("begin-print") do |_operation, context|
        on_begin_print(_operation, context, data)
      end
      operation.signal_connect("draw-page") do |_operation, context, page_number|
        on_draw_page(_operation, context, page_number, data)
      end
      operation.signal_connect("end-print") do |_operation, context|
        on_end_print(_operation, context, data)
      end

      operation.use_full_page = false
      operation.unit = :points

      operation.run(:print_dialog, self)
    end

    def on_begin_print(operation, context, data)
      height = context.height - HEADER_HEIGHT - HEADER_GAP
      data.lines_per_page = (height / data.font_size).floor
      data.lines = File.readlines(__FILE__)
      data.n_pages = (data.lines.size - 1) / data.lines_per_page + 1
      operation.set_n_pages(data.n_pages)
    end

    def on_draw_page(operation, context, page_number, data)
      cr = context.cairo_context
      draw_header(cr, operation, context, page_number, data)
      draw_body(cr, operation, context, page_number, data)
    end

    def draw_header(cr, operation, context, page_number, data)
      width = context.width

      cr.rectangle(0, 0, width, HEADER_HEIGHT)
      cr.set_source_rgb(0.8, 0.8, 0.8)
      cr.fill_preserve

      cr.set_source_rgb(0, 0, 0)
      cr.line_width = 1
      cr.stroke

      layout = context.create_pango_layout
      layout.font_description = "sans 14"

      layout.text = File.basename(__FILE__)
      text_width, text_height = layout.pixel_size

      if (text_width > width)
        layout.width = width
        layout.ellipsize = :start
        text_width, text_height = layout.pixel_size
      end

      y = (HEADER_HEIGHT - text_height) / 2

      cr.move_to((width - text_width) / 2, y)
      cr.show_pango_layout(layout)

      layout.text = "#{page_number + 1}/#{data.n_pages}"
      layout.width = -1
      text_width, text_height = layout.pixel_size
      cr.move_to(width - text_width - 4, y)
      cr.show_pango_layout(layout)
    end

    def draw_body(cr, operation, context, page_number, data)
      layout = context.create_pango_layout
      description = Pango::FontDescription.new("monosapce")
      description.size = data.font_size * Pango::SCALE
      layout.font_description = description

      cr.move_to(0, HEADER_HEIGHT + HEADER_GAP)
      start_line = page_number * data.lines_per_page
      data.lines[start_line, data.lines_per_page].each do |line|
        layout.text = line
        cr.show_pango_layout(layout)
        cr.rel_move_to(0, data.font_size)
      end
    end

    def on_end_print(operation, context, data)
    end
  end
end

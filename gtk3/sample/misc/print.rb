#!/usr/bin/env ruby
=begin
   print.rb - Gtk::Print* sample

   Copyright (C) 2007 Mathieu Blondel
   Copyright (C) 2007-2015 Ruby-GNOME2 Project Team
   This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

# Values returned by a Pango::Layout or set in a Pango::Layout
# are always multiple of Pango::SCALE. Those are convenience methods to
# deal with that.
class Pango::Layout
  def size_in_points
    size.collect { |v| v / Pango::SCALE }
  end

  def width_in_points
    size[0] / Pango::SCALE
  end

  def height_in_points
    size[1] / Pango::SCALE
  end

  def width_in_points=(width)
    self.width = width * Pango::SCALE
  end
end

class Print < Gtk::PrintOperation
  FONT = Pango::FontDescription.new("sans 12")
  FONT_SMALL = Pango::FontDescription.new("sans 8")

  FONT_SIZE = 12
  FONT_SMALL_SIZE = 8

  def initialize(parent_window, text)
    super()
    @parent_window = parent_window
    @text = text

    # with this option disabled, the origin is the the upper left corner
    # *taking into consideration margins* !
    self.use_full_page = false
    self.unit = Gtk::Unit::POINTS

    # set default paper size
    page_setup = Gtk::PageSetup.new
    paper_size = Gtk::PaperSize.new(Gtk::PaperSize.default)
    page_setup.paper_size_and_default_margins = paper_size
    self.default_page_setup = page_setup

    # show a progress bar
    self.show_progress = true

    # You must choose between "paginate" and "begin-print".
    # "begin-print" is emitted only once and "paginate" is emitted
    # until the block returns true.
    # In both cases, you must set the number of pages at the end with
    # Gtk::PrintOperation#n_pages=
    signal_connect("begin-print") do |pop, context|
      puts "calls begin-print"
      cr = context.cairo_context
      paragraphs = @text.split("\n")
      layouts = []
      paragraphs.each do |para|
        layouts << create_layout(cr, para)
      end

      # distribute paragraph layouts among pages
      @page_layouts = []

      curr_height = 0
      n_pages = 0
      layouts.each do |layout|
        height = layout.height_in_points
        if curr_height + height > real_page_height
          n_pages += 1
          curr_height = 0
        end
        @page_layouts[n_pages] ||= []
        @page_layouts[n_pages] << layout
        curr_height += height
      end

      pop.n_pages = n_pages + 1
    end

    signal_connect("draw-page") do |_pop, context, page_num|
      puts format("calls draw-page %d", page_num)
      cr = context.cairo_context

      x, y = [0, 0] # this means we start at the upper left margin

      if @page_layouts[page_num]
        @page_layouts[page_num].each do |layout|
          cr.move_to(x, y)
          cr.show_pango_layout(layout)
          y += layout.height_in_points
        end
        total_pages = @page_layouts.length
      else
        total_pages = 1
      end

      # page_num starts at 0
      draw_footer(cr, page_num + 1, total_pages)
    end
  end

  def run_print_dialog
    res = run(Gtk::PrintOperationAction::PRINT_DIALOG, @parent_window)
    case res
    when Gtk::PrintOperationResult::ERROR
      puts "error"
    when Gtk::PrintOperationResult::CANCEL
      puts "cancelled"
    when Gtk::PrintOperationResult::APPLY
      puts "applied"
    when Gtk::PrintOperationResult::IN_PROGRESS
      puts "in progress"
    end
  end

  def run_preview
    res = run(Gtk::PrintOperationAction::PREVIEW, @parent_window)
  end

  private

  def page_height
    setup = default_page_setup
    # this takes margins into consideration, contrary to get_paper_height
    setup.get_page_height(Gtk::Unit::POINTS)
  end

  def page_width
    setup = default_page_setup
    width = setup.get_page_width(Gtk::Unit::POINTS)
  end

  def real_page_height
    page_height - footer_height
  end

  def footer_height
    5 * FONT_SMALL_SIZE
  end

  def create_layout(cr, text)
    layout = cr.create_pango_layout

    layout.width_in_points = page_width
    layout.font_description = FONT
    layout.wrap = Pango::Layout::WrapMode::CHAR
    layout.ellipsize = Pango::Layout::ELLIPSIZE_NONE
    layout.single_paragraph_mode = false

    layout.text = text

    layout
  end

  def draw_footer(cr, nth_page, total_page)
    layout = cr.create_pango_layout
    layout.alignment = Pango::Layout::ALIGN_RIGHT
    layout.font_description = FONT_SMALL
    layout.text = format("Page %d/%d\n%s\nPowered by Ruby-GNOME2!",
                         nth_page, total_page,
                         Time.now.strftime("Printed on %Y/%m/%d at %H:%M"))
    width, height = layout.size_in_points
    x, y = [page_width, page_height]
    x -= width
    y -= height
    cr.move_to(x, y)
    cr.show_pango_layout(layout)
    cr.rel_move_to(width, -2)
    cr.rel_line_to(-page_width, 0)
    cr.stroke
  end
end

class Window < Gtk::Window
  def initialize
    super

    signal_connect("delete-event") { Gtk.main_quit }

    set_default_size(600, 600)

    @textview = Gtk::TextView.new
    @textview.wrap_mode = Gtk::WrapMode::WORD
    fill_buffer

    hbox = Gtk::Box.new(:horizontal)

    page_setup_button = Gtk::Button.new(:label => "Page setup")
    page_setup_button.signal_connect("clicked") do
      @page_setup = Gtk.print_run_page_setup_dialog(self, # parent window
                                                    @page_setup)
    end
    print_preview_button = Gtk::Button.new(:label => "Print Preview")
    print_preview_button.signal_connect("clicked") do
      printop = Print.new(self, @textview.buffer.text)
      printop.default_page_setup = @page_setup if @page_setup
      printop.run_preview
    end

    print_button = Gtk::Button.new(:label => "Print")
    print_button.signal_connect("clicked") do
      printop = Print.new(self, @textview.buffer.text)
      printop.default_page_setup = @page_setup if @page_setup
      printop.run_print_dialog
    end

    [page_setup_button, print_preview_button, print_button].each do |b|
      hbox.pack_start(b, :expand => true, :fill => true)  # expand, fill
    end

    vbox = Gtk::Box.new(:vertical)
    scroll = Gtk::ScrolledWindow.new
    @textview.show
    scroll.add(@textview)
    scroll.set_policy(:automatic, :automatic)
    vbox.pack_start(scroll, :expand => true, :fill => true)
    vbox.pack_end(hbox, :expand => false, :fill => false)

    add(vbox)

    show_all
  end

  private

  def fill_buffer
    open(__FILE__).read.each_line do |line|
      @textview.buffer.insert_at_cursor(line)
    end
    @textview.buffer.place_cursor(@textview.buffer.start_iter)
  end
end

Window.new

Gtk.main

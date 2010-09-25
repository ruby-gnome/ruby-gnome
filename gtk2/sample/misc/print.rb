#!/usr/bin/env ruby
=begin
  print.rb - Gtk::Print* sample

  Copyright (C) 2007 Mathieu Blondel
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: print.rb,v 1.1 2007/08/05 15:41:31 mutoh Exp $
=end

require 'gtk2'

# Values returned by a Pango::Layout or set in a Pango::Layout
# are always multiple of Pango::SCALE. Those are convenience methods to
# deal with that.
class Pango::Layout
    def size_in_points
        self.size.collect { |v| v / Pango::SCALE }
    end

    def width_in_points
        self.size[0] / Pango::SCALE
    end

    def height_in_points
        self.size[1] / Pango::SCALE
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

    def initialize (parent_window, text)
        super()
        @parent_window = parent_window
        @text = text

        # with this option disabled, the origin is the the upper left corner
        # *taking into consideration margins* !
        self.use_full_page = false
        self.unit = Gtk::PaperSize::UNIT_POINTS

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

        signal_connect("draw-page") do |pop, context, page_num|
            puts "calls draw-page %d" % page_num

            cr = context.cairo_context

            x, y = [0,0] # this means we start at the upper left margin  

            if @page_layouts[page_num]
                @page_layouts[page_num].each do |layout|
                    cr.move_to(x,y)
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
        res = run(ACTION_PRINT_DIALOG, @parent_window)
        case res
            when RESULT_ERROR
                puts "error"
            when RESULT_CANCEL
                puts "cancelled"
            when RESULT_APPLY
                puts "applied"
            when RESULT_IN_PROGRESS
                puts "in progress"
        end
    end

    def run_preview
        res = run(ACTION_PREVIEW, @parent_window)
    end

    private

    def page_height
        setup = self.default_page_setup
        # this takes margins into consideration, contrary to get_paper_height
        setup.get_page_height(Gtk::PaperSize::UNIT_POINTS)
    end

    def page_width
        setup = self.default_page_setup
        width = setup.get_page_width(Gtk::PaperSize::UNIT_POINTS)
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
        layout.wrap = Pango::Layout::WRAP_CHAR
        layout.ellipsize = Pango::Layout::ELLIPSIZE_NONE
        layout.single_paragraph_mode = false

        layout.text = text

        layout
    end

    def draw_footer(cr, nth_page, total_page)        
        layout = cr.create_pango_layout
        layout.alignment = Pango::Layout::ALIGN_RIGHT
        layout.font_description = FONT_SMALL
        layout.text = "Page %d/%d" % [nth_page, total_page] + "\n" + \
            Time.now.strftime("Printed on %Y/%m/%d at %H:%M") + "\n" + \
                "Powered by Ruby-GNOME2!"
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
        @textview.wrap_mode = Gtk::TextTag::WRAP_WORD

        hbox = Gtk::HBox.new

        page_setup_button = Gtk::Button.new
        page_setup_button.label = "Page setup"
        page_setup_button.signal_connect("clicked") do
            @page_setup = Print::run_page_setup_dialog(self, # parent window
                                                       @page_setup)
        end

        print_preview_button = Gtk::Button.new(Gtk::Stock::PRINT_PREVIEW)
        print_preview_button.signal_connect("clicked") do
            printop = Print.new(self, @textview.buffer.text)
            printop.default_page_setup = @page_setup if @page_setup
            printop.run_preview
        end

        print_button = Gtk::Button.new(Gtk::Stock::PRINT)
        print_button.signal_connect("clicked") do
            printop = Print.new(self, @textview.buffer.text)
            printop.default_page_setup = @page_setup if @page_setup
            printop.run_print_dialog
        end

        [page_setup_button, print_preview_button, print_button].each do |b|
            hbox.pack_start(b, true, true)  # expand, fill
        end

        scrollbar = Gtk::VScrollbar.new

        vbox = Gtk::VBox.new
        scroll = Gtk::ScrolledWindow.new.add(@textview)        
        scroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
        vbox.pack_start(scroll)
        vbox.pack_end(hbox, false, false)

        add(vbox)

        show_all
        
    end

end

Window.new

Gtk.main

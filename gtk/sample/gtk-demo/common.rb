=begin
  common.rb - Common class for gtk-demo.

  Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: common.rb,v 1.7 2005/10/12 05:38:30 ktou Exp $
=end

require 'gtk2'

module Demo
  def self.find_file(basename)
    %w(. /usr/share/gtk-2.0/demo /usr/local/share/gtk-2.0/demo/).each do |dirname|
      path = File.join(dirname, basename)
      if File.exist?(path)
	return path
      end
    end

    raise "#{basename}: No such file or directory"
  end

  class BasicWindow < Gtk::Window
    def initialize(title = nil)
      super(Gtk::Window::TOPLEVEL)
      if title
	set_title("#{title} in Ruby/GTK")
      end

      signal_connect("delete_event") do |widget, event|
	quit
      end
    end

    def quit
      destroy
      true
    end
  end

  class CairoWindow < BasicWindow

    def initialize(title=nil)
      super
      
      unless cairo_available?
        add_cairo_require_label
        return
      end

      drawing_area = Gtk::DrawingArea.new
      add(drawing_area)

      drawing_area.signal_connect("expose_event") do |widget, event|
        cr = widget.window.create_cairo_context
        cr.scale(*widget.window.size)
        cr.set_line_width(0.04)

        cr.save do
          cr.set_source_color(Gdk::Color.new(65535, 65535, 65535))
          cr.gdk_rectangle(Gdk::Rectangle.new(0, 0, 1, 1))
          cr.fill
        end
        
        draw(cr)
      end
    end
    
    def cairo_available?
      Gdk::Drawable.instance_methods.include?("create_cairo_context")
    end

    def add_cairo_require_label
      message = "This sample requires GTK+ 2.8.0 or later and cairo support"
      add(Gtk::Label.new(message))
    end

    def draw(cr)
      raise "not implemented"
    end
  end
end

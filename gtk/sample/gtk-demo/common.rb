=begin
  common.rb - Common class for gtk-demo.

  Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: common.rb,v 1.6 2005/01/30 23:14:15 kzys Exp $
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
end

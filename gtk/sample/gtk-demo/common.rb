# $Id: common.rb,v 1.4 2003/10/14 13:50:24 kzys Exp $
require 'gtk2'

module Demo
  def self.find_file(basename)
    %w(. /usr/share/gtk-2.0/demo).each do |dirname|
      path = File.join(dirname, basename)
      if File.exist?(path)
	return path
      end
    end

    raise Exception
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

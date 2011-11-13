# Copyright (c) 2008 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Interface Builder

Demonstrates an interface loaded from a XML description.
=end

require 'common'

module Demo
  class Dialog < Demo::BasicWindow
    def initialize
      super('Interface Builder')

      available = Gtk.check_version?(2, 12, 0)
      if available
        label = Gtk::Label.new("Build an interface from XML description.")
      else
        label = Gtk::Label.new("You need GTK+ >= 2.12.0 to run this demo.")
      end
      add(label)
      return unless available

      @builder = Gtk::Builder.new
      filename = File.join(File.dirname(__FILE__), "demo.ui")
      @builder << filename
      @builder.connect_signals {|name| method(name)}
      @window = @builder["window1"]
      @window.show_all

      signal_connect("destroy") do
        @window.destroy unless @window.destroyed?
      end
    end

    private
    def quit_activate
      @window.destroy
      destroy unless destroyed?
    end

    def about_activate
      dialog = @builder["aboutdialog1"]
      dialog.run
      dialog.hide
    end
  end
end


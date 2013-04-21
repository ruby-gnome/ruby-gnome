# Copyright (c) 2013 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Spinner

GtkSpinner allows to show that background activity is on-going.
=end
require 'common'

module Demo
  class Spinner < Gtk::Dialog
    def initialize
      super(:title => 'Spinner',
            :parent => nil,
            :flags => nil,
            :buttons => [[:close, :none]])

      signal_connect(:response) {self.destroy}
      signal_connect(:destroy) {self.destroy}

      self.resizable = false
      
      vbox = Gtk::Box.new :vertical, 5

      self.content_area.pack_start vbox, :expand => true, :fill => true, :padding => 0
      vbox.border_width = 5

      # Sensitive
      hbox = Gtk::Box.new :horizontal, 5
      @spinner_sensitive = Gtk::Spinner.new
      hbox.add @spinner_sensitive
      hbox.add Gtk::Entry.new
      vbox.add hbox

      # Disabled
      hbox = Gtk::Box.new :horizontal, 5
      @spinner_insensitive = Gtk::Spinner.new
      hbox.add @spinner_insensitive
      hbox.add Gtk::Entry.new
      vbox.add hbox
      hbox.sensitive = false

      button = Gtk::Button.new :stock_id => :media_play
      button.signal_connect(:clicked) do
        @spinner_sensitive.start
        @spinner_insensitive.start
      end
      vbox.add button

      button = Gtk::Button.new :stock_id => :media_stop
      button.signal_connect(:clicked) do
        @spinner_sensitive.stop
        @spinner_insensitive.stop
      end
      vbox.add button
    end
  end
end
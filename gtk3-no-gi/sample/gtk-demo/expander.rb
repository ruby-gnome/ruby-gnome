# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
# $Id: expander.rb,v 1.2 2005/02/25 17:09:25 kzys Exp $
=begin
= Expander (Expander)

GtkExpander allows to provide additional content that is initially hidden.
This is also known as "disclosure triangle".
=end

require 'common'

module Demo
  class Expander < Gtk::Dialog
    def initialize
      # as opposed to the GTK2 dialog, the buttons have to be enclodes by brackeds
      # together with their ResponseType AND all groups have to bracketed together
      super(:title => 'GtkExpander',
            :parent => nil,
            :flags => 0,
            :buttons => [[Gtk::Stock::CLOSE, Gtk::ResponseType::NONE]])
      self.resizable = false

      signal_connect('response') do
        self.destroy
      end

      vbox = Gtk::Box.new(:vertical, 5)
      self.child.pack_start(vbox, :expand => true, :fill => true)
      vbox.border_width = 5

      label = Gtk::Label.new('Expander demo. CLick on the triangle for details.')
      vbox.pack_start(label, :expand => false, :fill => false)

      # Create the expander
      expander = Gtk::Expander.new('Details')
      vbox.pack_start(expander, :expand => false, :fill => false)

      expander.add(Gtk::Label.new('Details can be shown or hidden.'))
    end
  end
end

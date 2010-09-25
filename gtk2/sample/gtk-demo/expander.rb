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
      super('GtkExpander',
	    nil, # parent
	    0,
	    [Gtk::Stock::CLOSE, Gtk::Dialog::RESPONSE_NONE])
      self.resizable = false

      signal_connect('response') do
        self.destroy
      end

      vbox = Gtk::VBox.new(false, 5)
      self.vbox.pack_start(vbox, true, true)
      vbox.border_width = 5

      label = Gtk::Label.new('Expander demo. CLick on the triangle for details.')
      vbox.pack_start(label, false, false)

      # Create the expander
      expander = Gtk::Expander.new('Details')
      vbox.pack_start(expander, false, false)

      expander.add(Gtk::Label.new('Details can be shown or hidden.'))
    end
  end
end

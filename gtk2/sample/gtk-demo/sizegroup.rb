# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: sizegroup.rb,v 1.5 2005/02/12 23:02:43 kzys Exp $
=begin
= Size Groups

Gtk::SizeGroup provides a mechanism for grouping a number of
widgets together so they all request the same amount of space.
This is typically useful when you want a column of widgets to
have the same size, but you can't use a Gtk::Table widget.

Note that size groups only affect the amount of space requested,
not the size that the widgets finally receive. If you want the
widgets in a Gtk::SizeGroup to actually be the same size, you need
to pack them in such a way that they get the size they request
and not more. For example, if you are packing your widgets
into a table, you would not include the Gtk::FILL flag.
=end
require 'common'

module Demo
  class SizeGroup < Gtk::Dialog
    def initialize
      super('GtkSizeGroup', nil, 0,
	    [Gtk::Stock::CLOSE, Gtk::Dialog::RESPONSE_NONE])

      color_options = %w(Red Green Blue)
      dash_options = %w(Solid Dashed Dotted)
      end_options = %w(Square Round Arrow)

      set_resizable(false)

      signal_connect('response') do
	destroy
      end

      vbox = Gtk::VBox.new(false, 5)
      self.vbox.pack_start(vbox, true, true, 0)
      vbox.set_border_width(5)

      size_group = Gtk::SizeGroup.new(Gtk::SizeGroup::HORIZONTAL)

      ## Create one frame holding color options
      frame = Gtk::Frame.new('Color Options')
      vbox.pack_start(frame, true, true, 0)

      table = Gtk::Table.new(2, 2, false)
      table.set_border_width(5)
      table.set_row_spacings(5)
      table.set_column_spacings(10)
      frame.add(table)

      add_row(table, 0, size_group, '_Foreground', color_options)
      add_row(table, 1, size_group, '_Background', color_options)

      ## And another frame holding line style options
      frame = Gtk::Frame.new('Line Options')
      vbox.pack_start(frame, false, false, 0)

      table = Gtk::Table.new(2, 2, false)
      table.set_border_width(5)
      table.set_row_spacings(5)
      table.set_column_spacings(10)
      frame.add(table)

      add_row(table, 0, size_group, '_Dashing', dash_options)
      add_row(table, 1, size_group, '_Line ends', end_options)

      # And a check button to turn grouping on and off
      check_button = Gtk::CheckButton.new('_Enable grouping', true)
      vbox.pack_start(check_button, false, false, 0)

      check_button.set_active(true)
      check_button.signal_connect('toggled', size_group) do |check_button, size_group|
	new_mode = if check_button.active?
		     Gtk::SizeGroup::HORIZONTAL
		   else
		     Gtk::SizeGroup::VERTICAL
		   end
	size_group.set_mode(new_mode)
      end
    end

    def add_row(table, row, size_group, label_text, options)
      label = Gtk::Label.new(label_text, true)
      label.set_alignment(0, 1)
      table.attach(label,
		   0, 1,                    row, row + 1,
		   Gtk::EXPAND | Gtk::FILL, 0,
		   0,                       0)

      option_menu = create_option_menu(options)
      label.set_mnemonic_widget(option_menu)
      size_group.add_widget(option_menu)
      table.attach(option_menu,
		   1, 2,                    row, row + 1,
		   0,                       0,
		   0,                       0)
    end

    def create_option_menu(strings)
      menu = Gtk::Menu.new

      strings.each do |str|
	menu_item = Gtk::MenuItem.new(str)
	menu_item.show

	menu.append(menu_item)
      end

      option_menu = Gtk::OptionMenu.new
      option_menu.set_menu(menu)

      return option_menu
    end
  end
end

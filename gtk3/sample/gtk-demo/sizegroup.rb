# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Size Groups

 GtkSizeGroup provides a mechanism for grouping a number of
 widgets together so they all request the same amount of space.
 This is typically useful when you want a column of widgets to
 have the same size, but you can't use a GtkTable widget.

 Note that size groups only affect the amount of space requested,
 not the size that the widgets finally receive. If you want the
 widgets in a GtkSizeGroup to actually be the same size, you need
 to pack them in such a way that they get the size they request
 and not more. For example, if you are packing your widgets
 into a table, you would not include the GTK_FILL flag.
=end
module SizegroupDemo
  def self.run_demo(main_window)
    color_options = %w(Red Green Blue)
    dash_options = %w(Solid Dashed Dotted)
    end_options = %w(Square Round Double Arrow)

    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Size Groups")
    window.set_resizable(false)

    vbox = Gtk::Box.new(:vertical, 5)
    window.add(vbox)
    vbox.set_border_width(5)

    size_group = Gtk::SizeGroup.new(:horizontal)

    # Create one frame holding color options
    frame = Gtk::Frame.new("Color Options")
    vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)

    table = Gtk::Grid.new
    table.set_border_width(5)
    table.set_row_spacing(5)
    table.set_column_spacing(10)
    frame.add(table)

    add_row(table, 0, size_group, "_Foreground", color_options)
    add_row(table, 1, size_group, "_Background", color_options)

    # And another frame holding line style options
    frame = Gtk::Frame.new("Line options")
    vbox.pack_start(frame, :expand => false, :fill => false, :padding => 0)

    table = Gtk::Grid.new
    table.set_border_width(5)
    table.set_row_spacing(5)
    table.set_column_spacing(10)
    frame.add(table)

    add_row(table, 0, size_group, "_Dashing", dash_options)
    add_row(table, 1, size_group, "_Line ends", end_options)

    # And a check button to turn grouping on and off
    check_button = Gtk::CheckButton.new("_Enable grouping")
    check_button.set_use_underline(true)
    vbox.pack_start(check_button, :expand => false, :fill => false, :padding => 0)

    check_button.signal_connect("toggled") do |widget|
      if widget.active?
        size_group.set_mode(:horizontal)
      else
        size_group.set_mode(:none)
      end
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.add_row(table, row, size_group, label_text, options)
    label = Gtk::Label.new(label_text, :use_underline => true)
    label.set_halign(:start)
    label.set_valign(:baseline)
    label.set_hexpand(true)
    table.attach(label, 0, row, 1, 1)

    combo_box = create_combo_box(options)
    label.set_mnemonic_widget(combo_box)
    combo_box.set_halign(:end)
    combo_box.set_valign(:baseline)
    size_group.add_widget(combo_box)
    table.attach(combo_box, 1, row, 1, 1)
  end

  def self.create_combo_box(options)
    combo_box = Gtk::ComboBoxText.new
    options.each do |o|
      combo_box.append_text(o)
    end
    combo_box.set_active(0)
    combo_box
  end
end

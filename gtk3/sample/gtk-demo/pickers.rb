# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Pickers

These widgets are mainly intended for use in preference dialogs.
They allow to select colors, fonts, files, directories and applications.
=end
module PickersDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Pickers")
    window.set_border_width(10)

    table = Gtk::Grid.new
    table.set_row_spacing(3)
    table.set_column_spacing(10)
    window.add(table)

    label = Gtk::Label.new("Color:")
    label.set_halign(:start)
    label.set_valign(:center)
    label.set_hexpand(true)
    picker = Gtk::ColorButton.new
    table.attach(label, 0, 0, 1, 1)
    table.attach(picker, 1, 0, 1, 1)

    label = Gtk::Label.new("Font:")
    label.set_halign(:start)
    label.set_valign(:center)
    label.set_hexpand(true)
    picker = Gtk::FontButton.new
    table.attach(label, 0, 1, 1, 1)
    table.attach(picker, 1, 1, 1, 1)

    label = Gtk::Label.new("File:")
    label.set_halign(:start)
    label.set_valign(:center)
    label.set_hexpand(true)
    picker = Gtk::FileChooserButton.new("Pick a file", :open)
    table.attach(label, 0, 2, 1, 1)
    table.attach(picker, 1, 2, 1, 1)

    label = Gtk::Label.new("Folder:")
    label.set_halign(:start)
    label.set_valign(:center)
    label.set_hexpand(true)
    picker = Gtk::FileChooserButton.new("Pick a folder", :select_folder)
    table.attach(label, 0, 3, 1, 1)
    table.attach(picker, 1, 3, 1, 1)

    label = Gtk::Label.new("Mail:")
    label.set_halign(:start)
    label.set_valign(:center)
    label.set_hexpand(true)
    picker = Gtk::AppChooserButton.new("x-scheme-handler/mailto")
    picker.set_show_dialog_item(true)
    table.attach(label, 0, 4, 1, 1)
    table.attach(picker, 1, 4, 1, 1)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end

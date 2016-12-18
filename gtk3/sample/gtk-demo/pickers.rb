# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Pickers

These widgets are mainly intended for use in preference dialogs.
They allow to select colors, fonts, files, directories and applications.
=end
class PickersDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Pickers"

    initialize_grid

    label = generate_label("Color:")
    picker = Gtk::ColorButton.new
    @table.attach(label, 0, 0, 1, 1)
    @table.attach(picker, 1, 0, 1, 1)

    label = generate_label("Font:")
    picker = Gtk::FontButton.new
    @table.attach(label, 0, 1, 1, 1)
    @table.attach(picker, 1, 1, 1, 1)

    label = generate_label("File:")
    picker = Gtk::FileChooserButton.new("Pick a file", :open)
    picker.local_only = true
    @table.attach(label, 0, 2, 1, 1)
    @table.attach(picker, 1, 2, 1, 1)

    label = generate_label("Folder:")
    picker = Gtk::FileChooserButton.new("Pick a folder", :select_folder)
    @table.attach(label, 0, 3, 1, 1)
    @table.attach(picker, 1, 3, 1, 1)

    label = generate_label("Mail:")
    picker = Gtk::AppChooserButton.new("x-scheme-handler/mailto")
    picker.show_dialog_item = true
    @table.attach(label, 0, 4, 1, 1)
    @table.attach(picker, 1, 4, 1, 1)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def initialize_grid
    @table = Gtk::Grid.new
    @table.row_spacing = 3
    @table.column_spacing = 10
    @table.margin = 20
    @window.add(@table)
  end

  def generate_label(label)
    label = Gtk::Label.new(label)
    label.halign = :start
    label.valign = :center
    label.hexpand = :true
    label
  end
end

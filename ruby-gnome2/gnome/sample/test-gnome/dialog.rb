=begin header

  dialog.rb - Dialog test rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

Original Copyright:
 
  Author : Richard Hestilow <hestgray@ionet.net>

  Copyright (C) 1998 Free Software Foundation

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

=end

require 'sample'

MODAL = 0
JUST_HIDE = 1
CLICK_CLOSES = 2
EDITABLE_ENTERS = 3

class DialogSample < SampleApp
  def initialize
    super(true,"testGNOME", "Dialog Boxes")

    @dialog = nil

    vbox = Gtk::VBox.new(false, Gnome::PAD)
    hbox = Gtk::HBox.new(false, Gnome::PAD)

    set_contents(vbox)

    vbox.pack_start(hbox, true, true, Gnome::PAD)

    labels = [ "Modal", "Hide don't destroy",
	"Close on click", "Editable enters" ]
    @settings = [ false , true, false, true ]

    for i in MODAL .. EDITABLE_ENTERS do
      toggle = Gtk::ToggleButton.new(labels[i])
      toggle.set_active(@settings[i])
      toggle.signal_connect("toggled", i) { |t,i| toggle_boolean(i) }
      hbox.pack_start(toggle, false, false, Gnome::PAD)
    end
    
    button = Gtk::Button.new("Create the dialog")
    button.signal_connect("clicked") { create_test_dialog }
    vbox.pack_end(button, false, false, Gnome::PAD)

    show_all
  end

  private
  def toggle_boolean(setme)
    current = @settings[setme]
    @settings[setme] = !current
  end

  private
  def create_test_dialog
    if @dialog then
      puts("Previous dialog was not destroyed, destroying...")
      @dialog.destroy
      @dialog = nil
    end

    @dialog = Gnome::Dialog.new("A Test Dialog",
			       Gnome::Stock::BUTTON_OK,
			       "Not a stock button",
			       Gnome::Stock::BUTTON_CANCEL)

    @dialog.set_parent(self)

    entry = Gtk::Entry.new
    button = Gtk::Button.new("gnome_dialog_run");

    button.signal_connect("clicked") { block_until_clicked }

    @dialog.signal_connect("destroy") { @dialog = nil }

    @dialog.vbox.pack_start(entry, true, true, Gnome::PAD)
    @dialog.vbox.pack_start(button, false, false, Gnome::PAD)

    if @settings[MODAL] then
      print("Modal... ")
      @dialog.set_modal(true)
    end
    if @settings[JUST_HIDE] then
      print("Close hides... ")
      @dialog.close_hides(true)
    end
    if @settings[CLICK_CLOSES] then
      print("Click closes... ")
      @dialog.set_close(true)
    end
    if @settings[EDITABLE_ENTERS] then
      print("Editable enters... ")
      @dialog.editable_enters(entry)
    end
    print("\n")

    @dialog.show_all
  end

  private
  def block_until_clicked
    button = @dialog.run
    printf("Modal run ended, button %d clicked\n", button)
  end

end

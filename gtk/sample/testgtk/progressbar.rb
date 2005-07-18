=begin header

  progressbar.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: progressbar.rb,v 1.9 2005/07/18 17:13:32 mutoh Exp $

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

Original Copyright:
 
  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

class ProgressBarSample < SampleDialog

  def initialize
    super("Gtk::ProgressBar")
    set_border_width(10)

    vbox1 = Gtk::VBox.new(false, 5)
    vbox1.border_width = 10
    vbox.add(vbox1)

    frame = Gtk::Frame.new("Progress")
    vbox1.add(frame)

    vbox2 = Gtk::VBox.new(false, 5)
    frame.add(vbox2)

    align = Gtk::Alignment.new(0.5, 0.5, 0, 0)
    vbox2.pack_start(align, FALSE, FALSE, 5)

    @pbar = Gtk::ProgressBar.new
    align.add(@pbar)
    @timer = Gtk.timeout_add(100){progress_timeout}

    align = Gtk::Alignment.new(0.5, 0.5, 0, 0)
    vbox2.pack_start(align, false, false, 5);

    hbox = Gtk::HBox.new(false, 5)
    align.add(hbox)
    label = Gtk::Label.new("Label updated by user :")
    hbox.add(label)
    @label = Gtk::Label.new("")
    hbox.add(@label)

    frame = Gtk::Frame.new("Options")
    vbox1.add(frame)

    vbox2 = Gtk::VBox.new(false, 5)
    frame.add(vbox2)

    tab = Gtk::Table.new(3, 2, false)
    vbox2.add(tab)

    label = Gtk::Label.new("Orientation :");
    tab.attach(label, 0, 1, 0, 1,
	       Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
	       5, 5)
    label.set_alignment(0, 0.5)

    @omenu1 = build_option_menu(
	[ OptionMenuItem.new("Left-Right", proc { |w| toggle_orientation(w) }),
	  OptionMenuItem.new("Right-Left", proc { |w| toggle_orientation(w) }),
	  OptionMenuItem.new("Bottom-Top", proc { |w| toggle_orientation(w) }),
	  OptionMenuItem.new("Top-Bottom", proc { |w| toggle_orientation(w) })
	], 0)
    hbox = Gtk::HBox.new(false, 0)
    tab.attach(hbox, 1, 2, 0, 1,
		Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
		5, 5);
    hbox.add(@omenu1)
       
    check = Gtk::CheckButton.new("Show text")
    check.signal_connect("clicked") do |w| toggle_show_text(w) end
    tab.attach(check, 0, 1, 1, 2,
		Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
		5, 5)

    hbox = Gtk::HBox.new(false, 0)
    tab.attach(hbox, 1, 2, 1, 2,
	       Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
	       5, 5)

    label = Gtk::Label.new("Text align :")
    tab.attach(label, 0, 1, 2, 3,
 		Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
		5, 5)
    label.set_alignment(0, 0.5)

    hbox = Gtk::HBox.new(false, 0)
    tab.attach(hbox, 1, 2, 2, 3,
		Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
		5, 5)

    label = Gtk::Label.new("x :")
    hbox.pack_start(label, false, true, 5)
      
    adj = Gtk::Adjustment.new(0.5, 0, 1, 0.1, 0.1, 0)
    @x_align_spin = Gtk::SpinButton.new(adj, 0, 1)
    adj.signal_connect("value_changed"){adjust_align}
    hbox.pack_start(@x_align_spin, false, true, 0)
    @x_align_spin.sensitive = false

    label = Gtk::Label.new("y :")
    hbox.pack_start(label, false, true, 5)

    adj = Gtk::Adjustment.new(0.5, 0, 1, 0.1, 0.1, 0)
    @y_align_spin = Gtk::SpinButton.new(adj, 0, 1)
    adj.signal_connect("value_changed"){adjust_align}
    hbox.pack_start(@y_align_spin, false, true, 0)
    @y_align_spin.sensitive = false

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){
      Gtk::timeout_remove(@timer)
      destroy
    }
    button.can_default = true
    self.action_area.add(button)
    button.grab_default
  end

  private
  def toggle_orientation(rmitem)
    return unless rmitem.mapped?

    i = 3
    group = rmitem.group
    group.each do |g|
      break if g.active?
      i -= 1
    end

    @pbar.orientation = i
  end

  private
  def progress_timeout
    new_val = @pbar.fraction + 0.05
    new_val = 0.0 if new_val > 1.0
    @pbar.fraction = new_val
    @label.text = "#{(new_val * 100).to_i}"
    true
  end

  private
  def toggle_show_text(cbutton)
    @pbar.show_text = cbutton.active?
    @x_align_spin.sensitive = cbutton.active?
    @y_align_spin.sensitive = cbutton.active?
  end

  private
  def adjust_align
    @pbar.text_xalign = @x_align_spin.value
    @pbar.text_yalign = @y_align_spin.value
  end

end

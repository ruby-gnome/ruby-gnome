=begin header

  spinbutton.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: spinbutton.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>

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

# #
# # GtkSpinButton
# #
class SpinButtonSample < SampleWindow

# static GtkWidget *spinner1

  def toggle_snap(button, spin)
    spin.set_snap_to_ticks(button.active?)
  end

  def toggle_numeric(button, spin)
    spin.set_numeric(button.active?)
  end

  def change_digits(spin)
    @spinner1.set_digits(spin.value_as_int)
  end

  def get_value(label, data)
#   gchar buf[32]
#   GtkLabel *label
#   GtkSpinButton *spin

    spin = @spinner1
    #label = button.get_user_data
    if (data == 1)
      buf = sprintf("%d", spin.value_as_int)
    else
      buf = sprintf("%0.*f", spin.digits, spin.value)
    end
    label.set_text(buf)
  end

  def initialize
 # create_spins ()
#   static GtkWidget *window = nil
#   GtkWidget *frame
#   GtkWidget *hbox
#   GtkWidget *main_vbox
#   GtkWidget *vbox
#   GtkWidget *vbox2
#   GtkWidget *spinner2
#   GtkWidget *spinner
#   GtkWidget *button
#   GtkWidget *label
#   GtkWidget *val_label
#   GtkAdjustment *adj

    super("GtkSpinButton")
      
    main_vbox = Gtk::VBox::new(false, 5)
    main_vbox.set_border_width(10)
    add(main_vbox) 
     
    frame = Gtk::Frame::new("Not accelerated")
    main_vbox.pack_start(frame, true, true, 0)
      
    vbox = Gtk::VBox::new(false, 0)
    vbox.set_border_width(5)
    frame.add(vbox)
      
    # Day, month, year spinners 
      
    hbox = Gtk::HBox::new(false, 0)
    vbox.pack_start(hbox, true, true, 5)
      
    vbox2 = Gtk::VBox::new(false, 0)
    hbox.pack_start(vbox2, true, true, 5)
      
    label = Gtk::Label::new("Day :")
    label.set_alignment(0, 0.5)
    vbox2.pack_start(label, false, true, 0)
      
    adj = Gtk::Adjustment::new(1.0, 1.0, 31.0, 1.0, 5.0, 0.0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    spinner.set_wrap(true)
    #spinner.set_shadow_type(Gtk::SHADOW_OUT)
    vbox2.pack_start(spinner, false, true, 0)

    vbox2 = Gtk::VBox::new(false, 0)
    hbox.pack_start(vbox2, true, true, 5)
      
    label = Gtk::Label::new("Month :")
    label.set_alignment(0, 0.5)
    vbox2.pack_start(label, false, true, 0)
      
    adj = Gtk::Adjustment::new(1.0, 1.0, 12.0, 1.0, 5.0, 0.0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    spinner.set_wrap(true)
    #spinner.set_shadow_type(Gtk::SHADOW_ETCHED_IN)
    vbox2.pack_start(spinner, false, true, 0)
      
    vbox2 = Gtk::VBox::new(false, 0)
    hbox.pack_start(vbox2, true, true, 5)

    label = Gtk::Label::new("Year :")
    label.set_alignment(0, 0.5)
    vbox2.pack_start(label, false, true, 0)

    adj = Gtk::Adjustment::new(1998.0, 0.0, 2100.0, 1.0, 100.0, 0.0)
    spinner = Gtk::SpinButton::new(adj, 0, 0)
    spinner.set_wrap(true)
    #spinner.set_shadow_type(Gtk::SHADOW_IN)
    spinner.set_size_request(55, -1)
    vbox2.pack_start(spinner, false, true, 0)

    frame = Gtk::Frame::new("Accelerated")
    main_vbox.pack_start(frame, true, true, 0)
  
    vbox = Gtk::VBox::new(false, 0)
    vbox.set_border_width(5)
    frame.add(vbox)
      
    hbox = Gtk::HBox::new(false, 0)
    vbox.pack_start(hbox, false, true, 5)
      
    vbox2 = Gtk::VBox::new(false, 0)
    hbox.pack_start(vbox2, true, true, 5)
      
    label = Gtk::Label::new("Value :")
    label.set_alignment(0, 0.5)
    vbox2.pack_start(label, false, true, 0)

    adj = Gtk::Adjustment::new(0.0, -10000.0, 10000.0, 0.5, 100.0, 0.0)
    @spinner1 = Gtk::SpinButton::new(adj, 1.0, 2)
    @spinner1.set_wrap(true)
    @spinner1.set_size_request(100, -1)
    @spinner1.set_update_policy(Gtk::SpinButton::UPDATE_ALWAYS)
    vbox2.pack_start(@spinner1, false, true, 0)

    vbox2 = Gtk::VBox::new(false, 0)
    hbox.pack_start(vbox2, true, true, 5)

    label = Gtk::Label::new("Digits :")
    label.set_alignment(0, 0.5)
    vbox2.pack_start(label, false, true, 0)

    adj = Gtk::Adjustment::new(2, 1, 5, 1, 1, 0)
    spinner2 = Gtk::SpinButton::new(adj, 0.0, 0) 
    spinner2.set_wrap(true)
    adj.signal_connect("value_changed") do change_digits(spinner2) end
    vbox2.pack_start(spinner2, false, true, 0)
    
    hbox = Gtk::HBox::new(false, 0)
    vbox.pack_start(hbox, false, true, 5)

    button = Gtk::CheckButton::new("Snap to 0.5-ticks")
    button.signal_connect("clicked") do toggle_snap(button, @spinner1) end
    vbox.pack_start(button, true, true, 0)
    button.set_active(true)

    button2 = Gtk::CheckButton::new("Numeric only input mode")
    button2.signal_connect("clicked") do toggle_numeric(button2, @spinner1) end
    vbox.pack_start(button2, true, true, 0)
    button2.set_active(true)

    val_label = Gtk::Label::new("")

    hbox = Gtk::HBox::new(false, 0)
    vbox.pack_start(hbox, false, true, 5)

    button3 = Gtk::Button::new("Value as Int")
    #button.set_user_data(val_label)
    button3.signal_connect("clicked") do get_value(val_label, 1) end
    hbox.pack_start(button3, true, true, 5)

    button4 = Gtk::Button::new("Value as Float")
    #button.set_user_data(val_label)
    button4.signal_connect("clicked") do get_value(val_label, 2) end
    hbox.pack_start(button4, true, true, 5)

    vbox.pack_start(val_label, true, true, 0)
    val_label.set_text("0")

    hbox = Gtk::HBox::new(false, 0)
    main_vbox.pack_start(hbox, false, true, 0)
  
    button5 = Gtk::Button::new("Close")
    button5.signal_connect("clicked") do destroy end
    hbox.pack_start(button5, true, true, 5)
  end
end

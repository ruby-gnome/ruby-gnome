#!/usr/bin/env ruby
=begin
  status_icon.rb - Gtk::StatusIcon sample.

  This sample refers gtk/tests/teststatusicon.c of GTK+-2.10.x.

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: statusicon.rb,v 1.1 2006/11/17 18:12:41 mutoh Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 10, 0)
  puts "This sample requires GTK+ 2.10.0 or later"
  puts str
  exit
end

class StatusIconSample < Gtk::StatusIcon
  STATUS_INFO = 0
  STATUS_QUESTION = 1

  def initialize
    super
    @status = STATUS_INFO
    update_icon

    set_blinking(true)
    signal_connect("activate"){
      icon_activated
    }
    signal_connect("popup-menu"){|w, button, activate_time|
      menu = Gtk::Menu.new
      checkmenuitem = Gtk::CheckMenuItem.new("Blink")
      checkmenuitem.active = blinking?
      checkmenuitem.signal_connect("activate"){|w|
        set_blinking(w.active?)
      }
      menu.append(checkmenuitem)
      
      menuitem = Gtk::MenuItem.new("Quit")
      menuitem.signal_connect("activate"){
        set_visible(false)
        Gtk.main_quit
      }
      menu.append(menuitem)
      menu.show_all
      menu.popup(nil, nil, button, activate_time)
    }

    timeout_func
  end

  def update_icon
    if @status == STATUS_INFO
      set_icon_name(Gtk::Stock::DIALOG_INFO)
      set_tooltip("Some Information ...")
    else
      set_icon_name(Gtk::Stock::DIALOG_QUESTION)
      set_tooltip("Some Question ...")
    end
  end

  def timeout_func
    GLib::Timeout.add(2000) {
      if @status == STATUS_INFO
        @status = STATUS_QUESTION
      else
        @status = STATUS_INFO
      end
      update_icon
      true
    }
  end

  def icon_activated
    unless @dialog
      @dialog = Gtk::MessageDialog.new(nil, 0,
                                       Gtk::MessageDialog::QUESTION,
                                       Gtk::MessageDialog::BUTTONS_CLOSE,
                                       "You wanna test the status icon?")
      @dialog.window_position = Gtk::Window::POS_CENTER
      
      @dialog.signal_connect("response"){ @dialog.hide }
      @dialog.signal_connect("delete_event"){ @dialog.hide_on_delete }
      
      toggle = Gtk::ToggleButton.new("_Show the icon")
      @dialog.vbox.pack_end(toggle, true, true, 6)
      toggle.active = visible?
      toggle.signal_connect("toggled"){|w|
        set_visible(w.active?)
      }

      toggle = Gtk::ToggleButton.new("_Blink the icon")
      @dialog.vbox.pack_end(toggle, true, true, 6)
      toggle.active = blinking?
      toggle.signal_connect("toggled"){|w|
        set_blinking(w.active?)
      }
 
      toggle = Gtk::ToggleButton.new("_Change the icon")
      @dialog.vbox.pack_end(toggle, true, true, 6)
      toggle.active = (@timeout != 0)
      toggle.signal_connect("toggled"){ 
        if @timeout and @timeout > 0
          GLib::Source.remove(@timeout)
          @timeout = 0
        else 
          @timeout = timeout_func
        end
      }
    end
    @dialog.show_all
    @dialog.present
  end

end

StatusIconSample.new
  
Gtk.main

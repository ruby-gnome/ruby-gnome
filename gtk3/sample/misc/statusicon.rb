#!/usr/bin/env ruby
=begin
  status_icon.rb - Gtk::StatusIcon sample.

  This sample refers gtk/tests/teststatusicon.c of GTK+-2.10.x.

  Copyright (c) 2006-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

unless Gtk::Version.or_later?(3, 10, 7)
  puts "This sample requires GTK+ 3.10.7 or later"
  exit
end

class StatusIconSample < Gtk::StatusIcon
  STATUS_INFO = 0
  STATUS_QUESTION = 1

  def initialize
    super
    @status = STATUS_INFO
    update_icon

    signal_connect("activate") {icon_activated}
    signal_connect("popup-menu") do |w, button, activate_time|
      menu = Gtk::Menu.new

      menuitem = Gtk::MenuItem.new(:label => "Quit")
      menuitem.signal_connect("activate") do
        set_visible(false)
        Gtk.main_quit
      end
      menu.append(menuitem)
      menu.show_all
      menu.popup(nil, nil, button, activate_time)
    end

    @timeout = timeout_func
  end

  def update_icon
    if @status == STATUS_INFO
      self.stock = Gtk::Stock::DIALOG_INFO
      set_tooltip_text("Some Information ...")
    else
      self.stock = Gtk::Stock::DIALOG_QUESTION
      set_tooltip_text("Some Question ...")
    end
  end

  def timeout_func
    GLib::Timeout.add(2000) do
      if @status == STATUS_INFO
        @status = STATUS_QUESTION
      else
        @status = STATUS_INFO
      end
      update_icon
      true
    end
  end

  def icon_activated
    unless @dialog
      @dialog = Gtk::MessageDialog.new(:parent => nil, :flags => 0,
                                       :type => Gtk::MessageType::QUESTION,
                                       :buttons_type => Gtk::ButtonsType::CLOSE,
                                       :message => "You wanna test the status icon?")
      @dialog.window_position = Gtk::WindowPosition::CENTER

      @dialog.signal_connect("response") {@dialog.hide}
      @dialog.signal_connect("delete_event") {@dialog.hide_on_delete}

      toggle = Gtk::ToggleButton.new(:label => "_Show the icon", :use_underline => true)
      @dialog.child.pack_end(toggle, :expand => true, :fill => true, :padding => 6)
      toggle.active = visible?
      toggle.signal_connect("toggled") do |w|
        set_visible(w.active?)
      end

      toggle = Gtk::ToggleButton.new(:label => "_Change the icon", :use_underline => true)
      @dialog.child.pack_end(toggle, :expand => true, :fill => true, :padding => 6)
      toggle.active = (@timeout != 0)
      toggle.signal_connect("toggled") do
        if @timeout and @timeout > 0
          GLib::Source.remove(@timeout)
          @timeout = 0
        else
          @timeout = timeout_func
        end
      end
    end
    @dialog.show_all
    @dialog.present
  end
end

StatusIconSample.new

Gtk.main

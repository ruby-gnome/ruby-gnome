#!/usr/local/bin/ruby

=begin header

  testgtk.rb - testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team

  $Id: testgtk.rb,v 1.11 2004/08/15 14:38:17 mutoh Exp $

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

require 'gtk2'
require 'sample'
require 'button'
require 'checkbutton'
require 'togglebutton'
require 'radiobutton'
require 'buttonbox'
require 'toolbar'
require 'statusbar'
require 'handlebox'
require 'reparent'
require 'pixmap'
require 'tooltips'
require 'labels'
require 'layout'
require 'menu'
require 'scrolledwindow'
require 'entry'
require 'spinbutton'
require 'cursors'
require 'liststore'
require 'treestore'
require 'colorselect'
require 'dialog'
require 'range'
require 'rulers'
require 'notebook'
require 'panes'
require 'dnd'
require 'shapedwindow'
require 'wmhints'
require 'progressbar'
require 'colorpreview'
require 'graypreview'
require 'selection'
require 'gammacurve'
require 'scroll'
require 'timeout'
require 'idle'
require 'mainloop'
require 'savedposition'
require 'filesel'
require 'fontselection'

#
# Main Window
#
def create_main_window
  buttons = [
    ["button box", ButtonBoxSample],
    ["buttons", ButtonSample],
    ["check buttons", CheckButtonSample],
    ["color selection", ColorSelectionSample],
    ["cursors", nil],
    ["dialog", DialogSample],
    #["dnd", nil],
    ["entry", EntrySample],
    ["event watcher", nil],  #create_event_watcher
    ["file selection", FileSelectionSample],
    ["font selection", FontSelectionSample],
    ["gamma curve", GammaCurveSample],
    ["handle box", nil],
    ["item factory", nil],   #create_item_factory
    ["labels", LabelSample],
    ["layout", LayoutSample],
#    ["liststore", ListStoreSample],
    ["liststore", nil],
    ["menus", MenuSample],
    ["modal window", nil],   #create_modal_window
    ["notebook", NotebookSample],
    ["panes", nil],
    ["pixmap", PixmapSample],
    ["preview color", nil],
    ["preview gray", nil],
    ["progress bar", ProgressBarSample],
    ["radio buttons", RadioButtonSample],
    ["range controls", RangeSample],
    ["rc file", nil], #create_rc_file
    ["reparent", ReparentSample],
    ["rulers", RulerSample],
    ["saved position", SavedPositionSample],
    ["scrolled windows", ScrolledWindowSample],
    ["shapes", ShapesSample],
    ["spinbutton", SpinButtonSample],
    ["statusbar", StatusbarSample],
    ["test idle", nil],
    ["test mainloop", nil],
    ["test scrolling", nil],
    ["test selection", nil],
    ["test timeout", nil],
    ["toggle buttons", ToggleButtonSample],
    ["toolbar", ToolbarSample],
    ["tooltips", TooltipsSample],
    ["treestore", nil],
#    ["treestore", TreeStoreSample],
    ["WM hints", WMHintsSample],
  ]
  nbuttons = buttons.size

  window = Gtk::Window.new
  window.set_title($0)
  window.set_name("main window")
  window.set_default_size(200, 400)
  window.move(20, 20)

  window.signal_connect("delete_event") do 
    Gtk.main_quit 
    true 
  end

  box1 = Gtk::VBox.new(false, 0)
  window.add(box1)

  buffer =
    if Gtk::MICRO_VERSION > 0
      sprintf("Gtk+ v%d.%d.%d",
	      Gtk::MAJOR_VERSION,
	      Gtk::MINOR_VERSION,
	      Gtk::MICRO_VERSION)
    else
      sprintf("Gtk+ v%d.%d",
	      Gtk::MAJOR_VERSION,
	      Gtk::MICRO_VERSION)
    end

  label = Gtk::Label.new(buffer)
  box1.pack_start(label, false, false, 0)

  buffer =
      sprintf("Ruby/GTK2 v%d.%d.%d",
	      Gtk::BINDING_VERSION[0],
	      Gtk::BINDING_VERSION[1],
	      Gtk::BINDING_VERSION[2])
  label = Gtk::Label.new(buffer)
  box1.pack_start(label, false, false, 0)

  scrolled_window = Gtk::ScrolledWindow.new(nil, nil)
  scrolled_window.set_border_width(10)
  scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC)
  box1.pack_start(scrolled_window, true, true, 0)

  box2 = Gtk::VBox.new(false, 0)
  box2.set_border_width(10)
  scrolled_window.add_with_viewport(box2);
  box2.set_focus_vadjustment(scrolled_window.vadjustment)

  buttons.each do |title, sample_class|
    button = Gtk::Button.new(title)
    unless sample_class.nil?
      button.signal_connect("clicked") do |obj|
        sample_class.invoke
      end
    else
      button.set_sensitive(false)
    end
    box2.pack_start(button, true, true, 0)
  end

  separator = Gtk::HSeparator.new
  box1.pack_start(separator, false, true, 0)

  box2 = Gtk::VBox.new(false, 10)
  box2.set_border_width(10)
  box1.pack_start(box2, false, true, 0)

  button = Gtk::Button.new("close")
  button.signal_connect("clicked") do
    window.destroy
    Gtk.main_quit
  end
  box2.pack_start(button, true, true, 0)
  button.set_flags(Gtk::Widget::CAN_DEFAULT)
  button.grab_default

  window.show_all
end

def main
  srand
  Gtk::RC.parse("testgtkrc")
  #gtk_rc_add_default_file ("testgtkrc");
  #gdk_rgb_init ();

  # bindings test
  #GtkBindingSet *binding_set;
#  binding_set = gtk_binding_set_by_class (gtk_type_class (GTK_TYPE_WIDGET));
#  gtk_binding_entry_add_signal (binding_set,
#				'9', GDK_CONTROL_MASK | GDK_RELEASE_MASK,
#				"debug_msg",
#				1,
#				GTK_TYPE_STRING, "GtkWidgetClass <ctrl><release>9 test");

  create_main_window
  Gtk.main
end

if $DEBUG
  STDERR.sync = true
  # for GC (and thread) test
  Thread.start do
    loop do
      STDERR.print("+")
      GC.start
      sleep(1)
    end
  end
  STDERR.puts("#{$0}: started GC-thread for debugging.")

  # timeout
  Gtk.timeout_add(1000) do
    STDERR.print("*")
    true
  end

  # idle
#  Gtk.idle_add do
#    STDERR.print("?")
#    true
#  end

  # io-blocked thread test
#  Thread.start do
#    loop do
#      buf = STDIN.gets
#      STDERR.puts(buf)
#    end
#  end
end

Gtk.init
main
puts("#{$0}: done.")

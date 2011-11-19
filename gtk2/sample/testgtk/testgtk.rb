#!/usr/bin/env ruby

=begin header

  testgtk.rb - testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team

  $Id: testgtk.rb,v 1.16 2006/06/17 13:18:12 mutoh Exp $

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

$:.unshift(File.dirname(__FILE__))

require 'gtk2'
require 'sample'
require 'button'
require 'checkbutton'
require 'togglebutton'
require 'radiobutton'
require 'buttonbox'
require 'toolbar'
require 'statusbar'
require 'reparent'
require 'pixmap'
require 'tooltips'
require 'labels'
require 'layout'
require 'menu'
require 'scrolledwindow'
require 'entry'
require 'spinbutton'
require 'colorselect'
require 'dialog'
require 'range'
require 'rulers'
require 'notebook'
require 'shapedwindow'
require 'wmhints'
require 'progressbar'
require 'gammacurve'
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
    ["dialog", DialogSample],
    ["entry", EntrySample],
    ["file selection", FileSelectionSample],
    ["font selection", FontSelectionSample],
    ["gamma curve", GammaCurveSample],
    ["labels", LabelSample],
    ["layout", LayoutSample],
    ["menus", MenuSample],
    ["notebook", NotebookSample],
    ["pixmap", PixmapSample],
    ["progress bar", ProgressBarSample],
    ["radio buttons", RadioButtonSample],
    ["range controls", RangeSample],
    ["reparent", ReparentSample],
    ["rulers", RulerSample],
    ["saved position", SavedPositionSample],
    ["scrolled windows", ScrolledWindowSample],
    ["shapes", ShapesSample],
    ["spinbutton", SpinButtonSample],
    ["statusbar", StatusbarSample],
    ["toggle buttons", ToggleButtonSample],
    ["toolbar", ToolbarSample],
    ["tooltips", TooltipsSample],
    ["WM hints", WMHintsSample],
  ]
  nbuttons = buttons.size

  window = Gtk::Window.new
  window.name = "main window"

  window.set_default_size(200, 400)
  window.move(20, 20)

  window.signal_connect("destroy"){Gtk.main_quit}

  box1 = Gtk::VBox.new(false, 0)
  window.add(box1)

  label = Gtk::Label.new("Gtk+ v#{Gtk::MAJOR_VERSION}.#{Gtk::MINOR_VERSION}.#{Gtk::MICRO_VERSION}")
  box1.pack_start(label, false, false, 0)

  label = Gtk::Label.new("Ruby/GTK2 v#{Gtk::BINDING_VERSION.join(".")}")
  box1.pack_start(label, false, false, 0)

  scrolled_window = Gtk::ScrolledWindow.new(nil, nil)
  scrolled_window.border_width = 10
  scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC)
  box1.pack_start(scrolled_window, true, true, 0)

  box2 = Gtk::VBox.new(false, 0)
  box2.border_width = 10
  scrolled_window.add_with_viewport(box2);
  box2.focus_vadjustment = scrolled_window.vadjustment

  buttons.each do |title, sample_class|
    button = Gtk::Button.new(title)
    unless sample_class.nil?
      button.signal_connect("clicked"){|obj|
        sample_class.invoke
      }
    else
      button.sensitive = false
    end
    box2.pack_start(button, true, true, 0)
  end

  box1.pack_start(Gtk::HSeparator.new, false, true, 0)

  button = Gtk::Button.new("close")
  button.signal_connect("clicked") do
    window.destroy
    Gtk.main_quit
  end
  box1.pack_start(button, false, true, 5)
  window.show_all
end

def main
  srand
  Gtk::RC.parse("testgtkrc")
  create_main_window
  Gtk.main
end

if $DEBUG
  $stderr.sync = true
  Thread.start do
    loop do
      STDERR.print("+")
      GC.start
      sleep(1)
    end
  end
  $stderr.puts("#{$0}: started GC-thread for debugging.")

  # timeout
  Gtk.timeout_add(1000) do
    $stderr.print("*")
    true
  end
end

main
puts("#{$0}: done.")

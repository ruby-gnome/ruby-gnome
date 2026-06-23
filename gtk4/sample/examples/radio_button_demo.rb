#!/usr/bin/env ruby
#
# Copyright (C) 2024  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# Example from:
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application1/exampleapp.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application1/exampleappwin.c
# License: LGPL2.1-or-later


require "gtk4"

RADIO_UI = <<-TEXT
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <object class="GtkWindow" id="window1">
    <property name="title" translatable="yes">Radio Button Demo</property>
    <property name="default-width">300</property>
    <property name="resizable">True</property>
    <child>
      <object class="GtkBox">
      <property name="orientation">vertical</property>
      <property name="spacing">30</property>
      <property name="margin-top">40</property>
      <property name="margin-bottom">40</property>
      <property name="margin-start">40</property>
      <property name="margin-end">40</property>
        <child>
          <object class="GtkEntry" id="entryToggle"></object>
        </child>
        <child>
          <object class="GtkBox">
          <property name="spacing">15</property>  
            <child>
              <object class="GtkToggleButton" id="toggleCurrent">
                <property name="label">Current</property>
              </object>
            </child>
            <child>
              <object class="GtkToggleButton" id="toggleOpen">
                <property name="label">All Open</property>
                <property name="group">toggleCurrent</property>
              </object>
            </child>
            <child>
              <object class="GtkToggleButton" id="toggleDisk">
                <property name="label">On Disk</property>
                <property name="group">toggleCurrent</property>
                <property name="active">True</property>
              </object>
            </child>
          </object>
        </child>
        <child>
          <object class="GtkButton" id="buttonFetch">
          <property name="label">Fetch Button Values</property>
          </object>
        </child>
      </object>
    </child>
  </object> 
</interface>
TEXT


def on_activate(app)

  builder = Gtk::Builder.new(:string => RADIO_UI)
  builder[:window1].application = app

  # dispaly the active? property of buttons
  builder[:buttonFetch].signal_connect("clicked") do |button|
    builder[:entryToggle].text = builder[:toggleCurrent].active?.to_s + ",  " +
              builder[:toggleOpen].active?.to_s + ",  " + builder[:toggleDisk].active?.to_s 
  end

  builder[:window1].present 
end


app = Gtk::Application.new("io.github.ruby-gnome.gtk4.example.RadioButtons")
app.signal_connect("activate") do |a|
  on_activate(a)
end
app.run

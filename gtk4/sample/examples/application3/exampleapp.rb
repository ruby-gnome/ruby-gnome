#!/usr/bin/env ruby
#
# Copyright (C) 2023  Ruby-GNOME Project Team
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
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application3/exampleapp.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application3/exampleappwin.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application3/window.ui
# License: LGPL2.1-or-later

require "gtk4"

class ExampleAppWindow < Gtk::ApplicationWindow
  type_register
  class << self
    def init
      template = <<~TEMPLATE
      <?xml version="1.0" encoding="UTF-8"?>
      <interface>
        <template class="ExampleAppWindow" parent="GtkApplicationWindow">
          <property name="title" translatable="yes">Example Application</property>
          <property name="default-width">600</property>
          <property name="default-height">400</property>
          <child type="titlebar">
            <object class="GtkHeaderBar" id="header">
              <child type="title">
                <object class="GtkStackSwitcher" id="tabs">
                  <property name="stack">stack</property>
                </object>
              </child>
            </object>
          </child>
          <child>
            <object class="GtkBox" id="content_box">
              <property name="orientation">vertical</property>
              <child>
                <object class="GtkStack" id="stack"/>
              </child>
            </object>
          </child>
        </template>
      </interface>
      TEMPLATE
      set_template(data: template)
      bind_template_child("stack")
    end
  end

  def initialize(application)
    super(application: application)
  end

  def open(file)
    basename = file.basename
    scrolled = Gtk::ScrolledWindow.new
    scrolled.hexpand = true
    scrolled.vexpand = true
    view = Gtk::TextView.new
    view.editable = false
    view.cursor_visible = false
    scrolled.child = view
    stack.add_titled(scrolled, basename, basename)
    stream = file.read
    view.buffer.text = stream.read
  end
end

class ExampleApp < Gtk::Application
  def initialize
    super("org.gtk.exampleapp", :handles_open)

    signal_connect "activate" do |application|
      window = ExampleAppWindow.new(application)
      window.present
    end
    signal_connect "open" do |application, files, hint|
      window = application.windows[0] || ExampleAppWindow.new(application)
      files.each do |file|
        window.open(file)
      end
      window.present
    end
  end
end

app = ExampleApp.new

# Gtk::Application#run needs C style argv ([prog, arg1, arg2, ...,argn]).
# The ARGV ruby variable only contains the arguments ([arg1, arg2, ...,argb])
# and not the program name. We have to add it explicitly.

app.run([$PROGRAM_NAME] + ARGV)

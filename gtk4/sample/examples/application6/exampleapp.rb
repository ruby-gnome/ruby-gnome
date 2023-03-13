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
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application6/exampleapp.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application6/exampleappwin.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application6/exampleappprefs.c
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application6/window.ui
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application6/gears-menu.ui
# * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/examples/application6/prefs.ui
# License: LGPL2.1-or-later

# GSETTINGS_SCHEMA_DIR must be set before requiring "gtk4" gem because it is used in the GIO initialization.

if File.exist?(File.join(__dir__, "gschemas.compiled"))
  ENV["GSETTINGS_SCHEMA_DIR"] = __dir__
else
  raise %{gschemas.compiled doesn't exist. Run "rake" to generate it.}
end

require "gtk4"

class ExampleAppPrefs < Gtk::Dialog
  type_register
  class << self
    def init
      template = <<~TEMPLATE
      <?xml version="1.0" encoding="UTF-8"?>
      <interface>
        <template class="ExampleAppPrefs" parent="GtkDialog">
          <property name="title" translatable="yes">Preferences</property>
          <property name="resizable">0</property>
          <property name="modal">1</property>
          <child internal-child="content_area">
            <object class="GtkBox" id="content_area">
              <child>
                <object class="GtkGrid" id="grid">
                  <property name="margin-start">12</property>
                  <property name="margin-end">12</property>
                  <property name="margin-top">12</property>
                  <property name="margin-bottom">12</property>
                  <property name="row-spacing">12</property>
                  <property name="column-spacing">12</property>
                  <child>
                    <object class="GtkLabel" id="fontlabel">
                      <property name="label">_Font:</property>
                      <property name="use-underline">1</property>
                      <property name="mnemonic-widget">font</property>
                      <property name="xalign">1</property>
                      <layout>
                        <property name="column">0</property>
                        <property name="row">0</property>
                      </layout>
                    </object>
                  </child>
                  <child>
                    <object class="GtkFontButton" id="font">
                      <layout>
                        <property name="column">1</property>
                        <property name="row">0</property>
                      </layout>
                    </object>
                  </child>
                  <child>
                    <object class="GtkLabel" id="transitionlabel">
                      <property name="label">_Transition:</property>
                      <property name="use-underline">1</property>
                      <property name="mnemonic-widget">transition</property>
                      <property name="xalign">1</property>
                      <layout>
                        <property name="column">0</property>
                        <property name="row">1</property>
                      </layout>
                    </object>
                  </child>
                  <child>
                    <object class="GtkComboBoxText" id="transition">
                      <items>
                        <item translatable="yes" id="none">None</item>
                        <item translatable="yes" id="crossfade">Fade</item>
                        <item translatable="yes" id="slide-left-right">Slide</item>
                      </items>
                      <layout>
                        <property name="column">1</property>
                        <property name="row">1</property>
                      </layout>
                    </object>
                  </child>
                </object>
              </child>
            </object>
          </child>
        </template>
      </interface>
      TEMPLATE
      set_template(data: template)
      bind_template_child("font")
      bind_template_child("transition")
    end
  end

  def initialize(win)
    # The original C program sets use-header-bar property to TRUE instead of 1.
    # But the property type is int, not gboolean.
    # Therefore, the property value must be 1, not `true` here.
    super('transient-for': win, 'use-header-bar': 1)
    settings = Gio::Settings.new("org.gtk.exampleapp")
    settings.bind("font", font, "font", :default)
    settings.bind("transition", transition, "active-id", :default)
  end
end

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
              <child type="end">
                <object class="GtkMenuButton" id="gears">
                  <property name="direction">none</property>
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
      bind_template_child("gears")
    end
  end

  def initialize(application)
    menu_ui = <<~MENU
    <?xml version="1.0" encoding="UTF-8"?>
    <interface>
      <menu id="menu">
        <section>
          <item>
            <attribute name="label" translatable="yes">_Preferences</attribute>
            <attribute name="action">app.preferences</attribute>
          </item>
        </section>
        <section>
          <item>
            <attribute name="label" translatable="yes">_Quit</attribute>
            <attribute name="action">app.quit</attribute>
          </item>
        </section>
      </menu>
    </interface>
    MENU
    super(application: application)
    builder = Gtk::Builder.new(string: menu_ui)
    gears.menu_model = builder["menu"]
    @settings = Gio::Settings.new("org.gtk.exampleapp")
    @settings.bind("transition", stack, "transition-type", :default)
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
    buffer = view.buffer
    file.read do |stream|
      buffer.text = stream.read.force_encoding(Encoding::UTF_8)
    end
    tag = buffer.create_tag
    @settings.bind("font", tag, "font", :default)
    buffer.apply_tag(tag, buffer.start_iter, buffer.end_iter)
  end
end

class ExampleApp < Gtk::Application
  def initialize
    super("org.gtk.exampleapp", :handles_open)

    signal_connect "startup" do |application|
      [
        "preferences",
        "quit"
      ].each do |action_name|
        action = Gio::SimpleAction.new(action_name)
        action.signal_connect("activate") do |_action, _parameter|
          __send__("#{action_name}_activated")
        end
        application.add_action(action)
      end
      application.set_accels_for_action("app.quit", ["<Control>Q"])
    end
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

  private

  def preferences_activated
    prefs = ExampleAppPrefs.new(active_window)
    prefs.present
  end

  def quit_activated
    quit
  end
end

app = ExampleApp.new

# Gtk::Application#run needs C style argv ([prog, arg1, arg2, ...,argn]).
# The ARGV ruby variable only contains the arguments ([arg1, arg2, ...,argb])
# and not the program name. We have to add it explicitly.

app.run([$PROGRAM_NAME] + ARGV)

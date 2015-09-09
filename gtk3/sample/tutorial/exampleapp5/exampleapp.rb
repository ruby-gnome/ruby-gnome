# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
# Original:
#   * URL: https://git.gnome.org/browse/gtk+/tree/examples/application5/exampleapp.c
#   * URL: https://git.gnome.org/browse/gtk+/tree/examples/application5/exampleappwin.c
#   * License: LGPL 2

require "gtk3"

require "fileutils"

current_path = File.expand_path(File.dirname(__FILE__))
gresource_bin = "#{current_path}/exampleapp.gresource"
gresource_xml = "#{current_path}/exampleapp.gresource.xml"

system("glib-compile-resources",
       "--target", gresource_bin,
       "--sourcedir", current_path,
       gresource_xml)

gschema_bin = "#{current_path}/gschemas.compiled"
gschema_xml = "#{current_path}/org.gtk.exampleapp.gschema.xml"

system("glib-compile-schemas", current_path)


at_exit do
  FileUtils.rm_f([gresource_bin, gschema_bin])
end

resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)

ENV["GSETTINGS_SCHEMA_DIR"] = current_path 

class ExampleAppWindow < Gtk::ApplicationWindow
  # https://github.com/ruby-gnome2/ruby-gnome2/pull/445
  # https://github.com/ruby-gnome2/ruby-gnome2/issues/503
  type_register
  class << self
    def init
      set_template(:resource => "/org/gtk/exampleapp/window.ui")
      bind_template_child("stack")
    end
  end

  def initialize(application)
    super(:application => application)
    settings = Gio::Settings.new("org.gtk.exampleapp")
    settings.bind("transition",
                  stack,
                  "transition-type",
                  Gio::SettingsBindFlags::DEFAULT)
  end

  def open(file)
    basename = file.basename
    scrolled = Gtk::ScrolledWindow.new
    scrolled.show
    scrolled.set_hexpand(true)
    scrolled.set_vexpand(true)
    view = Gtk::TextView.new
    view.set_editable(false)
    view.set_cursor_visible(false)
    view.show
    scrolled.add(view)
    stack.add_titled(scrolled, basename, basename)
    stream = file.read
    view.buffer.text = stream.read
  end
end

class ExampleApp < Gtk::Application
  def initialize
    super("org.gtk.exampleapp", :handles_open)

    signal_connect "startup" do |application|
      quit_accels = ["<Ctrl>Q"]
      action = Gio::SimpleAction.new("quit")
      action.signal_connect("activate") do |_action, parameter|
        application.quit
      end
      application.add_action(action)
      application.set_accels_for_action("app.quit", quit_accels)

      action = Gio::SimpleAction.new("preferences")
      action.signal_connect("activate") do |_action, parameter|

      end
      application.add_action(action)

      builder = Gtk::Builder.new(:resource => "/org/gtk/exampleapp/app-menu.ui")
      app_menu = builder.get_object("appmenu")
      application.set_app_menu(app_menu)

    end
    signal_connect "activate" do |application|
      window = ExampleAppWindow.new(application)
      window.present
    end

    signal_connect "open" do |application, files, hint|
      windows = application.windows
      win = nil
      unless windows.empty?
        win = windows.first
      else
        win = ExampleAppWindow.new(application)
      end

      files.each { |file| win.open(file) }

      win.present
    end

  end
end

app = ExampleApp.new

puts app.run([$PROGRAM_NAME] + ARGV)

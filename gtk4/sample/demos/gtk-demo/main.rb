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

# Original:
#   https://gitlab.gnome.org/GNOME/gtk/-/blob/main/demos/gtk-demo/main.c
#
# See the following for license information:
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/AUTHORS
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/COPYING

require "gtk4"

gresource = File.join(__dir__, "demo.gresource")
gresource_xml = "#{gresource}.xml"
need_generate = false
if File.exist?(gresource)
  need_generate = File.mtime(gresource_xml) > File.mtime(gresource)
else
  need_generate = true
end
if need_generate
  unless system("glib-compile-resources",
                "--target", gresource,
                "--sourcedir", __dir__,
                gresource_xml)
    $stderr.puts("Failed to compile resource")
    exit(false)
  end
end
resource = Gio::Resource.load(gresource)
Gio::Resources.register(resource)

app = Gtk::Application.new("com.github.ruby-gnome.gtk4.Demo",
                           [:non_unique, :handles_command_line])

about_action = Gio::SimpleAction.new("about")
about_action.signal_connect("activate") do
  dialog = Gtk::AboutDialog.new
  dialog.modal = true
  dialog.transient_for = app.active_window
  dialog.destroy_with_parent = true
  dialog.program_name = "Ruby/GTK4 Demo"
  dialog.version = GLib::BINDING_VERSION.join(".")
  dialog.copyright = "(C) 2024-#{Time.now.year} Ruby-GNOME Project Team"
  dialog.license_type = Gtk::License::LGPL_2_1
  dialog.website = "https://ruby-gnome.github.io/"
  dialog.comments = "Program to demonstrate Ruby/GTK4 widgets"
  dialog.authors = ["Ruby-GNOME Project Team"]
  dialog.logo_icon_name = "org.gtk.Demo4"
  dialog.title = "About Ruby/GTK4 Demo"
  dialog.system_information = <<-INFO.chomp
OS\t#{GLib.get_os_info(:name)} #{GLib.get_os_info(:version)}

System Libraries
\tGLib\t#{GLib::Version::STRING}
\tPango\t#{Pango::Version::STRING}
\tGTK\t#{Gtk::Version::STRING}

A link can appear here: <https://ruby-gnome.github.io/>
  INFO
  dialog.present
end
app.add_action(about_action)

quit_action = Gio::SimpleAction.new("quit")
quit_action.signal_connect("activate") do
  app.windows.each do |window|
    window.destroy
  end
end
app.add_action(quit_action)

inspector_action = Gio::SimpleAction.new("inspector")
inspector_action.signal_connect("activate") do
  Gtk::Window.interactive_debugging = true
end
app.add_action(inspector_action)

app.add_main_option("version",
                    0,
                    GLib::OptionFlags::NONE,
                    GLib::OptionArg::NONE,
                    "Show program version")
app.add_main_option("run",
                    0,
                    GLib::OptionFlags::NONE,
                    GLib::OptionArg::STRING,
                    "Run an example",
                    "EXAMPLE")
app.add_main_option("list",
                    0,
                    GLib::OptionFlags::NONE,
                    GLib::OptionArg::NONE,
                    "List examples")
app.add_main_option("auto-quit",
                    0,
                    GLib::OptionFlags::NONE,
                    GLib::OptionArg::NONE,
                    "Quit after a delay")
run = nil
list = false
auto_quit = false
app.signal_connect("command-line") do |_app, command_line|
  run = command_line.options_dict.lookup("run", "s")
  list = command_line.options_dict.lookup("list", "b")
  auto_quit = command_line.options_dict.lookup("auto-quit", "b")
  app.activate
  0
end

app.signal_connect("handle-local-options") do |_app, options|
  if options.lookup("version", "b")
    puts("ruby-gtk4-demo: #{GLib::BINDING_VERSION.join(".")}")
    0
  else
    -1
  end
end

app.signal_connect("activate") do |_app|
  builder = Gtk::Builder.new(resource: "/ui/main.ui")
  window = builder.get_object("window")
  app.add_window(window)
  window.present
end

app.run([$0] + ARGV)

#!/usr/bin/env ruby
#
# Copyright (C) 2006-2015  Ruby-GNOME2 Project Team
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

require "gtk3"
require "optparse"
require "fileutils"

current_path = File.expand_path(File.dirname(__FILE__))

require "#{current_path}/demos.rb"

gresource_bin = "#{current_path}/demo.gresource"
gresource_xml = "#{current_path}/demo.gresource.xml"

system("glib-compile-resources",
       "--target", gresource_bin,
       "--sourcedir", current_path,
       gresource_xml)

gschema_bin = "#{current_path}/gschemas.compiled"

system("glib-compile-schemas", current_path)

at_exit do
  FileUtils.rm_f([gresource_bin, gschema_bin])
end

resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)

ENV["GSETTINGS_SCHEMA_DIR"] = current_path



class Demo < Gtk::Application
  def initialize
    super("org.gtk.Demo", [:non_unique, :handles_command_line])

    @options = {}
    @exit_status = 0

    signal_connect "startup" do |application|
      puts "startup"
      @builder = Gtk::Builder.new(:resource => "/ui/main.ui")
      appmenu = @builder["appmenu"]
      application.set_app_menu(appmenu)
    end

    signal_connect "activate" do |application|
      puts "activate"
      begin
        run_application
      rescue => error
        report_error(error)
        @exit_status = 1
      end
    end

    signal_connect "command-line" do |application, command_line|
      puts "cmd"
      begin
        parse_command_line(command_line.arguments)
      rescue SystemExit => error
        error.status
      rescue OptionParser::InvalidOption => error
        puts error.message
        1
      rescue => error
        report_error(error)
        1
      else
        activate
        @exit_status
      end
    end
 end

  private
  def parse_command_line(arguments)
    parser = OptionParser.new
    parser.on("-r", "--run EXAMPLE", "Run an example") do |example|
      @options[:name] = example
    end
    parser.on("-a", "--autoquit", "Quit after a delay") do
      @options[:autoquit] = true
    end
    parser.on("-l", "--list", "List examples") do
      @options[:list] = true
    end
    parser.parse(arguments)
  end

  def report_error(error)
    puts "#{error.class}: #{error.message}"
    puts error.backtrace
  end

  def run_application
    if @options[:list]
      puts "list"
      # list_demos
      quit
    end

    if @options[:name]
      puts "name"
      # lookup_for_corresponding_demo
      # load_demo
    end

    if @options[:autoquit]
      puts "autoquit"
      GLib::Timeout.add(1) do
        #implement auto_quit
      end
    end

    window = @builder["window"]
    add_window(window)

    action = Gio::SimpleAction.new("run")
    action.signal_connect "activate" do |_action, _parameter|
      # activate_run
    end
    add_action(action)

    notebook = @builder["notebook"]
    info_textwiew = @builder["info-textview"]
    source_textview = @builder["source-textview"]
    headerbar = @builder["headerbar"]
    treeview = @builder["treeview"]
    model = treeview.model

    sw = @builder["source-scrolledwindow"]
    scrollbar = sw.vscrollbar

    menu = Gtk::Menu.new

    item = Gtk::MenuItem.new("Start")
    menu.append(item)
    item.signal_connect "activate" do
      adj = scrollbar.adjustement
      adj.value = adj.get_lower
    end

    item = Gtk::MenuItem.new("End")
    menu.append(item)
    item.signal_connect "activate" do
      adj = scrollbar.adjustement
      adj.value = adj.get_upper - adj.get_page_size
    end

    menu.show_all

    scrollbar.signal_connect "popup-menu" do
      menu.popup(nil, nil, Gtk.current_event_time)
    end

    window.show_all
  end
end

demo = Demo.new

exit(demo.run([$PROGRAM_NAME] + ARGV))

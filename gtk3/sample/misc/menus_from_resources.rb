#!/usr/bin/env ruby
=begin
  menus_from_resources.rb - Ruby/GTK sample script.

  Copyright (c) 2016 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"
require "fileutils"

PATH = File.expand_path(File.dirname(__FILE__))

gresource_bin = "#{PATH}/menus_from_resources.gresource"
gresource_xml = "#{PATH}/menus_from_resources.gresource.xml"

system("glib-compile-resources",
       "--target", gresource_bin,
       "--sourcedir", File.dirname(gresource_xml),
       gresource_xml)

at_exit do
  FileUtils.rm_f(gresource_bin)
end

resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)

application_instance = Gtk::Application.new("org.gtk.menus_from_resources",
                                            :non_unique)

application_instance.signal_connect "startup" do |application|
  %w(about preferences quit copy paste open save close).each do |label|
    action = Gio::SimpleAction.new(label)
    action.signal_connect("activate") do |_action, _parameter|
      puts label
    end
    application.add_action(action)
  end
  builder = Gtk::Builder.new(:resource => "/org/gtk/menus_from_resources/app-menu.ui")
  app_menu = builder["appmenu"]
  application.set_app_menu(app_menu)
end

application_instance.signal_connect "activate" do |application|
  window = Gtk::ApplicationWindow.new(application)
  window.set_default_size(200, 200)
  window.border_width = 20

  toolbar = Gtk::Toolbar.new
  toolbar.style = :both
  toolbutton = Gtk::ToolButton.new(:label => "Open a Menu", :stock_id => Gtk::Stock::OPEN)
  toolbutton.signal_connect "clicked" do |widget|
    builder = Gtk::Builder.new(:resource => "/org/gtk/menus_from_resources/toolbar-menu.ui")
    menu = Gtk::Menu.new(builder["toolbarmenu"])
    # This method is important, it links the menu to the
    # Gio::Actions of the Gtk::Application throught one of
    # its children
    menu.attach_to_widget(widget)

    menu.show_all
    event = Gtk.current_event
    menu.popup(nil, nil, event.button, event.time)
  end
  toolbar.insert(toolbutton, 0)

  button = Gtk::Button.new(:label => "Click me")
  button.signal_connect "clicked" do |widget|
    builder = Gtk::Builder.new(:resource => "/org/gtk/menus_from_resources/button-menu.ui")
    menu = Gtk::Popover.new(widget, builder["buttonmenu"])
    event = Gtk.current_event
    x, y = event.window.coords_to_parent(event.x,
                                         event.y)
    rect = Gdk::Rectangle.new(x - widget.allocation.x,
                              y - widget.allocation.y,
                              1,
                              1)
    menu.set_pointing_to(rect)
    menu.show
  end
  hbox = Gtk::Box.new(:vertical, 0)

  hbox.pack_start(toolbar, :expand => false, :fill => false)
  hbox.pack_start(button, :expand => true, :fill => true)
  window.add(hbox)

  window.show_all
  window.present
end

application_instance.run

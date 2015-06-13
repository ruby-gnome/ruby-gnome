#!/usr/bin/env ruby
=begin
  template_from_resource.rb - Ruby/GTK sample script.

  Copyright (c) 2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"
require "fileutils"

current_path = File.expand_path(File.dirname(__FILE__))

# The gresource file name
gresource_bin = "#{current_path}/template.gresource"

# The gresource xml file name
# see here https://developer.gnome.org/gio/stable/GResource.html
gresource_xml = "#{current_path}/template.gresource.xml"

# Generate the resource file:
# see here https://developer.gnome.org/gio/stable/glib-compile-resources.html
Dir.chdir(File.dirname(gresource_xml)) do
  system("glib-compile-resources",
         "--target", gresource_bin,
         File.basename(gresource_xml))
end

at_exit do
  FileUtils.rm_f(gresource_bin)
end

resource = Gio::Resource.load(gresource_bin)
Gio::Resources.register(resource)

class MyWindow < Gtk::Window
  class << self
    def init
      set_template(:resource => "/template/template.ui")
      bind_template_child("label")
    end
  end

  type_register
end

window = MyWindow.new
window.set_title "Build interface from resource"
window.set_default_size 300, 300

window.label.text = "My UI was created with Glade"

window.show_all

window.signal_connect "destroy" do
  Gtk.main_quit
  Gio::Resources.unregister(resource)
end
Gtk.main

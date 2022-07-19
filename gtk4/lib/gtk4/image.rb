# Copyright (C) 2014-2022  Ruby-GNOME Project Team
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

module Gtk
  class Image
    alias_method :initialize_raw, :initialize
    # Creates a Gtk::Image. The source of the image depends on the options
    # given.
    #
    # @param Hash{Symbol => String, Gio::Icon, GdkPixbuf::Pixbuf}
    #
    # @example Create an empty image.
    #   image = Gtk::Image.new
    #
    # @example Create an image from a file.
    #   image = Gtk::Image.new(file: 'path/to/the/image.png')
    #
    # @example Create an image from icon name.
    #   image = Gtk::Image.new(icon_name: 'gtk-open')
    #
    # @example Create an image from a Gio::Icon, that itself is loaded from a
    #          file.
    #   icon = Gio::Icon.new_for_string 'path/to/the/image.png'
    #   image = Gtk::Image.new(icon: icon)
    #
    # @example Create an image from from a Gio::Icon that is an Gio::ThemedIcon.
    #   icon = Gio::ThemedIcon.new 'gtk-open'
    #   image = Gtk::Image.new(icon: icon)
    #
    # @example Create an image from a GdkPixbuf::Pixbuf.
    #   pixbuf = GdkPixbuf::Pixbuf.new(:file => 'path/to/the/image.png')
    #   image = Gtk::Image.new(pixbuf: pixbuf)
    #
    # @example Create an image from a file in a resource file
    #   resource = Gio::Resource.load(a_resource_file)
    #   Gio::Resources.register(resource)
    #   resource_path = "/path/to/image.png"
    #   image = Gtk::Image.new(resource: resource_path)
    def initialize(options={})
      icon_name = options[:icon_name] || nil
      icon      = options[:icon] || options[:gicon] || nil
      file      = options[:file] || nil
      pixbuf    = options[:pixbuf] || nil
      resource  = options[:resource] || nil

      if icon_name
        initialize_new_from_icon_name(icon_name)
      elsif icon
        initialize_new_from_gicon(icon)
      elsif file
        initialize_new_from_file(file)
      elsif pixbuf
        initialize_new_from_pixbuf(pixbuf)
      elsif resource
        initialize_new_from_resource(resource)
      else
        initialize_raw
      end
    end
  end
end

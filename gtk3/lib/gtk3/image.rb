# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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
    # @param Hash{Symbol => Gtk::Stock, String, Gtk::IconSet, Gio::Icon,
    #                       GdkPixbuf::Pixbuf, GdkPixbuf::PixbufAnimation, Cairo::Surface,
    #                       Fixnum}
    #
    # @example Create an empty image.
    #   image = Gtk::Image.new
    #
    # @example Create an image from a file.
    #   image = Gtk::Image.new :file => 'path/to/the/image.png'
    #
    # @example Create an image from stock.
    #   image = Gtk::Image.new :stock => Gtk::Stock::OPEN, :size => :dialog
    #
    # @example Create an image from icon name.
    #   image = Gtk::Image.new :icon_name => 'gtk-open', :size => :dialog
    #
    # @example Create an image from a Gio::Icon, that itself is loaded from a
    #          file.
    #   icon = Gio::Icon.new_for_string 'path/to/the/image.png'
    #   image = Gtk::Image.new :icon => icon, :size => :dialog
    #
    # @example Create an image from from a Gio::Icon that is an Gio::ThemedIcon.
    #   icon = Gio::ThemedIcon.new 'gtk-open'
    #   image = Gtk::Image.new :icon => icon, :size => :dialog
    #
    # @example Create an image from a GdkPixbuf::Pixbuf.
    #   pixbuf = GdkPixbuf::Pixbuf.new(:file => 'path/to/the/image.png')
    #   image = Gtk::Image.new :pixbuf => pixbuf
    #
    # @example Create an image from an Gtk::IconSet, that itself is created
    #          from a GdkPixbuf::Pixbuf.
    #   pixbuf = GdkPixbuf::Pixbuf.new(:file => 'path/to/the/image.png')
    #   iconSet = Gtk::IconSet.new pixbuf
    #   image = Gtk::Image.new :icon_set => iconSet, :size => :dialog
    #
    # @example Create an image from a GdkPixbuf::PixbufAnimation
    #   pixAnim = GdkPixbuf::PixbufAnimation.new 'anim.gif'
    #   image = Gtk::Image.new :animation => pixAnim
    #
    # @example Create an image from a file in a resource file
    #   resource = Gio::Resource.load(a_resource_file)
    #   Gio::Resources.register(resource)
    #   resource_path = "/path/to/image.png"
    #   image = Gtk::Image.new :resource => resource_path
    #
    # @example Create an image from a Cairo::Surface that is a
    #          Cairo::ImageSurface.
    #   surface = Cairo::ImageSurface.new :RGB24, 60, 60
    #   context = Cairo::Context.new surface
    #   context.set_source_rgb 0, 1, 0
    #   context.rectangle 10, 10, 40, 40
    #   context.fill.stroke
    #   image = Gtk::Image.new :surface => surface
    def initialize(options={})
      stock     = options[:stock] || nil
      icon_name = options[:icon_name] || nil
      icon_set  = options[:icon_set] || nil
      icon      = options[:icon] || options[:gicon] || nil
      file      = options[:file] || nil
      pixbuf    = options[:pixbuf] || nil
      animation = options[:animation] || nil
      resource  = options[:resource] ||nil
      surface   = options[:surface] || nil
      size      = options[:size] || nil

      case size
      when String, Symbol
        size = IconSize.new(size)
      else
        size ||= IconSize::BUTTON
      end

      if stock
        initialize_new_from_stock(stock, size)
      elsif icon_name
        initialize_new_from_icon_name(icon_name, size)
      elsif icon_set
        initialize_new_from_icon_set(icon_set, size)
      elsif icon
        initialize_new_from_gicon(icon, size)
      elsif file
        initialize_new_from_file(file)
      elsif pixbuf
        initialize_new_from_pixbuf(pixbuf)
      elsif animation
        initialize_new_from_animation(animation)
      elsif resource
        initialize_new_from_resource(resource)
      elsif surface
        initialize_new_from_surface(surface)
      else
        initialize_raw
      end
    end
  end
end

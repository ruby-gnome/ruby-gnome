# Copyright (C) 2014  Ruby-GNOME2 Project Team
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
    # Creates a Gtk::Image. The source of the image depends on the options
    # given.
    #
    # @param Hash{Symbol => Gtk::Stock, String, Gtk::IconSet, Gio::Icon,
    #                       Gdk::Pixbuf, Gdk::PixbufAnimation, Cairo::Surface,
    #                       Fixnum}
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
    #   gicon = Gio::Icon.new_for_string 'path/to/the/image.png'
    #   image = Gtk::Image.new :gicon => gicon, :size => :dialog
    #
    # @example Create an image from from a Gio::Icon that is an Gio::ThemedIcon.
    #   gicon = Gio::ThemedIcon.new 'gtk-open'
    #   image = Gtk::Image.new :gicon => gicon, :size => :dialog
    #
    # @example Create an image from a Gdk::Pixbuf.
    #   pixbuf = Gdk::Pixbuf.new 'path/to/the/image.png'
    #   image = Gtk::Image.new :pixbuf => pixbuf
    #
    # @example Create an image from an Gtk::IconSet, that itself is created
    #          from a Gdk::Pixbuf.
    #   pixbuf = Gdk::Pixbuf.new 'path/to/the/image.png'
    #   iconSet = Gtk::IconSet.new pixbuf
    #   image = Gtk::Image.new :icon_set => iconSet, :size => :dialog
    #
    # @example Create an image from a Gdk::PixbufAnimation
    #   pixAnim = Gdk::PixbufAnimation.new 'anim.gif'
    #   image = Gtk::Image.new :animation => pixAnim
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
      gicon     = options[:gicon] || nil
      file      = options[:file] || nil
      pixbuf    = options[:pixbuf] || nil
      animation = options[:animation] || nil
      resource  = options[:resource] ||nil
      surface   = options[:surface] || nil
      size_sym  = options[:size] || nil

      size = Gtk.icon_size_from_name('gtk-' + size_sym.to_s) if size_sym

      if stock
        initialize_new_from_stock stock, size
      elsif icon_name
        initialize_new_from_icon_name icon_name, size
      elsif icon_set
        initialize_new_from_icon_set icon_set, size
      elsif gicon
        initialize_new_from_gicon gicon, size
      elsif file
        initialize_new_from_file file
      elsif pixbuf
        initialize_new_from_pixbuf pixbuf
      elsif animation
        initialize_new_from_animation animation
#      elsif resource
#        # ToDo: resource usage is not verified and therefor not enabled
#        initialize_new_from_resource resource
      elsif surface
        initialize_new_from_surface surface
      else
        raise ArgumentError, "Invalid arguments."
      end
    end
  end
end

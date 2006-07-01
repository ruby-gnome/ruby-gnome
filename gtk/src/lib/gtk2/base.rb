#! /usr/bin/env ruby
=begin
  gtk2/base.rb

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: base.rb,v 1.2 2006/07/01 09:08:34 mutoh Exp $
=end


require 'glib2'
require 'atk'
require 'pango'
begin
  require 'cairo'
rescue LoadError
end
require 'gtk2.so'
require 'gdk_pixbuf2'

module Gdk
  LOG_DOMAIN = "Gdk"

  module_function
  def cairo_available?
    Gdk::Drawable.instance_methods.include?("create_cairo_context")
  end
end

module Gtk
  LOG_DOMAIN = "Gtk"
end

 GLib::Log.set_log_domain(Gdk::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::LOG_DOMAIN)

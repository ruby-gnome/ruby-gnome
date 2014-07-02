#! /usr/bin/env ruby
=begin
  Copyright (c) 2006-2011 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require 'gtk3/base'
require 'gtk3/deprecated'

Gdk.init if Gdk.respond_to?(:init)
Gtk.init

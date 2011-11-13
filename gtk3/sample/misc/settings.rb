=begin
  setting.rb - Gtk::Settings sample script.

  Copyright (C) 2004-2006 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: settings.rb,v 1.3 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

spec = GLib::Param::Int.new("integer",         # name
			    "Integer",         # nick
			    "Integer integer", # blurb
			    0,                 # min
			    10000,             # max
			    0,                 # default
			    GLib::Param::READABLE|
			    GLib::Param::WRITABLE)

enum = GLib::Param::Enum.new("enum",                      # name
			     "Enum",                      # nick
			     "Enum enum",                 # blurb
			     GLib::Type["GdkCursorType"], #type
			     Gdk::Cursor::ARROW,          #default
			     GLib::Param::READABLE|
			     GLib::Param::WRITABLE)

flags = GLib::Param::Flags.new("flags",                       # name
			       "Flags",                       # nick
			       "Flags flags",                 # blurb
			       GLib::Type["GdkEventMask"],    #type
			       Gdk::Event::LEAVE_NOTIFY_MASK, #default
			       GLib::Param::READABLE|
			       GLib::Param::WRITABLE)

p Gtk::Settings.rc_property_parse_color(spec, "{2222,44444,65535}").to_a
p Gtk::Settings.rc_property_parse_enum(enum, "watch")
p Gtk::Settings.rc_property_parse_flags(flags, "(scroll-mask|structure-mask)")
p Gtk::Settings.rc_property_parse_requisition(spec, "{100, 200}")
p Gtk::Settings.rc_property_parse_border(spec, "{100, 200, 300, 400}").to_a

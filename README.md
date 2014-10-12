# README

## Ruby-GNOME2 -- Ruby bindings for GNOME-2.x

This is a set of bindings for the GNOME-2.x and GNOME-3.x libraries to
use from Ruby 1.9.x, 2.0.0 and 2.1.

## Release packages

### ruby-gtk2

* Ruby/GLib2:           GLib 2.12.x or later
* Ruby/ATK:             ATK 1.12.x or later
* Ruby/Pango:           Pango 1.14.x or later
* Ruby/GdkPixbuf2:      GTK+ 2.0.x or later
* Ruby/GTK2:            GTK+ 2.10.x or later
* Ruby/GObjectIntrospection: GObject Introspection 1.32.1 or later

### ruby-gtk3

* ruby-gtk2 - Ruby/GTK2
* Ruby/GDK3:            GTK+ 3.4.2 or later
* Ruby/GTK3:            GTK+ 3.4.2 or later

### ruby-gnome2-all

* ruby-gtk2 + ruby-gtk3
* Ruby/RSVG:            librsvg 2.8.0 or later
* Ruby/Poppler:         poppler-glib 0.8.0 or later
* Ruby/VTE              VTE 0.12.1 or later
* Ruby/GtkSourceView2:  GtkSourceView 2.0.0 or later
* Ruby/GIO2:            GIO 2.16.x or later
* Ruby/CairoGObject:    cairo-gobject 1.12.10 or later
* Ruby/Clutter:         Clutter 1.12.0 or later
* Ruby/ClutterGTK:      Clutter-GTK 1.2.0 or later
* Ruby/ClutterGStreamer:Clutter-GStreamer 2.0.0 or later
* Ruby/GtkSourceView3:  GtkSourceView 3.4.2 or later
* Ruby/VTE3:            VTE 0.32.2 or later
* Ruby/WebKitGTK:       WebKitGTK+ 1.8.1 or later (for GTK+ 3)
* Ruby/WebKitGTK2:      WebKitGTK+ 1.8.1 or later (for GTK+ 2)
* Ruby/GStreamer:       GStreamer 1.0.0 or later

### Experimental / Incomplete:

* ...

## Install

    % ruby extconf.rb
    % make
    % sudo make install

To compile and install a particular sub-binding, you can add arguments:

    % ruby extconf.rb [subdir]...
    e.g.) ruby extconf.rb glib2 pango atk gdk_pixbuf2 gtk2

Or you can compile each sub-binding:

    % cd <each sub-directory>
    % ruby extconf.rb
    % make
    % sudo make install

### extconf.rb options

* `--ruby`
  * ruby directory

* `--topsrcdir`
  * top source directory

* `--topdir`
  * top directory

* `--strict`
  * if some libraries fail to compile/install, "make"
    command returns 1(exit 1)

### Environment Variables

* `CAIRO_PATH`
  * rcairo path (for Win32).

### Dependencies

You should install a sub-binding with its dependencies.  The
current dependencies are:

* Ruby/GLib
  * none.
* Ruby/ATK
  * depends on Ruby/GLib.
* Ruby/Pango
  * depends on Ruby/GLib, rcairo.
* Ruby/GdkPixbuf
  * depends on Ruby/GLib.
* Ruby/GTK
  * depends on Ruby/GLib, Ruby/ATK, Ruby/Pango,
    Ruby/GdkPixbuf, rcairo
* Ruby/RSVG
  * depends on Ruby/GLib, Ruby/GdkPixbuf2.
* Ruby/Poppler
  * depends on Ruby/GLib, Ruby/GdkPixbuf(optional), rcairo.
* Ruby/VTE
  * depends on Ruby/GTK.
* Ruby/GtkSourceView2
  * depends on Ruby/GTK.
* Ruby/GIO2
  * depends on Ruby/GLib.
* Ruby/GStreamer
  * depends on Ruby/GLib.

### Experimental

* ...

## Bugs

Please report bugs either in our bug tracker

* [https://github.com/ruby-gnome2/ruby-gnome2/issues](https://github.com/ruby-gnome2/ruby-gnome2/issues) (current)
* [http://sourceforge.net/tracker/?group_id=53614&atid=470969](http://sourceforge.net/tracker/?group_id=53614&atid=470969) (previous)

or on the ruby-gnome2-devel-en / ruby-gnome2-devel-ja mailing list

* [http://sourceforge.net/mail/?group_id=53614](http://sourceforge.net/mail/?group_id=53614)

## Copying

Copyright (c) 2002-2014 Ruby-GNOME2 Project Team

This program is free software.
You can distribute/modify this program under the terms of
the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.

## Project Website

[http://ruby-gnome2.sourceforge.jp/](http://ruby-gnome2.sourceforge.jp/)

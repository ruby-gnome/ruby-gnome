#!/usr/bin/env ruby
=begin
  alpha-demo.rb - alpha blended window sample script. (need xcompmgr)

  Original: alphademo.py by Mike Hearn (*1) and David Trowbridge (*2)
  * (1) http://plan99.net/~mike/blog/2006/06/05/alpha-blended-windows-in-python/
  * (2) http://david.navi.cx/blog/?p=91

  Translated to Ruby by Juergen Mangler <juergen.mangler@univie.ac.at>

  Copyright (c) 2006-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"
require "cairo"

class AlphaDemo < Gtk::Window
  def initialize
    super()

    set_app_paintable(true)
    set_title("AlphaDemo")
    set_decorated(false)
    set_default_size(200, 220)
    signal_connect("delete_event") do
      Gtk.main_quit
    end
    set_double_buffered(false)

    signal_connect("draw") do |widget, _event|
      cr = widget.window.create_cairo_context

      rgba = [1.0, 1.0, 1.0]
      rgba << 0.0 if @supports_alpha
      cr.set_source_rgba(rgba)
      cr.set_operator(Cairo::OPERATOR_SOURCE)
      cr.paint

      twidth, theight = widget.size

      cr.set_source_rgba(1.0, 0.2, 0.2, 0.6)
      radius = [twidth, theight].min.to_f / 2 - 0.8

      cr.arc(twidth.to_f / 2, theight.to_f / 2, radius, 0, 2.0 * Math::PI)
      cr.fill_preserve
      cr.stroke
    end
    signal_connect("screen-changed") do |widget, _old_screen|
      screen_changed(widget)
    end

    screen_changed(self)
  end

  def screen_changed(widget)
    visual = widget.screen.rgba_visual
    if visual && widget.screen.composited?
      set_visual(visual)
      @supports_alpha = true
    else
      set_visual(widget.screen.system_visual)
      @supports_alpha = false
    end
  end
end

alpha = AlphaDemo.new
alpha.show

Gtk.main

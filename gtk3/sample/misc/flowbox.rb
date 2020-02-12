#!/usr/bin/env ruby
#
# Copyright (c)  2014  Gian Mario Tagliaretti
# Copyright (c)  2015-2020  Ruby-GNOME Project Team
#
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".
#
# Original code: https://github.com/sebp/PyGObject-Tutorial/blob/master/examples/layout_flowbox_example.py

require "gtk3"

unless Gtk::Version.or_later?(3, 12, 1)
  puts "This sample requires GTK+ 3.12.1 or later: #{Gtk::Version::STRING}"
  exit
end

COLORS = %w(AliceBlue AntiqueWhite AntiqueWhite1 AntiqueWhite2 AntiqueWhite3
            AntiqueWhite4 aqua aquamarine aquamarine1 aquamarine2 aquamarine3
            aquamarine4 azure azure1 azure2 azure3 azure4 beige bisque bisque1
            bisque2 bisque3 bisque4 black BlanchedAlmond blue blue1 blue2
            blue3 blue4 BlueViolet brown brown1 brown2 brown3 brown4 burlywood
            burlywood1 burlywood2 burlywood3 burlywood4 CadetBlue CadetBlue1
            CadetBlue2 CadetBlue3 CadetBlue4 chartreuse chartreuse1 chartreuse2
            chartreuse3 chartreuse4 chocolate chocolate1 chocolate2 chocolate3
            chocolate4 coral coral1 coral2 coral3 coral4)

class FlowBoxWindow < Gtk::Window
  def initialize
    super

    set_border_width(10)
    set_default_size(300, 250)

    header = Gtk::HeaderBar.new
    header.title = "Flow Box"
    header.subtitle = "Sample FlowBox app"
    header.show_close_button = true

    set_titlebar(header)

    scrolled = Gtk::ScrolledWindow.new
    scrolled.set_policy(:never, :automatic)

    flowbox = Gtk::FlowBox.new
    flowbox.valign = :start
    flowbox.max_children_per_line = 30
    flowbox.selection_mode = :none

    COLORS.each do |color|
      swatch = create_color_swatch(color)
      flowbox.add swatch
    end

    scrolled.add(flowbox)
    add(scrolled)

    signal_connect("destroy") { Gtk.main_quit }
  end

  private

  def create_color_swatch(color_name)
    color = Gdk::RGBA.parse(color_name)
    button = Gtk::Button.new

    area = Gtk::DrawingArea.new
    area.set_size_request(24, 24)
    area.signal_connect("draw") do |_, context|
      context.set_source_rgba(color)
      context.paint
    end

    button.add(area)
  end
end

win = FlowBoxWindow.new
win.show_all

Gtk.main

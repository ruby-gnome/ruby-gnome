#!/usr/bin/env ruby
=begin
  flowbox.rb    Ruby/GTK script

  Adapted (or stolen) from http://python-gtk-3-tutorial.readthedocs.org/en/latest/layout.html#flowbox

  Copyright (c) 2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end
require "gtk3"

unless Gtk::Version.or_later?(3.12.1) 
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
    header.set_title("Flow Box")
    header.subtitle = "Sample FlowBox app"
    header.show_close_button = true

    set_titlebar(header)

    scrolled = Gtk::ScrolledWindow.new
    scrolled.set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC)

    @flowbox = Gtk::FlowBox.new
    @flowbox.set_valign(Gtk::Align::START)
    @flowbox.set_max_children_per_line(30)
    @flowbox.set_selection_mode(Gtk::SelectionMode::NONE)
    fill_flowbox

    scrolled.add(@flowbox)
    add(scrolled)

    signal_connect("destroy") { Gtk.main_quit }
  end

  private

  def color_swatch_new(color_name)
    color = Gdk::RGBA.parse(color_name)
    button = Gtk::Button.new

    area = Gtk::DrawingArea.new
    area.set_size_request(24, 24)
    area.override_background_color(0, color)

    button.add(area)

    button
  end

  def fill_flowbox
    COLORS.each do |color|
      @flowbox.add(color_swatch_new(color))
    end
  end
end

win = FlowBoxWindow.new
win.show_all

Gtk.main

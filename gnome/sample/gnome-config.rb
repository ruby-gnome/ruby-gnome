# -*- indent-tabs-mode: nil -*-
# $Id: gnome-config.rb,v 1.2 2003/02/02 12:47:55 tkubo Exp $
#
# gnome-config.rb - sample script of Gnome::Config
#
# Copyright (c) 2002-2003 Ruby-GNOME2 Project Team
# Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
require 'gnome2'

class SampleApp < Gnome::App

  NAME = 'ruby-gnome-config'
  TITLE = 'Gnome Config Sample'
  VERSION = '1.0'

  def initialize
    super(NAME, TITLE)

    self.signal_connect("delete_event") { Gtk::main_quit() }

    box = Gtk::VBox.new()
    box.border_width = 10
    self.contents = box

    button = Gtk::Button.new("Save Position")
    button.signal_connect("clicked") { save_position() }
    box.pack_start(button, true, true, 0)

    button = Gtk::Button.new("Save Size")
    button.signal_connect("clicked") { save_size() }
    box.pack_start(button, true, true, 0)

    button = Gtk::Button.new("Restore Position")
    button.signal_connect("clicked") { restore_position() }
    box.pack_start(button, true, true, 0)

    button = Gtk::Button.new("Restore Size")
    button.signal_connect("clicked") { restore_size() }
    box.pack_start(button, true, true, 0)

    button = Gtk::Button.new("Clear Position")
    button.signal_connect("clicked") { clear_position() }
    box.pack_start(button, true, true, 0)

    button = Gtk::Button.new("Clear Size")
    button.signal_connect("clicked") { clear_size() }
    box.pack_start(button, true, true, 0)

    button = Gtk::Button.new("Quit")
    button.signal_connect("clicked") { Gtk::main_quit() }
    box.pack_start(button, true, true, 0)

    restore_settings_with_default()
    self.show_all()
  end

  private
  def restore_settings_with_default
    Gnome::Config::push_prefix(self.prefix)
    # format:                  "Section/key=default"
    x = Gnome::Config::get_int("Position/x=100")
    y = Gnome::Config::get_int("Position/y=100")
    w = Gnome::Config::get_int("Size/w=200")
    h = Gnome::Config::get_int("Size/h=200")
    self.move(x, y)
    self.resize(w, h)
    Gnome::Config::pop_prefix
  end

  def save_position
    x, y = self.position
    Gnome::Config::push_prefix(self.prefix)
    Gnome::Config::set_int("Position/x", x)
    Gnome::Config::set_int("Position/y", y)
    Gnome::Config::pop_prefix()
    # if you forget to sync(), your setting will be forgotten.
    Gnome::Config::sync()
  end

  def save_size
    w, h = self.size
    Gnome::Config::push_prefix(self.prefix)
    Gnome::Config::set_int("Size/w", w)
    Gnome::Config::set_int("Size/h", h)
    Gnome::Config::pop_prefix()
    Gnome::Config::sync()
  end

  def restore_position
    Gnome::Config::push_prefix(self.prefix)
    if Gnome::Config::has_section?("Position")
      x = Gnome::Config::get_int("Position/x")
      y = Gnome::Config::get_int("Position/y")
      self.move(x, y)
    end
    Gnome::Config::pop_prefix
  end

  def restore_size
    Gnome::Config::push_prefix(self.prefix)
    if Gnome::Config::has_section?("Size")
      w = Gnome::Config::get_int("Size/w")
      h = Gnome::Config::get_int("Size/h")
      self.resize(w, h)
    end
    Gnome::Config::pop_prefix
  end

  def clear_position
    Gnome::Config::push_prefix(self.prefix)
    Gnome::Config::clean_section("Position")
    Gnome::Config::pop_prefix
    Gnome::Config::sync()
  end

  def clear_size
    Gnome::Config::push_prefix(self.prefix)
    Gnome::Config::clean_section("Size")
    Gnome::Config::pop_prefix
    Gnome::Config::sync()
  end
end

Gnome::Program.new(SampleApp::NAME, SampleApp::VERSION)
SampleApp.new()
Gtk::main()

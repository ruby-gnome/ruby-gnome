# -*- indent-tabs-mode: nil -*-
# $Id: gnome-druid.rb,v 1.4 2002/11/07 13:05:31 tkubo Exp $
#
# test-druid.rb - sample script of Gnome::Druid
#                 This program was based of test-druid of libgnomeui
#                 and enhanced many features.
#
# Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
#
# Original copyright is unknown. There is no copyright in original source.
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

require 'gnome2'

class TestDruid < Gtk::Window
  def initialize
    super(Gtk::Window::TOPLEVEL)

    self.set_title("Druid Sample")
    self.signal_connect("delete_event") do
      Gtk::main_quit()
    end

    druid = Gnome::Druid.new()
    self.add(druid)
    druid.signal_connect("cancel") do
      Gtk::main_quit()
    end

    one = Gtk::Entry.new()
    two = Gtk::Entry.new()
    three = Gtk::Entry.new()
    four = Gtk::Entry.new()

    one.set_text("Eins")
    two.set_text("Twei")
    three.set_text("Drei")
    four.set_text("Vier")

    druid_page = Gnome::DruidPageEdge.new(Gnome::EDGE_START, false, "Start Page", "information")
    druid.append_page(druid_page)

    druid_page = Gnome::DruidPageStandard.new("Test Druid", nil, nil)
    druid.append_page(druid_page)
    druid_page.append_item("Test _one:", one, "Longer information here")
    druid_page.append_item("Test _two:", two, "Longer information here")
    druid_page.append_item("Test t_hree:", three, "Longer information here")
    druid_page.append_item("Test fou_r:", four, "Longer information here")

    druid_page = Gnome::DruidPageEdge.new(Gnome::EDGE_FINISH, false, "Finish Page")
    druid.append_page(druid_page)
    druid_page.signal_connect("prepare") do |page, druid|
      page.text = <<EOS
Confirm
  one:   #{one.text()}
  two:   #{two.text()}
  three: #{three.text()}
  four:  #{four.text()}
EOS
    end
    druid_page.signal_connect("finish") do |page, druid|
      puts <<EOS
Your Settings
  one:   #{one.text()}
  two:   #{two.text()}
  three: #{three.text()}
  four:  #{four.text()}
EOS
      Gtk::main_quit()
    end
  end
end

Gnome::Program.new('test-gnome-druid', '0.1')

window = TestDruid.new()
window.show_all()

Gtk::main()

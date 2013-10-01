#!/usr/bin/env ruby
#
# This sample code is a port of gtk3/demos/gtk-demo/css_accordion.c. The
# CSS files used in this sample code are copied from gtk3/demos/gtk-demo.
# They are licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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
=begin
= CSS Theming/CSS Accordion

A simple accordion demo written using CSS transitions and multiple backgrounds
=end

require "common"

module Demo
  class CssAccordion < BasicWindow
    def initialize
      super("CssAccordion")
      set_default_size(600, 300)

      container = Gtk::Box.new(:horizontal, 0)
      container.set_halign(:center)
      container.set_valign(:center)
      add(container)

      child = Gtk::Button.new(:label => "This")
      container.add(child)

      child = Gtk::Button.new(:label => "Is")
      container.add(child)

      child = Gtk::Button.new(:label => "A")
      container.add(child)

      child = Gtk::Button.new(:label => "CSS")
      container.add(child)

      child = Gtk::Button.new(:label => "Accordion")
      container.add(child)

      child = Gtk::Button.new(:label => ":-)")
      container.add(child)

      provider = Gtk::CssProvider.new
      provider.load(:data => File.read("css_accordion.css"))

      apply_css(self, provider)
    end

    def apply_css(widget, provider)
      widget.style_context.add_provider(provider, GLib::MAXUINT)
      if widget.respond_to?(:each_forall)
        widget.each_forall do |child|
          apply_css(child, provider)
        end
      end
    end
  end
end

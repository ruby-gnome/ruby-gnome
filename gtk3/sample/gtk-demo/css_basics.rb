#
# This sample code is a port of gtk3/demos/gtk-demo/css_basic.c. The
# CSS file used in this sample code is copied from gtk3/demos/gtk-demo.
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
= CSS Theming/CSS Basics

Gtk themes are written using CSS. Every widget is build of multiple items
that you can style very similarly to a regular website.
=end

require "common"

module Demo
  class CssBasics < BasicWindow
    def initialize
      super("CSS Basics")
      set_default_size(400, 300)

      text = Gtk::TextBuffer.new
      text.create_tag("warning", "underline" => Pango::AttrUnderline::SINGLE)
      text.create_tag("error", "underline" => Pango::AttrUnderline::ERROR)

      provider = Gtk::CssProvider.new

      container = Gtk::ScrolledWindow.new(nil, nil)
      add(container)
      child = Gtk::TextView.new(text)
      container.add(child)
      text.signal_connect("changed") do |_text|
        css_text_changed(_text, provider)
      end

      text.text = File.read(File.join(__dir__, "css_basics.css"))

      provider.signal_connect("parsing-error") do |_provider, section, error|
        p section
        p error
        show_parsing_error(section, error, child.buffer)
      end

      apply_css(self, provider)
    end

    private
    def show_parsing_error(section, error, buffer)
      p buffer.methods.grep /iter/
      start = buffer.get_iter_at_line_index(section.start_line,
                                            section.start_position)

      end_ = buffer.get_iter_at_line_index(section.end_line,
                                           section.end_position)

      if error == Gtk::CssProvider::ERROR ||
          error ==  Gtk::CssProvider::ERROR_DEPRECATED
        tag_name = "warning"
      else
        tag_name = "error"
      end

      buffer.apply_tag_by_name(buffer, tag_name, start, end_)
    end

    def css_text_changed(buffer, provider)
      start = buffer.start_iter
      end_ = buffer.end_iter
      buffer.remove_all_tags(start, end_)

      text = buffer.get_text(start, end_, false)
      Dir.chdir(__dir__) do
        provider.load(:data => text)
      end

      Gdk::Screen.default.reset_widgets
    end

    def apply_css(widget, provider)
      widget.style_context.add_provider(provider, GLib::MAXUINT)
      if widget.is_a?(Gtk::Container)
        widget.each_forall do |child|
          apply_css(child, provider)
        end
      end
    end
  end
end

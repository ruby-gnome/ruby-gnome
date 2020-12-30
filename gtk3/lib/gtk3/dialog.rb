# Copyright (C) 2015-2020  Ruby-GNOME Project Team
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

module Gtk
  class Dialog
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      initialize_raw

      title   = options[:title]
      parent  = options[:parent]
      flags   = options[:flags]
      buttons = options[:buttons]

      set_title(title) if title
      set_transient_for(parent) if parent
      if flags
        unless flags.is_a?(DialogFlags)
          flags = DialogFlags.new(flags)
        end
        set_modal(true) if flags.modal?
        set_destroy_with_parent(true) if flags.destroy_with_parent?
      end

      add_buttons(*buttons) if buttons
    end

    alias_method :run_raw, :run
    def run
      response_id = run_raw
      if response_id < 0
        ResponseType.new(response_id)
      else
        response_id
      end
    end

    def add_buttons(*buttons)
      buttons.each do |text, response_id|
        add_button(text, response_id)
      end
    end

    alias_method :add_button_raw, :add_button
    def add_button(text, response_id)
      add_button_raw(text, ResponseType.resolve(response_id))
    end

    alias_method :get_widget_for_response_raw, :get_widget_for_response
    def get_widget_for_response(response_id)
      get_widget_for_response_raw(ResponseType.resolve(response_id))
    end

    alias_method :set_default_response_raw, :set_default_response
    def set_default_response(response_id)
      set_default_response_raw(ResponseType.resolve(response_id))
    end
    alias_method :default_response_raw=, :default_response=
    alias_method :default_response=, :set_default_response

    if method_defined?(:use_header_bar)
      alias_method :use_header_bar_raw, :use_header_bar
      undef_method :use_header_bar
      def use_header_bar?
        use_header_bar_raw != 0
      end
    end
  end
end

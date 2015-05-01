# Copyright (C) 2014  Ruby-GNOME2 Project Team
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
  class Label
    alias_method :initialize_raw, :initialize
    def initialize(text=nil, options={})
      if options == true or options == false or options.nil?
        mnemonic = options
        warn "Gtk::Label.new(text, mnemonic) style has been deprecated. " +
             "Use Gtk::Label.new(text, {:use_underline => #{mnemonic}}) style instead."
        options = {
          :use_underline => mnemonic,
        }
      end
      if options[:use_underline]
        initialize_new_with_mnemonic(text)
      else
        initialize_raw(text || "")
      end
    end

    private :set_markup_with_mnemonic
    alias_method :set_markup_raw, :set_markup
    def set_markup(text, options={})
      if options == true or options == false or options.nil?
        mnemonic = options
        warn "Gtk::Label#set_markup(text, mnemonic) style has been deprecated. " +
             "Use Gtk::Label#set_marup(text, {:use_underline => #{mnemonic}}) style instead."
        options = {
          :use_underline => mnemonic,
        }
      end
      if options[:use_underline]
        set_markup_with_mnemonic(text)
      else
        set_markup_raw(text)
      end
    end
  end
end

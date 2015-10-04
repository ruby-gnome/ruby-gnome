# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
  class << self
    alias_method :show_uri_raw, :show_uri
    def show_uri(uri_or_options)
      if uri_or_options.is_a?(String)
        options = {:uri => uri_or_options}
      else
        options = uri_or_options
      end

      screen    = options[:screen]
      uri       = options[:uri]
      timestamp = options[:timestamp] || Gdk::CURRENT_TIME

      show_uri_raw(screen, uri, timestamp)
    end
  end
end

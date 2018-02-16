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
  class MessageDialog
    def initialize(options={})
      parent  = options[:parent]
      flags   = options[:flags] || 0
      type    = options[:type] || :info
      buttons = options[:buttons] || options[:buttons_type] || :ok
      message = options[:message]

      initialize_general = GLib::Object.instance_method(:initialize).bind(self)
      initialize_general.call(:message_type => type,
                              :buttons => buttons)
      Loader.reference_gobject(self, :sink => true)

      if message
        self.use_markup = false
        self.text = message
      end

      if parent
        self.transient_for = parent
      end

      if flags
        unless flags.is_a?(DialogFlags)
          flags = DialogFlags.new(flags)
        end
        self.modal = true if flags.modal?
        self.destroy_with_parent = true if flags.destroy_with_parent?
      end
    end
  end
end

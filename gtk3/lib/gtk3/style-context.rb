# Copyright (C) 2015-2019  Ruby-GNOME2 Project Team
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
  class StyleContext
    class << self
      def resolve_priority(priority)
        case priority
        when Symbol, String
          StyleProvider.const_get("PRIORITY_#{priority.to_s.upcase}")
        else
          priority || StyleProvider::PRIORITY_APPLICATION
        end
      end

      alias_method :add_provider_for_screen_raw, :add_provider_for_screen
      def add_provider_for_screen(screen, provider, priority=nil)
        priority = resolve_priority(priority)
        add_provider_for_screen_raw(screen, provider, priority)
      end

      alias_method :reset_widgets_raw, :reset_widgets
      def reset_widgets(screen=nil)
        reset_widgets_raw(screen || Gdk::Screen.default)
      end
    end

    alias_method :add_provider_raw, :add_provider
    def add_provider(provider, priority=nil)
      priority = self.class.resolve_priority(priority)
      add_provider_raw(provider, priority)
    end
  end
end

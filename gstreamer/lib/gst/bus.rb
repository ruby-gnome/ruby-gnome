# Copyright (C) 2013-2019  Ruby-GNOME2 Project Team
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

module Gst
  class Bus
    alias_method :add_watch_full, :add_watch
    def add_watch(priority=GLib::PRIORITY_DEFAULT, &block)
      add_watch_full(priority, &block)
    end

    def sync_handler(&block)
      @sync_handler = lambda do |bus, message|
        begin
          block.call(bus, message)
        rescue Exception
          $stderr.puts("An exception is raised in " +
                         "#{self.class}\##{__method__} callback: #{block}")
          $stderr.puts("#{$!.class}: #{$!.message}")
          $stderr.puts($@)
          BusSyncReply::DROP
        end
      end
      set_sync_handler(&@sync_handler)
    end
    private :set_sync_handler

    alias_method :poll_raw, :poll
    def poll(*args)
      events = nil
      timeout = nil

      n_args = args.size
      case n_args
      when 0
      when 1
        case args[0]
        when Hash
          options = args[0]
          events = options[:events]
          timeout = options[:timeout]
        when Integer
          timeout = args[0]
        else
          events = args[0]
        end
      when 2
        events, timeout = args
      else
        message = "wrong number of arguments "
        message << "(given #{n_args}, expected 0..2)"
        raise ArgumentError, message
      end
      poll_raw(events || :any,
               timeout || CLOCK_TIME_NONE)
    end
  end
end

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
  class Builder
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      file      = options[:file]
      resource  = options[:resource]
      string    = options[:string]

      if file
        initialize_new_from_file(file)
      elsif resource
        initialize_new_from_resource(resource)
      elsif string
        initialize_new_from_string(string, string.bytesize)
      else
        initialize_raw
      end
    end

    alias_method :[], :get_object

    alias_method :add_from_string_raw, :add_from_string
    def add_from_string(string)
      add_from_string_raw(string, string.bytesize)
    end

    def add(target)
      if target.respond_to?(:to_path)
        add_from_file(target.to_path)
      elsif target.start_with?("<") or target.start_with?(" ")
        add_from_string(target)
      elsif File.exist?(target)
        add_from_file(target)
      else
        add_from_resource(target)
      end
    end

    def <<(target)
      add(target)
      self
    end

    private
    def normalize_handler_name(name)
      name.gsub(/[-\s]+/, "_")
    end

    def __connect_signals__(connector, object, signal_name,
                            handler_name, connect_object, flags)
      handler_name = normalize_handler_name(handler_name)
      if connect_object
        handler = connect_object.method(handler_name)
      else
        handler = connector.call(handler_name)
      end
      unless handler
        $stderr.puts("Undefined handler: #{handler_name}") if $DEBUG
        return
      end

      if flags.after?
        signal_connect_method = :signal_connect_after
      else
        signal_connect_method = :signal_connect
      end

      if handler.arity.zero?
        object.send(signal_connect_method, signal_name) {handler.call}
      else
        object.send(signal_connect_method, signal_name, &handler)
      end
    end
  end
end

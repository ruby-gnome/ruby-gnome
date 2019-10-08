# Copyright (C) 2014-2019  Ruby-GNOME Project Team
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

module Gio
  class InputStream
    class << self
      def open(*arguments)
        input_stream = new(*arguments)
        return input_stream unless block_given?

        begin
          yield(input_stream)
        ensure
          input_stream.close unless input_stream.closed?
        end
      end
    end

    alias_method :read_raw, :read
    def read(size=nil)
      if size.nil?
        all = "".force_encoding("ASCII-8BIT")
        buffer_size = 8192
        buffer = " ".force_encoding("ASCII-8BIT") * buffer_size
        loop do
          read_bytes = read_all_raw_compatible(buffer)
          all << buffer.byteslice(0, read_bytes)
          break if read_bytes != buffer_size
        end
        all
      else
        buffer = " " * size
        read_bytes = read_raw_compatible(buffer)
        buffer.replace(buffer.byteslice(0, read_bytes))
        buffer
      end
    end

    alias_method :read_all_raw, :read_all
    def read_all(size)
      buffer = " " * size
      read_bytes = read_all_raw_compatible(buffer)
      buffer.replace(buffer.byteslice(0, read_bytes))
      buffer
    end

    private
    def read_raw_compatible(buffer)
      if (GLib::VERSION <=> [2, 62, 0]) >= 0
        read_bytes, _buffer = read_raw(buffer)
        read_bytes
      elsif (GLib::VERSION <=> [2, 36, 0]) >= 0
        read_raw(buffer)
      else
        read_bytes = read_raw(buffer, buffer.bytesize)
      end
    end

    def read_all_raw_compatible(buffer)
      if (GLib::VERSION <=> [2, 62, 0]) >= 0
        _success, _buffer, read_bytes = read_all_raw(buffer)
        read_bytes
      elsif (GLib::VERSION <=> [2, 36, 0]) >= 0
        _success, read_bytes = read_all_raw(buffer)
        read_bytes
      else
        _success, read_bytes = read_all_raw(buffer, buffer.bytesize)
        read_bytes
      end
    end
  end
end

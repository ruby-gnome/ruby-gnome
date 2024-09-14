# Copyright (C) 2021-2024  Ruby-GNOME Project Team
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

require "fiddle"

require_relative "ruby-seekable"

module Gio
  class RubyInputStream < InputStream
    type_register("RubyInputStream")

    include Seekable
    include RubySeekable

    def initialize(ruby_input)
      @ruby_input = ruby_input
      @buffer = +""
      super()
    end

    private
    def ruby_io
      @ruby_input
    end

    def virtual_do_read_fn(buffer_address, count, cancellable)
      buffer = Fiddle::Pointer.new(buffer_address)
      if @ruby_input.read(count, @buffer)
        buffer[0, @buffer.bytesize] = @buffer
        @buffer.bytesize
      else
        0
      end
    end

    def virtual_do_skip(count, cancellable)
      if @ruby_input.respond_to?(:seek)
        if @ruby_input.seek(count, IO::SEEK_CUR).zero?
          count
        else
          -1
        end
      else
        if @ruby_input.read(count, @buffer)
          @buffer.bytesize
        else
          -1
        end
      end
    end

    def virtual_do_close_fn(cancellable)
      @ruby_input.close
      true
    end
  end
end

# Copyright (C) 2021  Ruby-GNOME Project Team
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
  module RubySeekable
    private
    # GSeekable methods
    def virtual_do_tell
      ruby_io.tell
    end

    def virtual_do_can_seek
      ruby_io.respond_to?(:seek)
    end

    def virtual_do_seek(offset, type, cancellable)
      case type
      when GLib::IOChannel::SEEK_CUR
        ruby_type = IO::SEEK_CUR
      when GLib::IOChannel::SEEK_SET
        ruby_type = IO::SEEK_SET
      when GLib::IOChannel::SEEK_END
        ruby_type = IO::SEEK_END
      end
      ruby_io.seek(offset, ruby_type).zero?
    end

    def virtual_do_can_truncate
      ruby_io.respond_to?(:truncate)
    end

    def virtual_do_truncate_fn(offset, cancellable)
      ruby_io.truncate(offset)
      true
    end
  end
end

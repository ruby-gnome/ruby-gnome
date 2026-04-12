# Copyright (C) 2026  Ruby-GNOME Project Team
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
  class Buffer
    alias_method :map_raw, :map
    def map(flags)
      success, info = map_raw(flags)
      raise Gst::CoreError::Failed.new("failed to map buffer") unless success
      return info unless block_given?

      begin
        yield info
      ensure
        unmap(info)
      end
    end

    alias_method :map_range_raw, :map_range
    def map_range(idx, length, flags)
      success, info = map_range_raw(idx, length, flags)
      raise Gst::CoreError::Failed.new("failed to map buffer") unless success
      return info unless block_given?

      begin
        yield info
      ensure
        unmap(info)
      end
    end
  end
end

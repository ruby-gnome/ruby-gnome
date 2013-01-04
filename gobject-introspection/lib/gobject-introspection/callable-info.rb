# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

require "gobject-introspection/collection-reader"

module GObjectIntrospection
  class CallableInfo
    extend CollectionReader

    collection_reader("args")

    def in_args
      args.find_all do |arg|
        case arg.direction
        when Direction::IN, Direction::INOUT
          true
        else
          false
        end
      end
    end

    def n_in_args
      in_args.size
    end

    def out_args
      args.find_all do |arg|
        case arg.direction
        when Direction::OUT, Direction::INOUT
          true
        else
          false
        end
      end
    end

    def n_out_args
      out_args.size
    end
  end
end

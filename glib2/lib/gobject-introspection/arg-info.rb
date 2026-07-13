# Copyright (C) 2015-2019  Ruby-GNOME Project Team
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

module GObjectIntrospection
  class ArgInfo
    def gclosure?
      @gclosure_p ||= nil
      return @gclosure_p unless @gclosure_p.nil?
      @gclosure_p = compute_gclosure?
    end

    def signature
      sig = "#{name}:"
      sig << " (optional)" if optional?
      sig << " (may be null)" if may_be_null?
      sig << " (caller allocates)" if caller_allocates?
      sig << " #{type.description}"
      sig
    end

    def inspect
      super.gsub(/>\z/) do
        " name=#{name.inspect}" +
          " optional?=#{optional?.inspect}" +
          " may_be_null?=#{may_be_null?.inspect}" +
          " caller_allocates?=#{caller_allocates?.inspect}" +
          " type=#{type.inspect}>"
      end
    end

    private
    def compute_gclosure?
      type_info = type
      return false if type_info.tag != TypeTag::INTERFACE

      struct_info = type_info.interface
      struct_info.gtype.name == "GClosure"
    end
  end
end

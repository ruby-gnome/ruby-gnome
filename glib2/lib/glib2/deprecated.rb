# Copyright (C) 2018-2021  Ruby-GNOME Project Team
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

module GLib
  module Idle
    extend GLib::Deprecatable
    define_deprecated_singleton_method :remove,
                                       warn: "Use 'GLib::Source.remove(id)'."
  end

  class Param
    extend GLib::Deprecatable
    define_deprecated_const(:PRIVATE, :STATIC_NAME)
    define_deprecated_method(:private?, :static_name?)
  end

  class SpawnError
    extend GLib::Deprecatable
    define_deprecated_const(:E2BIG, :TOO_BIG)
  end

  module Unicode
    extend GLib::Deprecatable
    define_deprecated_singleton_method(
      :canonical_decomposition,
      warn: "Use 'GLib::UniChar.decompose(char)'.") do |_, char|
      UniChar.decompose(char)
    end
  end

  extend GLib::Deprecatable
  define_deprecated_singleton_method(
    :format_size_for_display,
    warn: "Use 'GLib.format_size(size, flags: :iec_units)'.") do |_, size|
    format_size(size, flags: :iec_units)
  end
end

# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

module Pango
  extend GLib::Deprecatable

  define_deprecated_enums :Alignment, "ALIGN"
  define_deprecated_enums :CoverageLevel, "COVERAGE"
  define_deprecated_enums :Direction, "DIRECTION"
  define_deprecated_enums :WrapMode, "WRAP"
  define_deprecated_enums :FontMask, "FONT_MASK"
  define_deprecated_enums :Stretch, "STRETCH"
  define_deprecated_enums :Style, "STYLE"
  define_deprecated_enums :Variant, "VARIANT"
  define_deprecated_enums :Weight, "WEIGHT"
  define_deprecated_enums :TabAlign, "TAB"
  define_deprecated_enums :Underline, "UNDERLINE"
  define_deprecated_enums :Script, "SCRIPT"
  define_deprecated_enums :EllipsizeMode, "ELLIPSIZE"
  define_deprecated_enums :RenderPart, "PART"

  class Context
    extend GLib::Deprecatable

    define_deprecated_enums :Direction, "DIRECTION"
  end

  class FontDescription
    extend GLib::Deprecatable

    define_deprecated_enums :Weight, "WEIGHT"
    define_deprecated_enums :Style, "STYLE"
  end

  class Gravity
    extend GLib::Deprecatable

    define_deprecated_enums :GravityHint, "HINT"
  end

  class Layout
    extend GLib::Deprecatable

    define_deprecated_enums :Alignment, "ALIGN"
    define_deprecated_const :Alignment, "Pango::Alignment"
    define_deprecated_enums :WrapMode, "WRAP"
    define_deprecated_const :WrapMode, "Pango::WrapMode"
    define_deprecated_enums :EllipsizeMode, "ELLIPSIZE"
    define_deprecated_const :EllipsizeMode, "Pango::EllipsizeMode"
  end
end

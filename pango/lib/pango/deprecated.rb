# Copyright (C) 2017-2018  Ruby-GNOME2 Project Team
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
  define_deprecated_enums :Alignment
  define_deprecated_enums :CoverageLevel, "COVERAGE"
  define_deprecated_enums :CoverageLevel
  define_deprecated_enums :Direction, "DIRECTION"
  define_deprecated_enums :Direction
  define_deprecated_enums :WrapMode, "WRAP"
  define_deprecated_enums :WrapMode
  define_deprecated_enums :FontMask, "FONT_MASK"
  define_deprecated_enums :FontMask
  define_deprecated_enums :Stretch, "STRETCH"
  define_deprecated_enums :Stretch
  define_deprecated_enums :Style, "STYLE"
  define_deprecated_enums :Style
  define_deprecated_enums :Variant, "VARIANT"
  define_deprecated_enums :Variant
  define_deprecated_enums :Weight, "WEIGHT"
  define_deprecated_enums :Weight
  define_deprecated_enums :TabAlign, "TAB"
  define_deprecated_enums :TabAlign
  define_deprecated_enums :Underline, "UNDERLINE"
  define_deprecated_enums :Underline
  define_deprecated_const :UNDERLINE, "Pango::Underline"
  define_deprecated_enums :Script, "SCRIPT"
  define_deprecated_enums :Script
  define_deprecated_enums :EllipsizeMode, "ELLIPSIZE"
  define_deprecated_enums :EllipsizeMode
  define_deprecated_enums :RenderPart, "PART"
  define_deprecated_enums :RenderPart

  class AttrScale
    extend GLib::Deprecatable

    define_deprecated_const :XX_SMALL, "Pango::Scale::XX_SMALL"
    define_deprecated_const :X_SMALL, "Pango::Scale::X_SMALL"
    define_deprecated_const :SMALL, "Pango::Scale::SMALL"
    define_deprecated_const :MEDIUM, "Pango::Scale::MEDIUM"
    define_deprecated_const :LARGE, "Pango::Scale::LARGE"
    define_deprecated_const :X_LARGE, "Pango::Scale::X_LARGE"
    define_deprecated_const :XX_LARGE, "Pango::Scale::XX_LARGE"
  end

  class AttrUnderline
    extend GLib::Deprecatable

    define_deprecated_enums :Underline
  end

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

    define_deprecated_method :get_pixel_extents, :pixel_extents
  end

  class Rectangle
    extend GLib::Deprecatable

    define_deprecated_method :lbearing, :left_bearing
    define_deprecated_method :rbearing, :right_bearing
  end

  class Renderer
    extend GLib::Deprecatable

    define_deprecated_enums :RenderPart, "PART"
  end
end

# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

module Vte
  extend GLib::Deprecatable
  define_deprecated_const :TerminalEraseBinding, 'Vte::Terminal::EraseBinding'
  define_deprecated_const :TerminalCursorBlinkMode, 'Vte::Terminal::CursorBlinkMode'
  define_deprecated_const :TerminalCursorShape, 'Vte::Terminal::CursorShape'

  class Terminal
    extend GLib::Deprecatable
    define_deprecated_method :match_set_cursor_type, :match_set_cursor
    define_deprecated_method :fork_pty, :raise => "Use 'Vte::Pty#fork'."
    define_deprecated_method :using_xft?, :warn => "Don't use this method." do |_self|
      false
    end
    define_deprecated_method :padding, :warn => "Use 'inner-border' style property." do |_self|
      [0, 0]
    end
    define_deprecated_method :char_descent, :warn => "Don't use this method." do |_self|
      0
    end
    define_deprecated_method :char_ascent, :warn => "Don't use this method." do |_self|
      0
    end
  end
end

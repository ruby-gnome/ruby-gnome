module Vte
  extend GLib::Deprecatable
  define_deprecated_const 'TerminalEraseBinding', 'Vte::Terminal::EraseBinding'
  define_deprecated_const 'TerminalCursorBlinkMode', 'Vte::Terminal::CursorBlinkMode'
  define_deprecated_const 'TerminalCursorShape', 'Vte::Terminal::CursorShape'

  class Terminal
    extend GLib::Deprecatable
    define_deprecated_method 'match_set_cursor_type', 'match_set_cursor'
  end
end


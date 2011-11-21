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
    define_deprecated_method_by_hash_args :fork_command, 'command, argv, envv, directory, lastlog, utmp, wtmp' do |_self, command, argv, envv, directory, lastlog, utmp, wtmp|
      pty_flags = nil # TODO
      argv = command && [command, *argv]
      {:pty_flags => pty_flags, :working_directory => directory, :argv => argv, :envv => envv}
    end

    alias :__set_font__ :set_font
    private :__set_font__
    def set_font(*args)
      if args.size == 1
        params = args.first
      else
        raise GLib::DeprecatedError.new("#{caller[0]}: '#{self.class}#set_font(desc_or_name, antialias)' style has been deprecated. Use '#{self.class}#set_font(desc_or_name)' style.")
      end
      __set_font__(params)
    end
  end
end


module Vte
  extend GLib::Deprecatable
  define_deprecated_const 'TerminalEraseBinding', 'Vte::Terminal::EraseBinding'
  define_deprecated_const 'TerminalCursorBlinkMode', 'Vte::Terminal::CursorBlinkMode'
  define_deprecated_const 'TerminalCursorShape', 'Vte::Terminal::CursorShape'

  class Terminal
    extend GLib::Deprecatable
    define_deprecated_method 'match_set_cursor_type', 'match_set_cursor'
    define_deprecated_method :fork_pty, :raise => "Use 'Vte::Pty#fork'."
    define_deprecated_method :using_xft?, :warn => "Don't use this method." do |_self|
      false
    end

    alias :__fork_command__ :fork_command
    private :__fork_command__
    def fork_command(*args)
      if args.size == 1 && args.first.is_a?(Hash)
        params = args.first
      else
        warn "#{caller[0]}: '#{self.class}#fork_command(command, argv, envv, directory, lastlog, utmp, wtmp)' style has been deprecated. Use '#{self.class}#fork_command(options = {})' style."
        command, argv, envv, directory, lastlog, utmp, wtmp = args
        params = {
          :pty_flags          => nil,  # TODO
          :working_directory  => directory,
          :argv               => command && [command, *argv],
          :envv               => envv,
        }
      end
      __fork_command__(params)
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


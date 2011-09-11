module GNOMEUtils
  @@deprecated_const = {}

  def define_deprecated_const(deprecated_const, new_const)
    @@deprecated_const[self] ||= {}
    @@deprecated_const[self][deprecated_const.to_sym] = new_const
  end

  def define_deprecated_method(deprecated_method, new_method)
    if public_method_defined?(new_method)
      define_method(deprecated_method) do |*args, &block|
        warn "'#{deprecated_method}' has been deprecated. Use '#{new_method}'."
        __send__(new_method, *args, &block)
      end
    end
  end

  private

  def const_missing(deprecated_const)
    if new_const = (@@deprecated_const[self] || {})[deprecated_const.to_sym]
      if new_const = constant_get(new_const)
        warn "'#{[name, deprecated_const].join('::')}' has been deprecated. Use '#{new_const}'."
        const_set(deprecated_const, new_const)
      end
    end
  end

  def constant_get(const)
    const.split('::').inject(Object){|r, c| r.const_get(c, false)} rescue nil
  end
end

module Vte
  extend GNOMEUtils
  define_deprecated_const 'TerminalEraseBinding', 'Vte::Terminal::EraseBinding'
  define_deprecated_const 'TerminalCursorBlinkMode', 'Vte::Terminal::CursorBlinkMode'
  define_deprecated_const 'TerminalCursorShape', 'Vte::Terminal::CursorShape'

  class Terminal
    extend GNOMEUtils
    define_deprecated_method 'match_set_cursor_type', 'match_set_cursor'
  end
end


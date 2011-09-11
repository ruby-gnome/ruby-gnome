module GNOMEUtils
  def define_deprecated_method(deprecated_method, new_method)
    if public_method_defined?(new_method)
      define_method(deprecated_method) do |*args, &block|
        warn "'#{deprecated_method}' has been deprecated. Use '#{new_method}'."
        __send__(new_method, *args, &block)
      end
    end
  end
end

module Vte
  class Terminal
    extend GNOMEUtils
    define_deprecated_method 'match_set_cursor_type', 'match_set_cursor'
  end
end


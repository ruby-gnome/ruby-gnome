module GLib
  module Deprecatable
    @@deprecated_const = {}

    def define_deprecated_const(deprecated_const, new_const = {})
      @@deprecated_const[self] ||= {}
      case new_const
      when String, Symbol
        new_const = new_const.to_s
      end
      @@deprecated_const[self][deprecated_const.to_sym] = new_const
    end

    def define_deprecated_method(deprecated_method, new_method = {}, &block)
      klass = self
      define_method(deprecated_method) do |*margs, &mblock|
        msg = "#{caller[0]}: '#{klass}##{deprecated_method}' has been deprecated."
        case new_method
        when String, Symbol
          warn "#{msg} Use '#{klass}##{new_method}'."
          __send__(new_method, *margs, &mblock)
        when Hash
          if new_method[:raise]
            raise DeprecatedError.new("#{msg} #{new_method[:raise]}")
          elsif new_method[:warn]
            warn "#{msg} #{new_method[:warn]}"
            block.call(self, *margs, &mblock) if block
          end
        end
      end
    end

    def define_deprecated_method_by_hash_args(deprecated_method, args, &block)
      klass = self
      alias_name = "__#{deprecated_method}__"
      alias_method alias_name, deprecated_method
      private alias_name
      define_method(deprecated_method) do |*margs, &mblock|
        if margs.empty? || (margs.size == 1 && margs.first.is_a?(Hash))
          params = margs.first
        else
          msg = "#{caller[0]}: '#{klass}##{deprecated_method}(#{args})' style has been deprecated."
          warn "#{msg} Use '#{klass}##{deprecated_method}(options = {})' style."
          params = block.call(self, *margs, &mblock)
        end
        __send__(alias_name, params, &mblock)
      end
    end

    private

    def const_missing(deprecated_const)
      if new_const = (@@deprecated_const[self] || {})[deprecated_const.to_sym]
        msg = "#{caller[0]}: '#{[name, deprecated_const].join('::')}' has been deprecated."
        if new_const.is_a?(String)
          if new_const_val = constant_get(new_const)
            warn "#{msg} Use '#{new_const}'."
            const_set(deprecated_const, new_const_val)
          else
            super
          end
        else
          if new_const[:raise]
            raise DeprecatedError.new("#{msg} #{new_const[:raise]}")
          else
            super
          end
        end
      else
        super
      end
    end

    def constant_get(const)
      const.split('::').inject(Object){|r, c| r.const_get(c)} rescue nil
    end
  end

  class DeprecatedError < RuntimeError
  end
end


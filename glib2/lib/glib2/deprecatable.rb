# Copyright (C) 2015-2017  Ruby-GNOME2 Project Team
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
  module Deprecatable
    unless respond_to?(:define_singleton_method)
      def define_singleton_method(name, &block)
        singleton_class = class << self; self; end
        singleton_class.__send__(:define_method, name, &block)
      end
    end

    @@deprecated_const = {}
    def define_deprecated_const(deprecated_const, new_const = {})
      @@deprecated_const[self] ||= {}
      @@deprecated_const[self][deprecated_const.to_sym] = new_const
    end

    def define_deprecated_enums(enums, prefix = nil)
      enums = resolve_constant_name(enums.to_s)
      enums.constants.each do |const|
        deprecated_const = prefix ? "#{prefix}_#{const}" : const
        new_const = [enums, const].join('::')
        define_deprecated_const(deprecated_const, new_const)
      end
    end
    alias :define_deprecated_flags :define_deprecated_enums

    def define_deprecated_singleton_method(deprecated_method, new_method = {}, &block)
      __define_deprecated_method__(:singleton, deprecated_method, new_method, &block)
    end

    def define_deprecated_method(deprecated_method, new_method = {}, &block)
      __define_deprecated_method__(:instance, deprecated_method, new_method, &block)
    end

    def define_deprecated_method_by_hash_args(deprecated_method, old_args, new_args, req_argc = 0, &block)
      klass = self
      alias_name = "__deprecatable_#{object_id}_#{deprecated_method}__"
      alias_method alias_name, deprecated_method
      private alias_name

      define_method(deprecated_method) do |*margs, &mblock|
        if (margs.size == req_argc) || (margs.size == (req_argc + 1) && margs.last.is_a?(Hash))
        else
          margs = block.call(self, *margs, &mblock)
          msg = "#{caller[0]}: '#{klass}##{deprecated_method}(#{old_args})' style has been deprecated."
          warn "#{msg} Use '#{klass}##{deprecated_method}(#{new_args})' style."
        end
        __send__(alias_name, *margs, &mblock)
      end
    end

    @@deprecated_signal = {}
    def define_deprecated_signal(deprecated_signal, new_signal = {})
      @@deprecated_signal[self] ||= {}
      @@deprecated_signal[self][deprecated_signal.to_s.gsub('_', '-').to_sym] = new_signal
    end

    def self.extended(class_or_module)
      GLib::Instantiatable.class_eval do
        %w(signal_connect signal_connect_after).each do |connect_method|
          alias_name = "__deprecatable_#{connect_method}__"
          next if private_method_defined?(alias_name)
          alias_method alias_name, connect_method
          private alias_name

          define_method(connect_method) do |signal, *margs, &mblock|
            signal = signal.to_s.gsub('_', '-').to_sym
            signals = @@deprecated_signal[self]
            if new_signal = (signals || {})[signal]
              msg = "#{caller[0]}: '#{signal}' signal has been deprecated."
              case new_signal
              when String, Symbol
                warn "#{msg} Use '#{new_signal}' signal."
                signal = new_signal
              when Hash
                if new_signal[:raise]
                  raise DeprecatedError.new("#{msg} #{new_signal[:raise]}")
                elsif new_signal[:warn]
                  warn "#{msg} #{new_signal[:warn]}"
                else
                  warn "#{msg} Don't use this signal anymore."
                end
                return
              end
            end
            __send__(alias_name, signal, *margs, &mblock)
          end
        end
      end
    end

    private

    def const_missing(deprecated_const)
      new_const = (@@deprecated_const[self] || {})[deprecated_const.to_sym]
      if new_const.nil?
        return super
      end

      msg = "#{caller[0]}: '#{[name, deprecated_const].join('::')}' has been deprecated."
      case new_const
      when String, Symbol
        new_const_val = resolve_constant_name(new_const)
        case new_const_val
        when GLib::Enum, GLib::Flags
          alt = " or ':#{new_const_val.nick.gsub('-', '_')}'"
        end
        warn "#{msg} Use '#{new_const}'#{alt}."
        return const_set(deprecated_const, new_const_val)
      when Hash
        if new_const[:raise]
          raise DeprecatedError.new("#{msg} #{new_const[:raise]}")
        elsif new_const[:warn]
          warn "#{msg} #{new_const[:warn]}"
        else
          warn "#{msg} Don't use this constant anymore."
        end
        return
      else
        super
      end
    end

    def resolve_constant_name(name)
      name.to_s.split("::").inject(nil) do |context, local_name|
        if context.nil?
          candidates = []
          candidate_context = ::Object
          self.to_s.split("::").each do |candidate_name|
            candidate = candidate_context.const_get(candidate_name)
            candidates.unshift(candidate)
            candidate_context = candidate
          end
          context = candidates.find do |candidate|
            candidate.const_defined?(local_name)
          end
          context ||= ::Object
        end
        context.const_get(local_name)
      end
    end

    def __define_deprecated_method__(type, deprecated_method, new_method = {}, &block)
      def_method = type == :singleton ? :define_singleton_method : :define_method
      sep = type == :singleton ? '.' : '#'
      klass = self

      __send__(def_method, deprecated_method) do |*margs, &mblock|
        msg = "#{caller[0]}: '#{klass}#{sep}#{deprecated_method}' has been deprecated."
        case new_method
        when String, Symbol
          warn "#{msg} Use '#{klass}#{sep}#{new_method}'."
          if block
            block.call(self, *margs, &mblock)
          else
            __send__(new_method, *margs, &mblock)
          end
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
  end

  class DeprecatedError < RuntimeError
  end
end


# Copyright (C) 2005-2021  Ruby-GNOME Project Team
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

require 'pathname'
require 'English'
require 'thread'
require 'glib2/deprecatable'

module GLib
  module_function
  def check_binding_version?(major, minor, micro)
    BINDING_VERSION[0] > major ||
      (BINDING_VERSION[0] == major &&
       BINDING_VERSION[1] > minor) ||
      (BINDING_VERSION[0] == major &&
       BINDING_VERSION[1] == minor &&
       BINDING_VERSION[2] >= micro)
  end

  def exit_application(exception, status)
    msg = exception.message || exception.to_s
    msg = exception.class.to_s if msg == ""
    backtrace = exception.backtrace || []
    first_line = backtrace.shift
    if first_line
      $stderr.puts("#{first_line}: #{msg}")
    else
      $stderr.puts(msg)
    end
    backtrace.each do |v|
      $stderr.puts("\t from #{v}")
    end
    exit(status)
  end

  def __add_one_arg_setter(klass)
    # for Instance methods.
    method_names = klass.instance_methods(false)
    method_names.each do |method_name|
      next if /\Aset_/ !~ method_name
      property_name = $POSTMATCH
      next if klass.method_defined?("#{property_name}=")
      next if klass.instance_method(method_name).arity != 1
      begin
        klass.module_eval("def #{property_name}=(val); set_#{property_name}(val); val; end\n")
      rescue SyntaxError
        if $DEBUG
          $stderr.puts "Couldn't create #{klass}\##{property_name}=(v)."
        end
      end
    end

    # for Class methods/Module functions.
    if klass.method(:methods).arity == -1
      method_names = klass.methods(false)
    else
      method_names = klass.methods
    end
    singleton_klass = (class << klass; self; end)
    method_names.each do |method_name|
      next if /\Aset_/ !~ method_name
      property_name = $POSTMATCH
      next if singleton_klass.method_defined?("#{property_name}=")
      next if klass.method(method_name).arity != 1
      begin
        klass.module_eval("def self.#{property_name}=(val); set_#{property_name}(val); val; end\n")
      rescue SyntaxError
        if $DEBUG
          $stderr.puts "Couldn't create #{klass}.#{property_name}=(v)."
        end
      end
    end
  end

  def prepend_path_to_environment_variable(path, environment_name)
    path = Pathname(path) unless path.is_a?(Pathname)
    return unless path.exist?

    dir = path.to_s
    dir = dir.gsub(/\//, ::File::ALT_SEPARATOR) if ::File::ALT_SEPARATOR

    separator = ::File::PATH_SEPARATOR
    paths = (ENV[environment_name] || '').split(separator)
    unless paths.include?(dir)
      paths = [dir] + paths
      ENV[environment_name] = paths.join(separator)
    end
  end

  def prepend_dll_path(path)
    path = Pathname(path) unless path.is_a?(Pathname)
    return unless path.exist?

    begin
      require "ruby_installer/runtime"
    rescue LoadError
    else
      RubyInstaller::Runtime.add_dll_directory(path.to_s)
    end
    prepend_path_to_environment_variable(path, "PATH")
  end
end

require "glib2.so"

module GLib
  SIGNAL_HANDLER_PREFIX = "signal_do_"
  VIRTUAL_FUNCTION_IMPLEMENTATION_PREFIX = "virtual_do_"

  module MetaInterface
    class << self
      def signal_callback(klass, name)
        lambda do |instance, *args|
          method_name = "#{SIGNAL_HANDLER_PREFIX}#{name}"
          klass.instance_method(method_name).bind(instance).call(*args)
        end
      end
    end
  end

  class Instantiatable
    class << self
      def method_added(name)
        super

        case name.to_s
        when /\A#{Regexp.escape(SIGNAL_HANDLER_PREFIX)}/o
          signal_name = $POSTMATCH
          begin
            signal_ = signal(signal_name)
          rescue NoSignalError
            return
          end
          return unless signal_.class != self
          signal_handler_attach(signal_, name.to_s) do |instance, *args|
            instance.__send__(name, *args)
          end
        when /\A#{Regexp.escape(VIRTUAL_FUNCTION_IMPLEMENTATION_PREFIX)}/o
          ancestors.each do |klass|
            next unless klass.respond_to?(:implement_virtual_function)
            return if klass.implement_virtual_function(self, name)
          end
        end
      end
    end

    private
    def create_signal_handler(signal_name, callback)
      callback
    end
  end

  class Type

    def decendants
      [self] + children.map{|t| t.decendants }.flatten
    end

    def ancestors
      #  ([self] + interfaces + (parent ? parent.ancestors : [])).reverse.uniq.reverse
      [self] + (parent ? parent.ancestors : [])
    end
  end


  class Enum
    class << self
      def try_convert(value)
        if value.is_a?(self)
          value
        else
          find(value)
        end
      end

      def _load(obj)
        new(Marshal.load(obj))
      end
    end

    def _dump(limit)
      Marshal.dump(to_i, limit)
    end
  end


  class Flags
    class << self
      def try_convert(value)
        case value
        when self
          value
        when Integer, String, Symbol, Array
          begin
            new(value)
          rescue ArgumentError
            nil
          end
        else
          nil
        end
      end

      def _load(obj)
        new(Marshal.load(obj))
      end
    end

    def _dump(limit)
      Marshal.dump(to_i, limit)
    end

    # FIXME
    def inspect
      values = self.class.values
      if values.find{|x| x == self }
        body = nick
      else
        a = values.select{|x| self >= x }
        a = a.reject{|x| a.find{|y| y > x } }
        body = a.empty? ? '{}' : a.map{|x| x.nick }.join('|')
      end
      format('#<%s %s>', self.class.inspect, body)
    end
  end

  module Log
    DOMAIN = "Ruby/GLib"
    LEVELS = {
      LEVEL_ERROR => "ERROR",
      LEVEL_CRITICAL => "CRITICAL",
      LEVEL_WARNING => "WARNING",
      LEVEL_MESSAGE => "MESSAGE",
      LEVEL_INFO => "INFO",
      LEVEL_DEBUG => "DEBUG"
    }

    module_function
    def error(str)
      log(DOMAIN, LEVEL_ERROR, caller(1)[0] << ": " << str)
    end
    def message(str)
      log(DOMAIN, LEVEL_MESSAGE, caller(1)[0] << ": " << str)
    end
    def critical(str)
      log(DOMAIN, LEVEL_CRITICAL, caller(1)[0] << ": " << str)
    end
    def warning(str)
      log(DOMAIN, LEVEL_WARNING, caller(1)[0] << ": " << str)
    end

    def set_log_domain(domain)
      level =
        FLAG_RECURSION |
        FLAG_FATAL |
        LEVEL_ERROR |
        LEVEL_CRITICAL |
        LEVEL_WARNING
      if $VERBOSE or $DEBUG
        level |=
          LEVEL_MESSAGE |
          LEVEL_INFO
      end
      if $DEBUG
        level |= LEVEL_DEBUG
      end
      GLib::Log.set_handler(domain, level)
    end
  end

  if const_defined?(:UserDirectory)
    class UserDirectory
      constants.each do |name|
        if /\ADIRECTORY_/ =~ name
          const_set($POSTMATCH, const_get(name))
        end
      end
    end
  end

  LOG_DOMAIN = "GLib"

  class Object
    LOG_DOMAIN = "GLib-GObject"
  end

  class Thread
    LOG_DOMAIN = "GThread"
  end

  module Module
    LOG_DOMAIN = "GModule"
  end
end

GLib::Log.set_log_domain(nil)
GLib::Log.set_log_domain(GLib::LOG_DOMAIN)
GLib::Log.set_log_domain(GLib::Object::LOG_DOMAIN)
GLib::Log.set_log_domain(GLib::Thread::LOG_DOMAIN)
GLib::Log.set_log_domain(GLib::Module::LOG_DOMAIN)

require "glib2/version"
require "glib2/regex"
require "glib2/deprecated"
=begin
Don't we need this?
ObjectSpace.define_finalizer(GLib) {
  GLib::Log.cancel_handler
  puts "GLib::Log.cancel_handler was called." if $DEBUG
}
=end


#
# glib2.rb
# Copyright(C) 2005-2010 Ruby-GNOME2 Project.
#
# This program is licenced under the same 
# license of Ruby-GNOME2.
#

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
    backtrace = exception.backtrace
    $stderr.puts backtrace.shift + ": #{msg}"
    backtrace.each do |v|
      $stderr.puts "\t from #{v}"
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

  def prepend_environment_path(path)
    path = Pathname(path) unless path.is_a?(Pathname)
    if path.exist?
      environment_name = "PATH"
      separator = ::File::PATH_SEPARATOR

      paths = (ENV[environment_name] || '').split(/#{separator}/)
      dir = path.to_s
      dir = dir.gsub(/\//, ::File::ALT_SEPARATOR) if ::File::ALT_SEPARATOR
      unless paths.include?(dir)
        paths = [dir] + paths
        ENV[environment_name] = paths.join(separator)
      end
    end
  end
end


base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
GLib.prepend_environment_path(vendor_dir + "bin")
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/glib2.so"
rescue LoadError
  require 'glib2.so'
end

module GLib
  
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
    def _dump(limit)
      Marshal.dump(to_i, limit)
    end

    def self._load(obj)
      new(Marshal.load(obj))
    end
  end


  class Flags
    def _dump(limit)
      Marshal.dump(to_i, limit)
    end

    def self._load(obj)
      new(Marshal.load(obj))
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
      log(DOMAIN,  LEVEL_WARNING, caller(1)[0] << ": " << str)
    end
    
    def set_log_domain(domain)
      level = GLib::Log::LEVEL_MASK
      if $DEBUG
        level = 255
      elsif $VERBOSE
        level = 127
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

=begin
Don't we need this?
ObjectSpace.define_finalizer(GLib) {
  GLib::Log.cancel_handler
  puts "GLib::Log.cancel_handler was called." if $DEBUG
}
=end


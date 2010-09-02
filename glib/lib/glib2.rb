#
# glib2.rb
# Copyright(C) 2005 Ruby-GNOME2 Project.
#
# This program is licenced under the same 
# license of Ruby-GNOME2.
#

require 'rbconfig'
if /mingw|mswin|mswin32/ =~ RUBY_PLATFORM
  ENV['PATH'] = %w(bin lib).collect{|dir|
    "#{Config::CONFIG["prefix"]}\\lib\\GTK\\#{dir};"
  }.join('') + ENV['PATH']
end

require 'English'
require 'thread'

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
    #for Instance methods.
    ary = klass.instance_methods(false)
    ary.each do |m|
      if /^set_(.*)/ =~ m and not ary.include? "#{$1}=" and klass.instance_method(m).arity == 1
	begin
          klass.module_eval("def #{$1}=(val); set_#{$1}(val); val; end\n")
        rescue SyntaxError
          $stderr.print "Couldn't create #{klass}\##{$1}=(v).\n" if $DEBUG
        end
      end
    end
    #for Class methods/Module functions.
    if Object.method(:methods).arity == -1
      ary = klass.methods(false)
    else
      ary = klass.methods
    end
    ary.each do |m|
      if /^set_(.*)/ =~ m and not ary.include? "#{$1}=" and klass.method(m).arity == 1
	begin
          klass.module_eval("def self.#{$1}=(val); set_#{$1}(val); val; end\n")
        rescue SyntaxError
          $stderr.print "Couldn't create #{klass}\##{$1}=(v).\n" if $DEBUG
        end
      end
    end
  end
end

require 'glib2.so'

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

# Ruby-GetText-Package support
begin
  require 'gettext'
rescue LoadError
  unless defined? GetText
    module GetText
      module_function
      def _(msgid); msgid; end
      def N_(msgid); msgid; end
      def n_(msgid, msgid_plural, n)
	n == 1 ? msgid : msgid_plural
      end
      def s_(msgid, div = '|')
	if index = msgid.rindex(div)
	  msgid = msgid[(index + 1)..-1]
	else
	  msgid
	end
      end
      def bindtextdomain(domainname, path = nil, locale = nil, charset = nil)
      end
    end
  end
end

=begin
Don't we need this?
ObjectSpace.define_finalizer(GLib) {
  GLib::Log.cancel_handler
  puts "GLib::Log.cancel_handler was called." if $DEBUG
}
=end

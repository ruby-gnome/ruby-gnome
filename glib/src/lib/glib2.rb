#
# Unicode
$KCODE = "U"

# for dropline GTK2-Runtime DLL
# http://www.dropline.net/gtk/
if /mingw|mswin/ =~ RUBY_PLATFORM
  begin
    require 'win32/registry'
    begin
      GTK2Dir =
        Win32::Registry::HKEY_CURRENT_USER.open('Software\GTK\2.0')['Path']
      ENV['PATH'] = %w(bin lib).collect{|dir|
        "#{GTK2Dir}\\#{dir};"
      }.join('') + ENV['PATH']
    rescue Win32::Registry::Error
    end
  rescue LoadError
  end
end

module GLib
  def self.__add_one_arg_setter(klass)
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
      level = GLib::Log::LEVEL_CRITICAL
      if $DEBUG
        level = 255
      elsif $VERBOSE
        level = 127
      end
      GLib::Log.set_handler(domain, level) do |domain, lvl, message|
        if lvl == LEVEL_CRITICAL || lvl = LEVEL_ERROR  
          raise "#{domain}-#{LEVELS[lvl]} **:#{message}"
        else
          $stderr.puts "#{domain}-#{LEVELS[lvl]} **:#{message}"
        end
      end
    end
  end

  LOG_DOMAIN = "GLib"

  class Object
    LOG_DOMAIN = "GLib-GObject"
  end

  module Thread
    LOG_DOMAIN = "GThread"
  end

  module Module
    LOG_DOMAIN = "GModule"
  end
end

GLib::Log.set_log_domain(GLib::LOG_DOMAIN)
GLib::Log.set_log_domain(GLib::Object::LOG_DOMAIN)
GLib::Log.set_log_domain(GLib::Thread::LOG_DOMAIN)
GLib::Log.set_log_domain(GLib::Module::LOG_DOMAIN)


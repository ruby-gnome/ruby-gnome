# for dropline GTK2-Runtime DLL
# http://www.dropline.net/gtk/
if /mingw|mswin/ =~ RUBY_PLATFORM and RUBY_VERSION >= "1.8.0"
  begin
    require 'win32/registry'
    GTK2Dir =
      Win32::Registry::HKEY_CURRENT_USER.open('Software\GTK\2.0')['Path']
    ENV['PATH'] = %w(bin lib).collect{|dir|
      "#{GTK2Dir}\\#{dir};"
    }.join('') + ENV['PATH']
  rescue Win32::Registry::Error
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

end

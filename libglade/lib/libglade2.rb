require 'gtk2'
begin
  require 'gnome2'
rescue LoadError
  puts "Ruby/GNOME2 is not supported." if $DEBUG
end
require 'libglade2.so'

class GladeXML
  def canonical_handler(handler)
    return handler.gsub(/[-\s]/, "_")
  end

  def connect(source, target, signal, handler, data)
    handler = canonical_handler(handler)
    if target
      signal_proc = target.method(handler)
    else
      signal_proc = @handler_proc.call(handler)
    end
    if signal_proc
      case signal_proc.arity
      when 0
        source.signal_connect(signal) {signal_proc.call}
      else
        source.signal_connect(signal, &signal_proc)
      end
    elsif $DEBUG
      puts "Undefined handler: #{handler}"
    end
  end
end

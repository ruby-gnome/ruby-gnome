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

  def connect(source, target, signal, handler, data, after = false)
    handler = canonical_handler(handler)
    if target
      signal_proc = target.method(handler)
    else
      signal_proc = @handler_proc.call(handler)
    end

    if after
      sig_conn_proc = source.method(:signal_connect_after)
    else
      sig_conn_proc = source.method(:signal_connect)
    end
    
    if signal_proc
      case signal_proc.arity
      when 0
        sig_conn_proc.call(signal) {signal_proc.call}
      else
        sig_conn_proc.call(signal, &signal_proc)
      end
    elsif $DEBUG
      puts "Undefined handler: #{handler}"
    end
  end

  def widget_names(nocache = false)
    @widget_names = [] if nocache || ! @widget_names
    if @widget_names.size == 0
      regexp = Regexp.new("<widget class=\".*\" id=\"(.*)\"")
      IO.readlines(filename).each { |line|
	if md = regexp.match(line)
	  @widget_names << md[1]
	end
      }
    end
    @widget_names
  end
end

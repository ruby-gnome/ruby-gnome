#!/usr/bin/env ruby
=begin
  libglade2.rb
                                                                                
  Copyright (c) 2002-2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
                                                                                
  $Id: libglade2.rb,v 1.8 2004/03/23 12:03:51 mutoh Exp $
=end

$DISABLE_RUBY_LIBGLADE_CUSTOM_WIDGET_HANDLER = nil

require 'gtk2'
begin
  require 'gnome2'
rescue LoadError
  puts "Ruby/GNOME2 is not supported." if $DEBUG
end
require 'libglade2.so'

if $DISABLE_RUBY_LIBGLADE_CUSTOM_WIDGET_HANDLER
  GladeXML.set_custom_widget_handler(false)
else
  GladeXML.set_custom_widget_handler(true)
end

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
    build_names(nocache)[0]
  end

  def custom_creation_methods(nocache = false)
    build_names(nocache)[1]
  end

  private
  def escape_xml(str)
    str.gsub(/&(.*?);/n) {
      match = $1.dup
      case match
      when /\Aamp\z/ni           then '&'
      when /\Aquot\z/ni          then '"'
      when /\Agt\z/ni            then '>'
      when /\Alt\z/ni            then '<'
      else
	match
      end
    }
  end

  def build_names(nocache)
    if nocache || ! @widget_names
      @widget_names = []
      @custom_methods = []
    end
    if @widget_names.size == 0
      regexp_name = Regexp.new("<widget class=\".*\" id=\"(.*)\"")
      regexp_custom = Regexp.new("<property name=\"creation_function\">(.*)</property>")
      IO.readlines(filename).each { |line|
	if md = regexp_name.match(line)
	  @widget_names << escape_xml(md[1])
	elsif md = regexp_custom.match(line)
	  @custom_methods << escape_xml(md[1])
	end
      }
    end
    [@widget_names, @custom_methods]    
  end
end

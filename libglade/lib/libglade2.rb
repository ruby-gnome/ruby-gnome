#!/usr/bin/env ruby
=begin
  libglade2.rb
                                                                                
  Copyright (c) 2002-2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
                                                                                
  $Id: libglade2.rb,v 1.17 2006/11/08 00:57:16 ktou Exp $
=end

require 'gtk2'
begin
  require 'gnome2'
rescue LoadError
  puts "Ruby/GNOME2 is not supported." if $DEBUG
end
require 'libglade2.so'
GladeXML.set_custom_widget_handler(true)

class GladeXML
  attr_accessor :handler_proc

  def canonical_handler(handler)
    return handler.gsub(/[-\s]/, "_")
  end

  def connect(source, target, signal, handler, data, after = false)
    @handler_proc ||= Proc.new{}
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

  def guard_sources_from_gc
    widget_names.each do |name|
      guard_source_from_gc(self[name])
    end
  end

  def guard_source_from_gc(source)
    @sources ||= {}
    @sources[source.object_id] = source
    source.signal_connect("destroy") do |object|
      @sources.delete(object.object_id)
    end
    # To get the parent window of the source as a ruby object.
    # Ruby/GTK keeps the Window objects on the memory to prevend from GC.
    parent = source.parent
    while parent
      parent = parent.parent
    end
  end

  LOG_DOMAIN = "libglade"
end

GLib::Log.set_log_domain(GladeXML::LOG_DOMAIN)

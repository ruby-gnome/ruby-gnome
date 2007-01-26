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
require 'libart2.so'

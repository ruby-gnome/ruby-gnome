=begin
  utils.rb - Sample for GLib module function produced by rbglib_utils.c

  Copyright (C) 2004 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2004/10/21 15:50:21 $
  $Id: utils.rb,v 1.2 2004/10/21 15:50:21 mutoh Exp $
=end
                                                                                
require 'glib2'

if GLib.check_version?(2, 2, 0)
  GLib.application_name = "application name"
  puts "GLib.application_name = #{GLib.application_name}"
end

GLib.prgname = "program name"
puts "GLib.prgname = #{GLib.prgname}"

puts "GLib.getenv('HOME') = #{GLib.getenv('HOME')}"
if GLib.check_version?(2, 4, 0)
  GLib.setenv("FOO", "foo")
  puts "GLib.getenv('FOO') = #{GLib.getenv('FOO')}"
  GLib.unsetenv("FOO")
  puts "GLib.getenv('FOO') = #{GLib.getenv('FOO')}"
end

puts "GLib.user_name = #{GLib.user_name}"
puts "GLib.real_name = #{GLib.real_name}"
puts "GLib.home_dir  = #{GLib.home_dir}"
puts "GLib.tmp_dir   = #{GLib.tmp_dir}"
puts "GLib.current_dir = #{GLib.current_dir}"
puts "GLib.path_is_absolute?('./') = #{GLib.path_is_absolute?("./")}"
puts "GLib.path_skip_root('/usr/local/bin/ruby') = #{GLib.path_skip_root('/usr/local/bin/ruby')}"
puts "GLib.path_get_basename(GLib.home_dir) = #{GLib.path_get_basename(GLib.home_dir)}"
puts "GLib.path_get_dirname(GLib.home_dir) = #{GLib.path_get_dirname(GLib.home_dir)}"
puts "GLib.find_program_in_path(GLib.prgname) = #{GLib.find_program_in_path(GLib.prgname)}"
puts "GLib.bit_nth_lsf(3, 1) = #{GLib.bit_nth_lsf(3, 1)}"
puts "GLib.bit_nth_msf(3, 1) = #{GLib.bit_nth_msf(3, 1)}"
puts "GLib.bit_storage(3) = #{GLib.bit_storage(3)}"
puts "GLib.spaced_primes_closest(10) = #{GLib.spaced_primes_closest(10)}"

keys = {
  "foo"  => 1 << 0,
  "bar"  => 1 << 1,
  "hoge" => 1 << 2,
  "fuga" => 1 << 3
}

puts GLib.parse_debug_string("foo", keys)
puts GLib.parse_debug_string("bar", keys)
puts GLib.parse_debug_string("foo:bar:hoge", keys)
puts GLib.parse_debug_string("foo:bar:hoge:fuga", keys)

#
# pkg-config.rb
#
# Wrapper of pkg-config tool.
#
# Copyright(C) 2003-2005 Ruby-GNOME2 Project.
#
# This file is part of Ruby/GLib2.
#
# Ruby/GLib2 is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


require 'mkmf'
require 'shellwords'

module PKGConfig
  @@cmd = with_config('pkg-config', ENV["PKG_CONFIG"] ||  'pkg-config')
  if /mswin32/ =~ RUBY_PLATFORM and /^cl\b/ =~ Config::CONFIG['CC']
    @@cmd += ' --msvc-syntax'
  end

  @@list = {}
  `#{@@cmd} --list-all`.chomp.split(/\n/).each{|v| 
    pkg, name, desc = /(\S+?)\s+(.*?)\s-\s(.*)/.match(v).to_a[1..3]
    @@list[pkg] = [name, desc]
  }

  module_function
  def exist?(pkg)
    system("#{@@cmd} --exists #{pkg}")
  end

  def libs(pkg)
    `#{@@cmd} --libs #{pkg}`.chomp
  end

  def libs_only_L(pkg)
    `#{@@cmd} --libs-only-L #{pkg}`.chomp
  end

  def libs_only_l(pkg)
    `#{@@cmd} --libs-only-l #{pkg}`.chomp
  end

  def cflags(pkg)
    `#{@@cmd} --cflags #{pkg}`.chomp
  end

  def cflags_only_I(pkg)
    `#{@@cmd} --cflags-only-I #{pkg}`.chomp
  end

  def cflags_only_other(pkg)
    `#{@@cmd} --cflags-only-other #{pkg}`.chomp
  end

  def variable(pkg, var)
    `#{@@cmd} --variable=#{var} #{pkg}`.chomp
  end

  def modversion(pkg)
    `#{@@cmd} --modversion #{pkg}`.chomp
  end

  def version
    `#{@@cmd} --version`.chomp
  end

  def list_all
    # Returns [pkg, name, description]
    @@list.keys.collect{|key| [key] + @@list[key]}.sort
  end

  def name(pkg)
    @@list[pkg][0]
  end

  def description(pkg)
    @@list[pkg][1]
  end

  def provides(pkg)
    `#{@@cmd} --print-provides #{pkg}`.chomp
  end

  def requires(pkg)
    `#{@@cmd} --print-requires #{pkg}`.chomp.gsub("\n", ", ") 
  end

  def check_version?(pkg, major = 0, minor = 0, micro = 0)
    return false unless exist?(pkg)
    ver = modversion(pkg).split(".").collect{|item| item.to_i}
    (0..2).each {|i| ver[i] = 0 unless ver[i]}

    (ver[0] > major ||
     (ver[0] == major && ver[1] > minor) ||
     (ver[0] == major && ver[1] == minor &&
      ver[2] >= micro))
  end

  def have_package(pkg, major = nil, minor = 0, micro = 0)
    if major.nil?
      STDOUT.print("checking for #{pkg}... ")
    else
      STDOUT.print("checking for #{pkg} version (>= #{major}.#{minor}.#{micro})... ")
    end
    major ||= 0
    STDOUT.flush
    if check_version?(pkg, major, minor, micro)
      STDOUT.print "yes\n"
      libraries = libs_only_l(pkg)
      dldflags = libs(pkg)
      dldflags = (Shellwords.shellwords(dldflags) - Shellwords.shellwords(libraries)).map{|s| /\s/ =~ s ? "\"#{s}\"" : s }.join(' ')
      $libs   += ' ' + libraries
      if /mswin32/ =~ RUBY_PLATFORM
	$DLDFLAGS += ' ' + dldflags
      else
	$LDFLAGS += ' ' + dldflags
      end
      $CFLAGS += ' ' + cflags(pkg)
      true
    else
      STDOUT.print "no\n"
      false
    end
  end
end

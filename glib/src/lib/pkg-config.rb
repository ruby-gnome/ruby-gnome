#
# pkg-config.rb
#
# Wrapper of pkg-config tool.
#
# Copyright(C) 2003-2005 Ruby-GNOME2 Project.
#
# This program is licenced under the same
# license of Ruby-GNOME2.
#

require 'mkmf'
require 'shellwords'

module PKGConfig
  @@cmd = with_config('pkg-config', ENV["PKG_CONFIG"] ||  'pkg-config')
  if /mswin32/ =~ RUBY_PLATFORM and /^cl\b/ =~ Config::CONFIG['CC']
    @@cmd += ' --msvc-syntax'
  end

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

  def check_version?(pkg, major = 0, minor = 0, micro = 0)
    return false unless exist?(pkg)
    ver = modversion(pkg).split(".").collect{|item| item.to_i}
    (0..2).each {|i| ver[i] = 0 unless ver[i]}

    (ver[0] > major ||
     (ver[0] == major && ver[1] > minor) ||
     (ver[0] == major && ver[1] == minor &&
      ver[2] >= micro))
  end

  def have_package(pkg, major = 0, minor = 0, micro = 0)
    if major > 0
      STDOUT.print("checking for #{pkg} version (>= #{major}.#{minor}.#{micro})... ")
    else
      STDOUT.print("checking for #{pkg}... ")
    end
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

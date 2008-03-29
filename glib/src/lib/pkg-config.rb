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
      library_paths = libs_only_L(pkg)
      library_paths = Shellwords.shellwords(library_paths).collect do |path|
        if /\b--msvc-syntax\b/ =~ @@cmd
          path.gsub(/\A\/libpath:/, '')
        else
          path.gsub(/\A\-L/, '')
        end
      end
      libraries = libs_only_l(pkg)
      dldflags = libs(pkg)
      dldflags = (Shellwords.shellwords(dldflags) - Shellwords.shellwords(libraries)).map{|s| /\s/ =~ s ? "\"#{s}\"" : s }.join(' ')
      $DEFLIBPATH[1, 0] = library_paths
      if $cc_is_gcc
        $DLDFLAGS += ' ' + library_paths.collect do |path|
          "-Wl,-rpath,#{path.quote}"
        end.join(' ')
      end
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

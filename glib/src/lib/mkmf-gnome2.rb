#
# mkmf-gnome2.rb
#
# Extended mkmf for Ruby-GNOME2 and Ruby/GLib based libraries.
#
# Copyright(C) 2003-2005 Ruby-GNOME2 Project.
#
# This program is licenced under the same
# license of Ruby-GNOME2.
#

require 'mkmf'
require 'pkg-config'

unless defined? macro_defined?
  def macro_defined?(macro, src, opt="")
    try_cpp(src + <<EOP, opt)
#ifndef #{macro}
# error
#endif
EOP
  end
end


STDOUT.print("checking for GCC... ")
STDOUT.flush
if macro_defined?("__GNUC__", "")
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
  $cc_is_gcc = true
else
  STDOUT.print "no\n"
  $cc_is_gcc = false
end

include_path = nil
if ENV['GTK_BASEPATH'] and /cygwin/ !~ RUBY_PLATFORM
  include_path = (ENV['GTK_BASEPATH'] + "\\INCLUDE").gsub("\\", "/")
  $hdrdir = include_path
  $CFLAGS += " -I#{include_path} " 
end

def check_win32()
  $G_PLATFORM_WIN32 = false
  $G_OS_WIN32       = false
  $G_WITH_CYGWIN    = false

  STDOUT.print("checking for G_PLATFORM_WIN32... ")
  STDOUT.flush
  if macro_defined?('G_PLATFORM_WIN32', "#include <glibconfig.h>\n")
    STDOUT.print "yes\n"
    $G_PLATFORM_WIN32 = true
  else
    STDOUT.print "no\n"
  end

  if $G_PLATFORM_WIN32
    STDOUT.print("checking for G_OS_WIN32... ")
    STDOUT.flush
    if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
      STDOUT.print "yes\n"
      $G_OS_WIN32 = true
      if $cc_is_gcc
	if /^2\./ =~ `#{Config::CONFIG['CC']} -dumpversion`.chomp
	  $CFLAGS += ' -fnative-struct'
	else
	  $CFLAGS += ' -mms-bitfields'
	end
      end
    else
      STDOUT.print "no\n"
    end

#    STDOUT.print("checking for G_WITH_CYGWIN... ")
#    STDOUT.flush
#    if macro_defined?('G_WITH_CYGWIN', "#include <glibconfig.h>\n")
#      STDOUT.print "yes\n"
#      $G_WITH_CYGWIN = true
#    else
#      STDOUT.print "no\n"
#    end
  end

  nil
end

def set_output_lib(target_name)
  if /cygwin|mingw/ =~ RUBY_PLATFORM
    filename = "libruby-#{target_name}.a"
    if RUBY_VERSION > "1.8.0"
      $DLDFLAGS << ",--out-implib=#{filename}" if filename
    elsif RUBY_VERSION > "1.8"
      $DLDFLAGS.gsub!(/ -Wl,--out-implib=[^ ]+/, '')
      $DLDFLAGS << " -Wl,--out-implib=#{filename}" if filename
    else
      $DLDFLAGS.gsub!(/ --output-lib\s+[^ ]+/, '')
      $DLDFLAGS << " --output-lib #{filename}" if filename
    end
  elsif /mswin32/ =~ RUBY_PLATFORM
      filename = "libruby-#{target_name}.lib"
      $DLDFLAGS.gsub!(/ --output-lib\s+[^ ]+/, '')
      $DLDFLAGS.gsub!(/ \/IMPLIB:[^ ]+/, '')
      $DLDFLAGS << " /IMPLIB:#{filename}" if filename    
  end
end

def setup_win32(target_name)
  check_win32
  set_output_lib(target_name)
end

#add_depend_package("glib2", "glib/src", "/...../ruby-gnome2")
def add_depend_package(target_name, target_srcdir, topdir)
  $CFLAGS += " -I" + File.join(topdir, target_srcdir)

  if /cygwin|mingw/ =~ RUBY_PLATFORM
    $libs << " -lruby-#{target_name}"
    $LDFLAGS << " -L#{topdir}/#{target_srcdir}"
  elsif /mswin32/ =~ RUBY_PLATFORM
    $DLDFLAGS << " /libpath:#{topdir}/#{target_srcdir}"
    $libs << " libruby-#{target_name}.lib"
  end
end

def add_distcleanfile(file)
  $distcleanfiles = [] unless $distcleanfiles 
  $distcleanfiles << file
end

def create_makefile_at_srcdir(pkg_name, srcdir, defs = nil)
  begin
    Dir.mkdir(srcdir) unless File.exist? srcdir
    Dir.chdir srcdir

    yield if block_given?

    $defs << defs if defs
    create_makefile(pkg_name, srcdir)
  ensure
    Dir.chdir('..')
  end
end

def create_top_makefile(sub_dirs = ["src"])
  mfile = File.open("Makefile", "w")
  if /mswin32/ =~ RUBY_PLATFORM
    mfile.print <<MSWIN32_END 

all:
#{sub_dirs.map{|d| "	@cd #{d}\n	@nmake -nologo all\n	@cd ..\n"}.join('')}
install:
#{sub_dirs.map{|d| "	@cd #{d}\n	@nmake -nologo install DESTDIR=$(DESTDIR)\n	@cd ..\n"}.join('')}
site-install:
#{sub_dirs.map{|d| "	@cd #{d}\n	@nmake -nologo site-install DESTDIR=$(DESTDIR)\n	@cd ..\n"}.join('')}
clean:
#{sub_dirs.map{|d| "	@cd #{d}\n	@nmake -nologo clean\n	@cd ..\n"}.join('')}	@if exist extconf.h del extconf.h
	@if exist conftest.* del conftest.*
	@if exist *.lib del *.lib
	@if exist *~ del *~
	@if exist mkmf.log del mkmf.log 
MSWIN32_END
  else
    mfile.print <<END
all:
#{sub_dirs.map{|d| "	@cd #{d}; make all\n"}.join('')}

install:
#{sub_dirs.map{|d| "	@cd #{d}; make install\n"}.join('')}
site-install:
#{sub_dirs.map{|d| "	@cd #{d}; make site-install\n"}.join('')}
clean:
#{sub_dirs.map{|d| "	@cd #{d}; make clean\n"}.join('')}
distclean:	clean
#{sub_dirs.map{|d| "	@cd #{d}; make distclean\n"}.join('')}
	@rm -f Makefile extconf.h conftest.*
	@rm -f core *~ mkmf.log
END
  end
  mfile.close
end

def make_version_header(app_name, pkgname, dir = "src")
  version = PKGConfig.modversion(pkgname).split(/\./)

  filename = "rb#{app_name.lowcase}version.h"

  out = File.open(File.join(dir, filename), "w")

  out.print %Q[/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  #{filename} -

  This file was generated with mkmf-gnome2.rb.

************************************************/

#ifndef __RB#{app_name}_VERSION_H__
#define __RB#{app_name}_VERSION_H__

#define #{app_name}_MAJOR_VERSION (#{version[0]})
#define #{app_name}_MINOR_VERSION (#{version[1]})
#define #{app_name}_MICRO_VERSION (#{version[2]})

#define #{app_name}_CHECK_VERSION(major,minor,micro)    \\
    (#{app_name}_MAJOR_VERSION > (major) || \\
     (#{app_name}_MAJOR_VERSION == (major) && #{app_name}_MINOR_VERSION > (minor)) || \\
     (#{app_name}_MAJOR_VERSION == (major) && #{app_name}_MINOR_VERSION == (minor) && \\
      #{app_name}_MICRO_VERSION >= (micro)))


#endif /* __RB#{app_name}_VERSION_H__ */
]
      out.close
end

def check_ruby_func
  $CPPFLAGS << " -I$(sitearchdir) "

  #Other options
  have_func("rb_define_alloc_func") # for ruby-1.8
  have_func("rb_block_proc") # for ruby-1.8

  STDOUT.print("checking for new allocation framework... ") # for ruby-1.7
  if Object.respond_to? :allocate
    STDOUT.print "yes\n"
    $defs << "-DHAVE_OBJECT_ALLOCATE"
  else
    STDOUT.print "no\n"
  end

  STDOUT.print("checking for attribute assignment... ") # for ruby-1.7
  STDOUT.flush
  if defined? try_compile and try_compile <<SRC
#include "ruby.h"
#include "node.h"
int node_attrasgn = (int)NODE_ATTRASGN;
SRC
    STDOUT.print "yes\n"
    $defs << "-DHAVE_NODE_ATTRASGN"
  else
    STDOUT.print "no\n"
  end
end

def add_obj(name)
  $objs << name unless $objs.index(name)
end


check_ruby_func

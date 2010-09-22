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
begin
  require 'pkg-config'
rescue LoadError
  require 'rubygems'
  gem 'pkg-config'
  require 'pkg-config'
end
require 'glib-mkenums'

unless defined?(macro_defined?)
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
#  $hdrdir += " -I#{include_path} "
  $CFLAGS += " -I#{include_path} "
end

def setup_win32(target_name, base_dir=nil)
  checking_for(checking_message("Win32 OS")) do
    case RUBY_PLATFORM
    when /cygwin|mingw|mswin32/
      import_library_name = "libruby-#{target_name}.a"
      $DLDFLAGS << " -Wl,--out-implib=#{import_library_name}"
      $cleanfiles << import_library_name
      base_dir ||= Pathname($0).dirname.parent.parent.expand_path
      base_dir = Pathname(base_dir) if base_dir.is_a?(String)
      binary_base_dir = base_dir + "vendor" + "local"
      $CFLAGS += " -I#{binary_base_dir}/include"
      pkg_config_dir = binary_base_dir + "lib" + "pkgconfig"
      PKGConfig.add_path(pkg_config_dir.to_s)
      PKGConfig.set_override_variable("prefix", binary_base_dir.to_s)
      true
    else
      false
    end
  end
end

#add_depend_package("glib2", "glib/src", "/...../ruby-gnome2")
def add_depend_package(target_name, target_srcdir, top_srcdir, options={})
  [top_srcdir, $configure_args['--topdir']].each do |topdir|
    topdir = File.expand_path(topdir)
    target_build_dir = options[:target_build_dir]
    $INCFLAGS = "-I#{File.join(topdir, target_srcdir)} #{$INCFLAGS}"
    if target_build_dir
      $INCFLAGS = "-I#{File.join(topdir, target_build_dir)} #{$INCFLAGS}"
    end
    target_build_dir ||= target_srcdir

    if /cygwin|mingw/ =~ RUBY_PLATFORM
      $libs << " -lruby-#{target_name}"
      $LDFLAGS << " -L#{topdir}/#{target_build_dir}"
    elsif /mswin32/ =~ RUBY_PLATFORM
      $DLDFLAGS << " /libpath:#{topdir}/#{target_build_dir}"
      $libs << " libruby-#{target_name}.lib"
    end
  end
end

def add_distcleanfile(file)
  $distcleanfiles ||= []
  $distcleanfiles << file
end

def create_pkg_config_file(package_name, c_package,
                           version=nil, pc_file_name=nil)
  pc_file_name ||= "#{package_name.downcase.sub(/\//, '-')}.pc"
  version ||= ruby_gnome2_version || PKGConfig.modversion(c_package)

  puts "creating #{pc_file_name}"

  File.open(pc_file_name, 'w') do |pc_file|
    if package_name.nil?
      c_module_name = PKGConfig.name(c_package)
      package_name = "Ruby/#{c_module_name}" if c_module_name
    end
    pc_file.puts("Name: #{package_name}") if package_name

    description = PKGConfig.description(c_package)
    pc_file.puts("Description: Ruby bindings for #{description}") if description
    pc_file.printf("Version: #{version}")
  end
end

def ruby_gnome2_version(glib_source_directory=nil)
  glib_source_directory ||= File.join(File.dirname(__FILE__), "..")
  rbglib_h = File.join(glib_source_directory, "rbglib.h")
  return nil unless File.exist?(rbglib_h)

  version = nil
  File.open(rbglib_h) do |h_file|
    version_info = {}
    h_file.each_line do |line|
      case line
      when /\A#define RBGLIB_(MAJOR|MINOR|MICRO)_VERSION\s+(\d+)/
        version_info[$1] = $2
      end
    end
    version_info = [version_info["MAJOR"],
                    version_info["MINOR"],
                    version_info["MICRO"]].compact
    version = version_info.join(".") if version_info.size == 3
  end

  version
end

def create_makefile_at_srcdir(pkg_name, srcdir, defs = nil)
  base_dir = File.basename(Dir.pwd)
  last_common_index = srcdir.rindex(base_dir)
  if last_common_index
    builddir = srcdir[(last_common_index + base_dir.size + 1)..-1]
  end
  builddir ||= "."
  FileUtils.mkdir_p(builddir)

  Dir.chdir(builddir) do
    yield if block_given?

    $defs << defs if defs

    pattern = "*.{#{SRC_EXT.join(',')}}"
    srcs = Dir[File.join(srcdir, pattern)]
    srcs |= Dir[File.join(".", pattern)]
    $objs = srcs.collect do |src|
      File.basename(src, ".*") + ".o"
    end.flatten.uniq

    create_makefile(pkg_name, srcdir)
  end
end

def run_make_in_sub_dirs_command(command, sub_dirs)
  if /mswin32/ =~ RUBY_PLATFORM
    sub_dirs.collect do |dir|
      <<-EOM.chmop
	@cd #{dir}
	@nmake -nologo DESTDIR=$(DESTDIR) #{command}
	@cd ..
      EOM
    end.join("\n")
  else
    sub_dirs.collect do |dir|
      "\t@cd #{dir}; $(MAKE) #{command}"
    end.join("\n")
  end
end

def create_top_makefile(sub_dirs=["src"])
  File.open("Makefile", "w") do |makefile|
    makefile.print(<<-EOM)
all:
#{run_make_in_sub_dirs_command("all", sub_dirs)}

install:
#{run_make_in_sub_dirs_command("install", sub_dirs)}

site-install:
#{run_make_in_sub_dirs_command("site-install", sub_dirs)}

clean:
#{run_make_in_sub_dirs_command("clean", sub_dirs)}
    EOM

    if /mswin32/ =~ RUBY_PLATFORM
      makefile.print(<<-EOM)
	@if exist extconf.h del extconf.h
	@if exist conftest.* del conftest.*
	@if exist *.lib del *.lib
	@if exist *~ del *~
	@if exist mkmf.log del mkmf.log
      EOM
    else
      makefile.print(<<-EOM)

distclean: clean
#{run_make_in_sub_dirs_command("distclean", sub_dirs)}
	@rm -f Makefile extconf.h conftest.*
	@rm -f core *~ mkmf.log
      EOM
    end
  end
end

# This is used for the library which doesn't support version info.
def make_version_header(app_name, pkgname, dir = "src")
  version = PKGConfig.modversion(pkgname).split(/\./)
  (0..2).each do |v|
    version[v] = "0" unless version[v]
  end
  filename = "rb#{app_name.downcase}version.h"

  puts "creating #{filename}"

  add_distcleanfile(filename)

  FileUtils.mkdir_p(dir)
  out = File.open(File.join(dir, filename), "w")

  out.print %Q[/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  #{filename} -

  This file was generated by mkmf-gnome2.rb.

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
  #Other options
  ruby_header = "ruby.h"
  have_func("rb_define_alloc_func", ruby_header) # for ruby-1.8
  have_func("rb_block_proc", ruby_header) # for ruby-1.8

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

def glib_mkenums(prefix, files, g_type_prefix, include_files, options={})
  add_distcleanfile(prefix + ".h")
  add_distcleanfile(prefix + ".c")
  GLib::MkEnums.create(prefix, files, g_type_prefix, include_files, options)

  if $objs.nil?
    source_dir = '$(srcdir)'
    RbConfig.expand(source_dir)

    $objs = []
    srcs = Dir[File.join(source_dir, "*.{#{SRC_EXT.join(',')}}")]
    srcs.each do |f|
      obj = File.basename(f, ".*") << ".o"
      add_obj(obj)
    end
  end
  add_obj("#{prefix}.o")
end

def check_cairo(options={})
  return false unless PKGConfig.have_package('cairo')

  rcairo_source_dir = options[:rcairo_source_dir]
  if rcairo_source_dir and !File.exist?(rcairo_source_dir)
    rcairo_source_dir = nil
  end
  $CFLAGS += " -I#{rcairo_source_dir}/ext/cairo" if rcairo_source_dir
  have_rb_cairo_h = have_header('rb_cairo.h')
  unless have_rb_cairo_h
    begin
      require 'rubygems'
      gem 'cairo'
      require 'cairo'
      rcairo_src_gem_path_re =
        /\A#{Regexp.escape(Gem.dir)}\/gems\/cairo-[\d.]+\/ext\/cairo\z/
      $LOAD_PATH.each do |path|
        if rcairo_src_gem_path_re =~ path
          $CFLAGS += " -I#{path} "
          have_rb_cairo_h = have_header('rb_cairo.h')
          break
        end
      end
    rescue LoadError
    end
  end

  if have_rb_cairo_h
    if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
      options = {}
      build_dir = "tmp/#{RUBY_PLATFORM}/cairo/#{RUBY_VERSION}"
      if File.exist?(File.join(rcairo_source_dir, build_dir))
        options[:target_build_dir] = build_dir
      end
      add_depend_package("cairo", "ext/cairo", rcairo_source_dir, options)
      $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
    end
  end
  have_rb_cairo_h
end

add_include_path = Proc.new do |dir_variable|
  value = RbConfig::CONFIG[dir_variable]
  if value and File.exist?(value)
    $INCFLAGS << " -I$(#{dir_variable}) "
  end
end

add_include_path.call("sitearchdir")
add_include_path.call("vendorarchdir")

check_ruby_func

if /mingw/ =~ RUBY_PLATFORM
  $ruby.gsub!('\\', '/')
end


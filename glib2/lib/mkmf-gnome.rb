# Copyright (C) 2003-2019 Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require 'English'
require 'mkmf'
require 'pkg-config'
require 'glib-mkenums'
require "native-package-installer"

$CFLAGS += " #{ENV['CFLAGS']}" if ENV['CFLAGS']

def gcc?
  CONFIG["GCC"] == "yes"
end

def disable_optimization_build_flag(flags)
  if gcc?
    RbConfig.expand(flags.dup).gsub(/(^|\s)?-O\d(\s|$)?/, '\\1-Og\\2')
  else
    flags
  end
end

def enable_debug_build_flag(flags)
  if gcc?
    expanded_flags = RbConfig.expand(flags.dup)
    debug_option_pattern = /(^|\s)-g(?:gdb)?\d?(\s|$)/
    if debug_option_pattern =~ expanded_flags
      expanded_flags.gsub(debug_option_pattern, '\\1-ggdb3\\2')
    else
      flags + " -ggdb3"
    end
  else
    flags
  end
end

checking_for(checking_message("--enable-debug-build option")) do
  enable_debug_build = enable_config("debug-build", false)
  if enable_debug_build
    $CFLAGS = disable_optimization_build_flag($CFLAGS)
    $CFLAGS = enable_debug_build_flag($CFLAGS)

    $CXXFLAGS = disable_optimization_build_flag($CXXFLAGS)
    $CXXFLAGS = enable_debug_build_flag($CXXFLAGS)
  end
  enable_debug_build
end

def try_compiler_option(opt, &block)
  checking_for "#{opt} option to compiler" do
    if try_compile '', opt + " -Werror", &block
      $CFLAGS += " #{opt}"
      true
    else
      false
    end
  end
end

try_compiler_option '-Wall'
try_compiler_option '-Waggregate-return'
try_compiler_option '-Wcast-align'
# NOTE: Generates way too many false positives.
# try_compiler_option '-Wconversion'
try_compiler_option '-Wextra'
try_compiler_option '-Wformat=2'
try_compiler_option '-Winit-self'
# NOTE: This generates warnings for functions defined in ruby.h.
# try_compiler_option '-Winline'
try_compiler_option '-Wlarger-than-65500'
try_compiler_option '-Wmissing-declarations'
try_compiler_option '-Wmissing-format-attribute'
try_compiler_option '-Wmissing-include-dirs'
try_compiler_option '-Wmissing-noreturn'
try_compiler_option '-Wmissing-prototypes'
try_compiler_option '-Wnested-externs'
try_compiler_option '-Wold-style-definition'
try_compiler_option '-Wpacked'
try_compiler_option '-Wp,-D_FORTIFY_SOURCE=2'
try_compiler_option '-Wpointer-arith'
# NOTE: ruby.h and intern.h have too many of these.
# try_compiler_option '-Wredundant-decls'
# NOTE: Complains about index, for example.
# try_compiler_option '-Wshadow'
try_compiler_option '-Wundef'
# NOTE: Incredible amounts of false positives.
#try_compiler_option '-Wunreachable-code'
try_compiler_option '-Wout-of-line-declaration'
try_compiler_option '-Wunsafe-loop-optimizations'
try_compiler_option '-Wwrite-strings'

if /-Wl,--no-undefined/ =~ $LDFLAGS.to_s
  $LDFLAGS.gsub!(/-Wl,--no-undefined/, '')
end

include_path = nil
if ENV['GTK_BASEPATH'] and /cygwin/ !~ RUBY_PLATFORM
  include_path = (ENV['GTK_BASEPATH'] + "\\INCLUDE").gsub("\\", "/")
#  $hdrdir += " -I#{include_path} "
  $INCFLAGS += " -I#{include_path} "
end

def windows_platform?
  /cygwin|mingw|mswin/ === RUBY_PLATFORM
end

# For backward compatibility
def setup_windows(target_name, base_dir=nil)
  checking_for(checking_message("Windows")) do
    windows_platform?
  end
end
# For backward compatibility
def setup_win32(*args, &block)
  setup_windows(*args, &block)
end

def find_gem_spec(package)
  begin
    Gem::Specification.find_by_name(package)
  rescue LoadError
    nil
  end
end

def setup_homebrew
  checking_for(checking_message("Homebrew")) do
    platform = NativePackageInstaller::Platform.detect
    if platform.is_a?(NativePackageInstaller::Platform::Homebrew)
      libffi_prefix = `brew --prefix libffi`.chomp
      PKGConfig.add_path("#{libffi_prefix}/lib/pkgconfig")
      true
    else
      false
    end
  end
end

setup_homebrew

#add_depend_package("glib2", "ext/glib2", "/...../ruby-gnome2")
def add_depend_package(target_name, target_srcdir, top_srcdir, options={})
  gem_spec = find_gem_spec(target_name)
  if gem_spec
    target_source_dir = File.join(gem_spec.full_gem_path, "ext/#{target_name}")
    target_build_dir = target_source_dir
    add_depend_package_path(target_name,
                            target_source_dir,
                            target_build_dir)
  end

  [top_srcdir,
   File.join(top_srcdir, target_name),
   $configure_args['--topdir'],
   File.join($configure_args['--topdir'], target_name)].each do |topdir|
    topdir = File.expand_path(topdir)
    target_source_dir_full_path = File.join(topdir, target_srcdir)

    top_build_dir = options[:top_build_dir] || topdir
    target_build_dir = options[:target_build_dir] || target_srcdir
    target_build_dir_full_path = File.join(top_build_dir, target_build_dir)
    unless File.exist?(target_build_dir_full_path)
      target_build_dir_full_path = File.join(top_build_dir, target_srcdir)
    end
    unless File.exist?(target_build_dir_full_path)
      target_build_dir_full_path = File.join(topdir, target_build_dir)
    end
    unless File.exist?(target_build_dir_full_path)
      target_build_dir_full_path = File.join(topdir, target_srcdir)
    end
    add_depend_package_path(target_name,
                            target_source_dir_full_path,
                            target_build_dir_full_path)
  end
end

def add_depend_package_path(target_name, target_source_dir, target_build_dir)
  if File.exist?(target_source_dir)
    $INCFLAGS = "-I#{target_source_dir} #{$INCFLAGS}"
  end

  return unless File.exist?(target_build_dir)
  if target_source_dir != target_build_dir
    $INCFLAGS = "-I#{target_build_dir} #{$INCFLAGS}"
  end

  library_base_name = File.basename(target_source_dir).gsub(/-/, "_")
  case RUBY_PLATFORM
  when /cygwin|mingw/
    $libs << " " << File.join(target_build_dir, "#{library_base_name}.so")
  when /mswin/
    $DLDFLAGS << " /libpath:#{target_build_dir}"
    $libs << " #{library_base_name}-$(arch).lib"
  end
end

def add_distcleanfile(file)
  $distcleanfiles ||= []
  $distcleanfiles << file
end

def create_pkg_config_file(package_name, c_package,
                           version=nil, pc_file_name=nil)
  pc_file_name ||= "#{package_name.downcase.sub(/\//, '-')}.pc"
  version ||= ruby_gnome_version || PKGConfig.modversion(c_package)

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

  add_distcleanfile(pc_file_name)
end

def ruby_gnome_version(glib_source_directory=nil)
  glib_source_directory ||= File.join(File.dirname(__FILE__), "..",
                                      "ext", "glib2")
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

def ruby_gnome2_version(*args)
  warn("ruby_gnome2_version is deprecated. Use ruby_gnome_version instead.")
  ruby_gnome_version(*args)
end

def ensure_objs
  return unless $objs.nil?

  source_dir = '$(srcdir)'
  RbConfig.expand(source_dir)

  pattern = "*.{#{SRC_EXT.join(',')}}"
  srcs = Dir[File.join(source_dir, pattern)]
  srcs |= Dir[File.join(".", pattern)]
  $objs = srcs.collect do |f|
    File.basename(f, ".*") + ".o"
  end.uniq
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
    ensure_objs
    create_makefile(pkg_name, srcdir)
  end
end

def run_make_in_sub_dirs_command(command, sub_dirs)
  if /mswin/ =~ RUBY_PLATFORM
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

    if /mswin/ =~ RUBY_PLATFORM
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
    if /\A(\d+)/ =~ version[v]
      number = $1
      tag = $POSTMATCH
      unless tag.empty?
        version[v] = number
        version[3] = tag
      end
    end
  end
  filename = "rb#{app_name.downcase}version.h"

  puts "creating #{filename}"

  add_distcleanfile(filename)

  FileUtils.mkdir_p(dir)
  out = File.open(File.join(dir, filename), "w")

  version_definitions = []
  ["MAJOR", "MINOR", "MICRO", "TAG"].each_with_index do |type, i|
    _version = version[i]
    next if _version.nil?
    version_definitions << "#define #{app_name}_#{type}_VERSION (#{_version})"
  end
  out.print %Q[/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  #{filename} -

  This file was generated by mkmf-gnome.rb.

************************************************/

#ifndef __RB#{app_name}_VERSION_H__
#define __RB#{app_name}_VERSION_H__

#{version_definitions.join("\n")}

#define #{app_name}_CHECK_VERSION(major,minor,micro)    \\
    (#{app_name}_MAJOR_VERSION > (major) || \\
     (#{app_name}_MAJOR_VERSION == (major) && #{app_name}_MINOR_VERSION > (minor)) || \\
     (#{app_name}_MAJOR_VERSION == (major) && #{app_name}_MINOR_VERSION == (minor) && \\
      #{app_name}_MICRO_VERSION >= (micro)))


#endif /* __RB#{app_name}_VERSION_H__ */
]
      out.close
end

def add_obj(name)
  ensure_objs
  $objs << name unless $objs.index(name)
end

def glib_mkenums(prefix, files, g_type_prefix, include_files, options={})
  add_distcleanfile(prefix + ".h")
  add_distcleanfile(prefix + ".c")
  GLib::MkEnums.create(prefix, files, g_type_prefix, include_files, options)
  add_obj("#{prefix}.o")
end

def check_cairo(options={})
  rcairo_source_dir = options[:rcairo_source_dir]
  if rcairo_source_dir.nil?
    rcairo_source_base_dir = "rcairo"
    top_dir = options[:top_dir]
    if top_dir
      rcairo_source_dir = File.join(top_dir, "..", rcairo_source_base_dir)
    end
  end

  if rcairo_source_dir and !File.exist?(rcairo_source_dir)
    rcairo_source_dir = nil
  end
  if rcairo_source_dir.nil?
    cairo_gem_spec = find_gem_spec("cairo")
    rcairo_source_dir = cairo_gem_spec.full_gem_path if cairo_gem_spec
  end

  unless rcairo_source_dir.nil?
    options = {}
    build_dir = "tmp/#{RUBY_PLATFORM}/cairo/#{RUBY_VERSION}"
    if File.exist?(File.join(rcairo_source_dir, build_dir))
      options[:target_build_dir] = build_dir
    end
    add_depend_package("cairo", "ext/cairo", rcairo_source_dir, options)
  end

  PKGConfig.have_package("cairo") and have_header("rb_cairo.h")
end

def install_missing_native_package(native_package_info)
  NativePackageInstaller.install(native_package_info)
end

def required_pkg_config_package(package_info, native_package_info=nil)
  if package_info.is_a?(Array)
    required_package_info = package_info
  else
    required_package_info = [package_info]
  end
  return true if PKGConfig.have_package(*required_package_info)

  native_package_info ||= {}
  return false unless install_missing_native_package(native_package_info)

  PKGConfig.have_package(*required_package_info)
end

add_include_path = Proc.new do |dir_variable|
  value = RbConfig::CONFIG[dir_variable]
  if value and File.exist?(value)
    $INCFLAGS << " -I$(#{dir_variable}) "
  end
end

add_include_path.call("sitearchdir")
add_include_path.call("vendorarchdir")

if /mingw/ =~ RUBY_PLATFORM
  $ruby.gsub!('\\', '/')
end

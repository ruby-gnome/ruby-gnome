#
# mkmf-gnome2.rb
#
# Extended mkmf for Ruby-GNOME2 and Ruby/GLib based libraries.
#
# Copyright(C) 2003-2011 Ruby-GNOME2 Project.
#
# This program is licenced under the same
# license of Ruby-GNOME2.
#

require 'English'
require 'mkmf'
begin
  require 'pkg-config'
rescue LoadError
  require 'rubygems'
  gem 'pkg-config'
  require 'pkg-config'
end
require 'glib-mkenums'

$CFLAGS += " #{ENV['CFLAGS']}" if ENV['CFLAGS']

def try_compiler_option(opt, &block)
  checking_for "#{opt} option to compiler" do
    $CFLAGS += " #{opt}" if try_compile '', opt, &block
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
try_compiler_option '-Wswitch-default'
try_compiler_option '-Wswitch-enum'
try_compiler_option '-Wundef'
# NOTE: Incredible amounts of false positives.
#try_compiler_option '-Wunreachable-code'
try_compiler_option '-Wunsafe-loop-optimizations'
try_compiler_option '-Wwrite-strings'

if /-Wl,--no-undefined/ =~ $LDFLAGS.to_s
  $LDFLAGS.gsub!(/-Wl,--no-undefined/, '')
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
      if binary_base_dir.exist?
        $CFLAGS += " -I#{binary_base_dir}/include"
        pkg_config_dir = binary_base_dir + "lib" + "pkgconfig"
        PKGConfig.add_path(pkg_config_dir.to_s)
      end
      true
    else
      false
    end
  end
end

def find_gem_spec(package)
  begin
    require 'rubygems'
    if Gem::Specification.respond_to?(:find_by_name)
      Gem::Specification.find_by_name(package)
    else
      Gem.source_index.find_name(package).last
    end
  rescue LoadError
    nil
  end
end

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

  case RUBY_PLATFORM
  when /cygwin|mingw|mswin32/
    case RUBY_PLATFORM
    when /cygwin|mingw/
      $LDFLAGS << " -L#{target_build_dir}"
      $libs << " -lruby-#{target_name}"
    when /mswin32/
      $DLDFLAGS << " /libpath:#{target_build_dir}"
      $libs << " libruby-#{target_name}.lib"
    end
    target_base_dir = Pathname.new(target_source_dir).parent.parent
    target_binary_base_dir = target_base_dir + "vendor" + "local"
    if target_binary_base_dir.exist?
      $INCFLAGS = "-I#{target_binary_base_dir}/include #{$INCFLAGS}"
      target_pkg_config_dir = target_binary_base_dir + "lib" + "pkgconfig"
      PKGConfig.add_path(target_pkg_config_dir.to_s)
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

  add_distcleanfile(pc_file_name)
end

def ruby_gnome2_version(glib_source_directory=nil)
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

  This file was generated by mkmf-gnome2.rb.

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
  if rcairo_source_dir and !File.exist?(rcairo_source_dir)
    rcairo_source_dir = nil
  end
  if rcairo_source_dir.nil?
    cairo_gem_spec = find_gem_spec("cairo")
    rcairo_source_dir = cairo_gem_spec.full_gem_path if cairo_gem_spec
  end

  unless rcairo_source_dir.nil?
    if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
      options = {}
      build_dir = "tmp/#{RUBY_PLATFORM}/cairo/#{RUBY_VERSION}"
      if File.exist?(File.join(rcairo_source_dir, build_dir))
        options[:target_build_dir] = build_dir
      end
      add_depend_package("cairo", "ext/cairo", rcairo_source_dir, options)
      $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
    end
    $CFLAGS += " -I#{rcairo_source_dir}/ext/cairo"
  end

  PKGConfig.have_package('cairo') and have_header('rb_cairo.h')
end

def package_platform
  if File.exist?("/etc/debian_version")
    :debian
  elsif File.exist?("/etc/fedora-release")
    :fedora
  elsif File.exist?("/etc/redhat-release")
    :redhat
  elsif find_executable("brew")
    :homebrew
  elsif find_executable("port")
    :macports
  else
    :unknown
  end
end

def super_user?
  Process.uid.zero?
end

def normalize_native_package_info(native_package_info)
  native_package_info ||= {}
  native_package_info = native_package_info.dup
  native_package_info[:fedora] ||= native_package_info[:redhat]
  native_package_info
end

def install_missing_native_package(native_package_info)
  platform = package_platform
  native_package_info = normalize_native_package_info(native_package_info)
  package = native_package_info[platform]
  return false if package.nil?

  need_super_user_priviledge = true
  case platform
  when :debian
    install_command = "apt-get install -V -y #{package}"
  when :fedora, :redhat
    install_command = "yum install -y #{package}"
  when :homebrew
    need_super_user_priviledge = false
    install_command = "brew install #{package}"
  when :macports
    install_command = "port install -y #{package}"
  else
    return false
  end

  have_priviledge = (not need_super_user_priviledge or super_user?)
  unless have_priviledge
    sudo = find_executable("sudo")
  end

  installing_message = "installing '#{package}' native package... "
  message("%s", installing_message)
  failed_to_get_super_user_priviledge = false
  if have_priviledge
    succeeded = xsystem(install_command)
  else
    if sudo
      prompt = "[sudo] password for %u to install <#{package}>: "
      sudo_options = "-p #{Shellwords.escape(prompt)}"
      install_command = "#{sudo} #{sudo_options} #{install_command}"
      succeeded = xsystem(install_command)
    else
      succeeded = false
      failed_to_get_super_user_priviledge = true
    end
  end

  if failed_to_get_super_user_priviledge
    result_message = "require super user privilege"
  else
    result_message = succeeded ? "succeeded" : "failed"
  end
  Logging.postpone do
    "#{installing_message}#{result_message}\n"
  end
  message("#{result_message}\n")

  error_message = nil
  unless succeeded
    if failed_to_get_super_user_priviledge
      error_message = <<-EOM
'#{package}' native package is required.
run the following command as super user to install required native package:
  \# #{install_command}
EOM
    else
      error_message = <<-EOM
failed to run '#{install_command}'.
EOM
    end
  end
  if error_message
    message("%s", error_message)
    Logging.message("%s", error_message)
  end

  Logging.message("--------------------\n\n")

  succeeded
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

check_ruby_func

if /mingw/ =~ RUBY_PLATFORM
  $ruby.gsub!('\\', '/')
end


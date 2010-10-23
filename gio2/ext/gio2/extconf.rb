=begin
extconf.rb for Ruby/GIO extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)(?:\.\d+)?\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gio2"
package_id = "gio-2.0"

require 'mkmf-gnome2'

win32 = setup_win32(module_name, base_dir)

defines = '-DRUBY_GIO2_COMPILATION'

PKGConfig.have_package(package_id) or exit 1
unless win32
  PKGConfig.have_package('gio-unix-2.0') and defines += ' -DHAVE_GIO_UNIX'
end
PKGConfig.have_package('gobject-2.0') or exit 1

have_func('rb_exec_recursive')

["glib2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

def try_compiler_option(opt, &block)
  checking_for "#{opt} option to compiler" do
    $CFLAGS += " #{opt}" if try_compile '', opt, &block
  end
end

try_compiler_option '-finline-functions'
try_compiler_option '-fno-common'
try_compiler_option '-Wall'
try_compiler_option '-Waggregate-return'
try_compiler_option '-Wcast-align'
# NOTE: Generates way too many false positives.
# try_compiler_option '-Wconversion'
try_compiler_option '-Wextra'
try_compiler_option '-Wformat=2'
try_compiler_option '-Winit-self'
try_compiler_option '-Winline'
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

create_pkg_config_file("Ruby/GIO2", package_id)

$defs << defines
create_makefile(module_name)

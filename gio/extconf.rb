=begin
extconf.rb for Ruby/GIO extention library
=end

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gio/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

PACKAGE_NAME = 'gio2'

require 'mkmf-gnome2'

PKGConfig.have_package('gio-2.0') or exit 1
PKGConfig.have_package('gobject-2.0') or exit 1

have_func('rb_exec_recursive')

setup_win32(PACKAGE_NAME)

add_depend_package('glib2', 'glib/src', TOPDIR)

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
try_compiler_option '-Wsign-compare'
try_compiler_option '-Wstrict-aliasing=2'
# NOTE: ruby.h and intern.h are messed up and for some reason GCC wants to
# complain about them as well.  System headers?
try_compiler_option '-Wswitch-default'
try_compiler_option '-Wswitch-enum'
try_compiler_option '-Wundef'
# NOTE: Incredible amounts of false positives.
#try_compiler_option '-Wunreachable-code'
try_compiler_option '-Wunsafe-loop-optimizations'
try_compiler_option '-Wwrite-strings'

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, '-DRUBY_GIO2_COMPILATION')
create_top_makefile

require 'mkmf'

unless defined? macro_defined?
  def macro_defined?(macro, src, opt="")
    try_cpp(src + <<EOP, opt)
#ifndef #{macro}
# error
#endif
EOP
  end
end


while /^--/ =~ ARGV[0]
  ARGV.shift
end

rubyglib_dir = ARGV.shift || File.dirname(__FILE__)+"/../glib"
unless FileTest.exist?(rubyglib_dir)
  raise "Directory #{rubyglib_dir} not found.  Please specify Ruby/GLib source dir."
end
$CFLAGS += " -I#{rubyglib_dir}/src "

rubygtk_dir = ARGV.shift || File.dirname(__FILE__)+"/../gtk"
unless FileTest.exist?(rubyglib_dir)
  raise "Directory #{rubygtk_dir} not found.  Please specify Ruby/Gtk source dir."
end
$CFLAGS += " -I#{rubygtk_dir}/src "

unless system('pkg-config', '--exists', 'gdk-pixbuf-2.0')
  STDERR.print("gdk-pixbuf-2.0 doesn't exist\n")
  exit
end
$LDFLAGS += ' ' + `pkg-config gdk-pixbuf-2.0 --libs`.chomp
$CFLAGS  += ' ' + `pkg-config gdk-pixbuf-2.0 --cflags`.chomp

unless system('pkg-config', '--exists', 'gtk+-2.0')
  STDERR.print("gtk+-2.0 doesn't exist\n")
  exit
end
$LDFLAGS += ' ' + `pkg-config gtk+-2.0 --libs`.chomp
$CFLAGS  += ' ' + `pkg-config gtk+-2.0 --cflags`.chomp

$CFLAGS += ' -g'

STDOUT.print("checking for G_OS_WIN32... ")
STDOUT.flush
if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
  STDOUT.print "yes\n"
  $CFLAGS += ' -fnative-struct' if /gcc/ =~ Config::CONFIG['CC']
else
  STDOUT.print "no\n"
end

have_func("g_print") &&
have_func("gdk_init") &&
have_func("gdk_pixbuf_new") &&
create_makefile('gdk_pixbuf2')

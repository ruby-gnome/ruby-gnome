require 'mkmf'

$objs = ["rbglade.o"]

dir_config("glade")
dir_config("xml")
dir_config("z")

#
# detect libglade configurations
#
config_cmds = ["libglade-config"]
while /^--/ =~ ARGV[0]
  ARGV.shift
end
if ARGV.size > 0
  config_cmds.unshift(ARGV[0])
end

begin
  config_cmds.each do |config_cmd|
    version = `#{config_cmd} --version`
    if not version.chomp.empty?
      config_libs, config_cflags = "--libs", "--cflags"
      $LDFLAGS, *libs = `#{config_cmd} #{config_libs}`.chomp.split(/(-l.*)/)
      $libs = libs.join(' ') + ' ' + $libs
      $CFLAGS = `#{config_cmd} #{config_cflags}`.chomp
      break
    end
  end
rescue
  $LDFLAGS = '-L/usr/X11R6/lib -L/usr/local/lib'
  $CFLAGS = '-I/usr/X11R6/lib -I/usr/local/include'
  $libs = '-lm -lc'
end

["glade", "xml", "z"].each do |lib|
	unless have_library(lib)
		puts "\n***"
		puts "Could not find lib#{lib}."
		puts "You can specify its path using --with-#{lib}-lib=/path/"
		exit
	end
end

create_makefile("lglade")

require "mkmf"

$objs = ["rbglade.o"]
my_libs = ["glade", "xml", "z"]

$use_gnome = with_config("gnome")
puts "GNOME support is: #{$use_gnome ? 'enabled' : 'disabled'}."
my_libs.push("glade-gnome") if $use_gnome

my_libs.each do |lib|
  dir_config(lib)
end

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
      puts "Using #{version.chomp}"
      config_libs, config_cflags = "--libs #{'gnome' if $use_gnome}", "--cflags"
      $LDFLAGS, *libs = `#{config_cmd} #{config_libs}`.chomp.split(/(-l.*)/)
      $libs = libs.join(' ') + ' ' + $libs
      $CFLAGS = `#{config_cmd} #{config_cflags}`.chomp
      $CFLAGS << " -DENABLE_GNOME" if $use_gnome
      break
    end
  end
rescue
  $LDFLAGS = '-L/usr/X11R6/lib -L/usr/local/lib'
  $CFLAGS = '-I/usr/X11R6/lib -I/usr/local/include'
  $libs = '-lm -lc'
end

my_libs.each do |lib|
	unless have_library(lib)
		puts "\n***"
		puts "Could not find lib#{lib}."
		puts "You can specify its path using --with-#{lib}-lib=/path/"
		exit
	end
end

create_makefile("lglade")

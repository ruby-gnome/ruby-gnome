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
config_cmd = with_config("libglade-config", "libglade-config")
while /^--/ =~ ARGV[0]
  ARGV.shift
end

begin
  version = `#{config_cmd} --version`
  if not version.chomp.empty?
    $CFLAGS += ' ' + `#{config_cmd} --cflags`.chomp
    $LDFLAGS += ' ' + `#{config_cmd} --libs`.chomp
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

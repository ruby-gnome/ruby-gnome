=begin
extconf.rb for Ruby/Libgda extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

unless PKGConfig.respond_to?(:or_newer?)
  module PKGConfig
    module_function
    def or_newer?(pkg, version)
      STDOUT.print("checking for #{pkg} version (>= #{version})... ")
      STDOUT.flush
      mod_version = `#{@@cmd} --modversion #{pkg}`.chomp
      result = (mod_version.split(".") <=> version.split(".")) >= 0
      result_message = result ? "yes" : "no"
      STDOUT.print "#{result_message}\n"
      result
    end
  end
end

pkgnames   = ['libgnomeprint-2.2']

require_version = {
  "libgnomeprint-2.2" => "2.8",
}

pkgnames.each do |pkgname|
	PKGConfig.have_package(pkgname) or exit 1
  if require_version.has_key?(pkgname) and
      !PKGConfig.or_newer?(pkgname, require_version[pkgname])
    exit 1
  end
end

check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
  ].each{|d,l|
    $libs << " -l#{l}"
    $LDFLAGS << " -L#{top}/#{d}"
  }
end

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("gnomeprint2", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile

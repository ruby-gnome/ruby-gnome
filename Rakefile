# -*- ruby -*-

repository_base_url = "https://ruby-gnome2.svn.sourceforge.net/svnroot/ruby-gnome2/ruby-gnome2"

task :default => :build

def version
  @version ||= ENV["VERSION"] || guess_version
end

def guess_version
  versions = {}
  File.open("glib/src/rbglib.h") do |rbglib_h|
    rbglib_h.each_line do |line|
      if /#define\s+RBGLIB_([A-Z]+)_VERSION\s+(\d+)/ =~ line
        versions[$1.downcase] = $2.to_i
      end
    end
  end
  ["major", "minor", "micro"].collect {|type| versions[type]}.compact.join(".")
end

@needless_paths = [".svn", ".test-result", ".gdb_history", "mkmf.log"]
def package(base_name, paths, needless_paths=nil)
  dist_dir = "#{base_name}-#{version}"

  rm_rf(dist_dir)
  mkdir_p(dist_dir)
  cp_r(paths, dist_dir, :preserve => true)

  needless_paths ||= @needless_paths
  Dir.glob("#{dist_dir}/**/{#{needless_paths.join(',')}}") do |needless_path|
    rm_rf(needless_path)
  end
  sh("tar", "cvfz", "#{dist_dir}.tar.gz", dist_dir)
ensure
  rm_rf(dist_dir)
end

task :configure do
  ruby("extconf.rb")
end

file "Makefile" do
  task(:configure).invoke
end

task :build => ["Makefile"] do
  sh("make")
end

task :clean do
  sh("make", "clean") if File.exist?("Makefile")
end

task :distclean do
  sh("make", "distclean") if File.exist?("Makefile")
end

task :test => [:build] do
  ruby("run-tests.rb")
end

task :dist => [:dist_gtk2, :dist_gnome2]

base_files = ["AUTHORS", "COPYING.LIB", "ChangeLog", "NEWS",
              "README", "Rakefile",
              "exec_make.rb", "extconf.rb", "run-tests.rb"]
gtk2_dirs = ["glib", "atk", "pango", "gdkpixbuf", "gtk"]
task :dist_gtk2 do
  package("ruby-gtk2", base_files + gtk2_dirs)
end

gnome2_dirs = gtk2_dirs + ["bonobo", "bonoboui", "gconf", "goocanvas", "gnome",
                           "gnomecanvas", "gnomeprint", "gnomeprintui",
                           "gnomevfs", "gstreamer", "gtkglext",
                           "gtkhtml2", "gtkmozembed", "gtksourceview",
                           "libart", "libglade",
                           "panel-applet", "poppler", "rsvg", "vte"]
task :dist_gnome2 do
  package("ruby-gnome2-all", base_files + gnome2_dirs)
end

task :tag do
  tagged_url = "#{repository_base_url}/tags/#{version}"

  success = false
  begin
    sh("svn", "ls", tagged_url)
    success = true
  rescue RuntimeError
  end
  raise "#{version} is already tagged" if success

  svn_commands = ["svn", "cp", "-m", "released #{version}!!!"]
  if ENV["SVN_USER"]
    svn_commands.configure(["--username", ENV["SVN_USER"]])
  end
  svn_commands.concat(["#{repository_base_url}/trunk", tagged_url])
  sh(*svn_commands)
end

# -*- ruby -*-

require "pathname"
require "open-uri"

repository_base_url = "https://ruby-gnome2.svn.sourceforge.net/svnroot/ruby-gnome2/ruby-gnome2"

task :default => :test

def version
  @version ||= ENV["VERSION"] || guess_version
end

def guess_version
  versions = {}
  File.open("glib2/ext/glib2/rbglib.h") do |rbglib_h|
    rbglib_h.each_line do |line|
      if /#define\s+RBGLIB_([A-Z]+)_VERSION\s+(\d+)/ =~ line
        versions[$1.downcase] = $2.to_i
      end
    end
  end
  ["major", "minor", "micro"].collect {|type| versions[type]}.compact.join(".")
end

def dist_dir(base_name, _version=nil)
  _version ||= version
  "#{base_name}-#{_version}"
end

def archive_name(base_name, _version=nil)
  dist_dir(base_name, _version) + ".tar.gz"
end

@needless_paths = [".svn", ".test-result", ".gdb_history", "mkmf.log",
                   "*.o", "*.so", "pkg", "vendor", "tmp"]
def package(base_name, paths, needless_paths=nil)
  _dist_dir = dist_dir(base_name)

  rm_rf(_dist_dir)
  mkdir_p(_dist_dir)
  cp_r(paths, _dist_dir, :preserve => true)

  needless_paths ||= @needless_paths
  Dir.glob("#{_dist_dir}/**/{#{needless_paths.join(',')}}") do |needless_path|
    rm_rf(needless_path)
  end
  sh("tar", "cvfz", archive_name(base_name), _dist_dir)
ensure
  rm_rf(_dist_dir) if _dist_dir
end

def download(url, local_path=nil)
  local_path ||= File.basename(url)
  open(url) do |remote_file|
    File.open(local_path, "wb") do |local_file|
      local_file.print(remote_file.read)
    end
  end
  local_path
end

desc "configure all packages"
task :configure do
  ruby("extconf.rb")
end

file "Makefile" do
  task(:configure).invoke
end

desc "build all packages"
task :build => ["Makefile"] do
  sh("make")
end

desc "clean all packages"
task :clean do
  sh("make", "clean") if File.exist?("Makefile")
end

desc "more clean all packages"
task :distclean do
  sh("make", "distclean") if File.exist?("Makefile")
end

desc "run tests for all packages"
task :test => [:build] do
  ruby("run-test.rb")
end

gtk2_base_name = "ruby-gtk2"
gnome2_base_name = "ruby-gnome2-all"

gtk2_packages = ["glib2", "gio2", "atk", "pango", "gdk_pixbuf2", "gtk2"]
gnome2_packages = gtk2_packages + ["gobject-introspection",
                                   "goocanvas", "gstreamer",
                                   "gtksourceview2", "poppler", "rsvg2", "vte"]
namespace :dist do
  base_files = ["AUTHORS", "COPYING.LIB", "NEWS",
                "README", "Rakefile",
                "exec_make.rb", "extconf.rb", "run-test.rb"]
  desc "make Ruby/GTK2 package"
  task :gtk2 do
    package(gtk2_base_name, base_files + gtk2_packages)
  end

  desc "make Ruby/GNOME2 package"
  task :gnome2 do
    package(gnome2_base_name, base_files + gnome2_packages)
  end

  ruby_versions = ["1.8.7-p371", "1.9.3-p327"]
  namespace :test do
    ruby_base_url = "ftp://ftp.ruby-lang.org/pub/ruby"
    ruby_versions.each do |ruby_version|
      base_dir = "tmp/dist-test-#{version}/#{ruby_version}"
      directory base_dir

      prefix = Pathname.new(base_dir) + "local"
      ruby_tar_bz2_base = "ruby-#{ruby_version}.tar.bz2"
      file "#{base_dir}/#{ruby_tar_bz2_base}" => base_dir do
        generation = ruby_version.scan(/\A\d\.\d/)[0]
        ruby_tar_bz2_url = "#{ruby_base_url}/#{generation}/#{ruby_tar_bz2_base}"
        Dir.chdir(base_dir) do
          download(ruby_tar_bz2_url)
        end
      end

      ruby_path = "#{prefix}/bin/ruby"
      expanded_ruby_path = File.expand_path(ruby_path)
      file ruby_path => "#{base_dir}/#{ruby_tar_bz2_base}" do
        expanded_prefix = prefix.expand_path
        Dir.chdir(base_dir) do
          sh("tar", "xvf", ruby_tar_bz2_base)
          Dir.chdir(File.basename(ruby_tar_bz2_base, ".tar.bz2")) do
            sh("./configure", "--prefix=#{expanded_prefix}")
            sh("make", "-j8")
            sh("make", "install-nodoc")
          end
        end
      end

      rubygems_tgz_base = "rubygems-1.8.24.tgz"
      rubygems_tgz = "#{base_dir}/#{rubygems_tgz_base}"
      file rubygems_tgz => base_dir do
        rubygems_url_base = "http://production.cf.rubygems.org/rubygems"
        Dir.chdir(base_dir) do
          download("#{rubygems_url_base}/#{rubygems_tgz_base}")
        end
      end

      gem_path = "#{prefix}/bin/gem"
      expanded_gem_path = File.expand_path(gem_path)
      file gem_path => [ruby_path, rubygems_tgz] do
        Dir.chdir(base_dir) do
          sh("tar", "xvf", rubygems_tgz_base)
          Dir.chdir(File.basename(rubygems_tgz_base, ".tgz")) do
            sh(expanded_ruby_path, "setup.rb")
          end
        end
      end

      packages = {
        "gtk2" => {
          :archive_name => archive_name(gtk2_base_name),
          :packages => gtk2_packages,
        },
        "gnome2" => {
          :archive_name => archive_name(gnome2_base_name),
          :packages => gnome2_packages,
        },
      }
      namespace "ruby-#{ruby_version}" do
        packages.each do |name, attributes|
          namespace name do
            tar_gz = File.expand_path(attributes[:archive_name])
            task :prepare => ["dist:#{name}", gem_path] do
              Dir.chdir(base_dir) do
                sh("tar", "xvf", tar_gz)
              end
              if /cairo/ !~ `#{expanded_gem_path} list cairo`
                sh(expanded_gem_path, "install", "cairo")
              end
            end

            attributes[:packages].each do |sub_package_name|
              task sub_package_name => :prepare do
                Dir.chdir(base_dir) do
                  package_base_dir = File.basename(tar_gz, ".tar.gz")
                  rm_rf(sub_package_name)
                  sh("cp", "-a",
                     "#{package_base_dir}/#{sub_package_name}", sub_package_name)
                  Dir.chdir(sub_package_name) do
                    sh(expanded_ruby_path, "extconf.rb")
                    sh("make", "-j8")
                    sh("make", "install")
                  end
                  rm_rf(sub_package_name)
                end
              end
            end
          end
        end

        package_test_tasks = []
        packages.each do |name, attributes|
          tasks = attributes[:packages].collect do |sub_package_name|
            "dist:test:ruby-#{ruby_version}:#{name}:#{sub_package_name}"
          end
          package_test_tasks.concat(tasks)
          task name => package_test_tasks
        end
      end

      test_tasks = packages.collect do |name, attributes|
        "dist:test:ruby-#{ruby_version}:#{name}"
      end
      task "ruby-#{ruby_version}" => test_tasks
    end
  end

  test_tasks = ruby_versions.collect do |ruby_version|
    "dist:test:ruby-#{ruby_version}"
  end
  desc "run packages test"
  task :test => test_tasks
end
desc "make all packages"
task :dist => ["dist:gtk2", "dist:gnome2"]

directory "misc"
file "misc/release.rb" => "misc" do |task|
  release_rb = "https://raw.github.com/clear-code/cutter/master/misc/release.rb"
  open(release_rb) do |remote_release_rb|
    File.open(task.name, "w") do |local_release_rb|
      local_release_rb.print(remote_release_rb.read)
    end
  end
end

desc "release Ruby-GNOME2 packages"
task :release => ["misc/release.rb", :dist] do
  sf_user_name = ENV["SF_USER"] || ENV["USER"]
  project_id = "ruby-gnome2"
  project_name = "Ruby-GNOME 2"
  package_name = "ruby-gnome2"
  release_name = "ruby-gnome2-#{version}"
  archive_names = [gtk2_base_name, gnome2_base_name].collect do |base_name|
    archive_name(base_name)
  end
  ruby("misc/release.rb", sf_user_name, project_id, project_name,
       package_name, release_name, "README:1", "NEWS",
       *archive_names)
end

namespace :gem do
  desc "build all gems"
  task :build do
    gnome2_packages.each do |package|
      Dir.chdir(package) do
        ruby("-S", "rake", "gem")
      end
    end
  end

  desc "push all gems"
  task :push do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "push",
           *Dir.glob(File.join(package, "pkg", "*-#{version}.gem")))
    end
  end

  desc "install all gems"
  task :install do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "install", "--user-install",
           *Dir.glob(File.join(package, "pkg", "*-#{version}.gem")))
    end
  end

  desc "uninstall all gems"
  task :uninstall do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "uninstall", "--version", version, package)
    end
  end

  namespace :win32 do
    win32_unsupported_packages = ["gobject-introspection", "goocanvas", "vte"]
    win32_gnome2_packages = gnome2_packages - win32_unsupported_packages
    desc "build all Windows gems"
    task :build do
      win32_gnome2_packages.each do |package|
        Dir.chdir(package) do
          tasks = ["win32:build", "cross", "native", "gem"]
          ruby("-S", "rake", "RUBY_CC_VERSION=1.8.7:1.9.3", *tasks)
        end
      end
    end

    desc "clean all Windows gems build"
    task :clean do
      win32_gnome2_packages.each do |package|
        rm_rf(File.join(package, "tmp"))
      end
    end

    desc "download DLL for Windows all gems"
    task :download do
      win32_gnome2_packages.each do |package|
        Dir.chdir(package) do
          ruby("-S", "rake", "win32:download")
        end
      end
    end

    desc "push all Windows gems"
    task :push do
      win32_gnome2_packages.each do |package|
        ruby("-S", "gem", "push",
             *Dir.glob(File.join(package, "pkg", "*-#{version}-x86-mingw32.gem")))
      end
    end
  end
end

def guess_copy_source_repository_uri
  info_xml = `svn info --xml`
  if /<url>(.+?)<\/url>/ =~ info_xml
    $1
  else
    raise "can't find repository URI from:\n#{info_xml}"
  end
end

desc "tag the current release"
task :tag do
  sh("git", "tag", "-a", version, "-m", "release #{version}!!!")
end

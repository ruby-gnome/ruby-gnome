# -*- ruby -*-

require "pathname"
require "find"
require "tmpdir"
require "open-uri"

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

@needless_paths = [
  ".svn",
  ".test-result",
  ".gdb_history",
  "mkmf.log",
  "*.o",
  "*.so",
  "pkg",
  "vendor",
  "tmp",
]
def package(base_name, paths, needless_paths=nil)
  _dist_dir = dist_dir(base_name)

  rm_rf(_dist_dir)
  mkdir_p(_dist_dir)
  cp_r(paths, _dist_dir, :preserve => true)

  needless_paths ||= @needless_paths
  Dir.glob("#{_dist_dir}/**/{#{needless_paths.join(',')}}") do |needless_path|
    rm_rf(needless_path)
  end
  Dir.glob("#{_dist_dir}/**/*/") do |directory|
    if (Dir.entries(directory) - [".", ".."]).empty?
      rm_r(directory)
    end
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

def change_environment_variable(variables)
  original_variables = {}
  variables.each do |key, value|
    next if ENV[key]
    original_variables[key] = ENV[key]
    ENV[key] = value
  end

  begin
    yield
  ensure
    original_variables.each do |key, value|
      ENV[key] = value
    end
  end
end

def gem_exist?(name, version)
  gem_uri = "https://rubygems.org/gems/#{name}/versions/#{version}"
  begin
    open(gem_uri)
    true
  rescue OpenURI::HTTPError
    false
  end
end

def gem_push(path, name, version)
  return if gem_exist?(name, version)
  begin
    ruby("-S", "gem", "push", path)
  rescue
    puts("failed to push gem: #{path}")
    puts("#{$!.class}: #{$!}")
  end
end

gtk2_base_name = "ruby-gtk2"
gtk3_base_name = "ruby-gtk3"
gnome2_base_name = "ruby-gnome2-all"

gtk2_packages = [
  "glib2",
  "gobject-introspection",
  "gio2",
  "atk",
  "cairo-gobject",
  "pango",
  "gdk_pixbuf2",
  "gtk2",
]
gtk3_packages = gtk2_packages - ["gtk2"] + [
  "gdk3",
  "gtk3",
]
gnome2_packages = gtk2_packages + gtk3_packages + [
  "gstreamer",
  "gtksourceview2",
  "poppler",
  "rsvg2",
  "vte",
  "vte3",
  "gtksourceview3",
  "gtksourceview4",
  "clutter",
  "clutter-gdk",
  "clutter-gtk",
  "clutter-gstreamer",
  "webkit-gtk",
  "webkit-gtk2",
  "webkit2-gtk",
  "gsf",
  "goffice",
  "gnumeric",
  "gegl",
]
gnome2_packages = gnome2_packages.uniq

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

namespace :dist do
  base_files = ["AUTHORS", "COPYING.LIB", "NEWS",
                "README.md", "Rakefile",
                "exec_make.rb", "extconf.rb", "run-test.rb"]
  desc "make Ruby/GTK2 package"
  task :gtk2 do
    package(gtk2_base_name, base_files + gtk2_packages)
  end

  desc "make Ruby/GTK3 package"
  task :gtk3 do
    package(gtk3_base_name, base_files + gtk3_packages)
  end

  desc "make Ruby/GNOME2 package"
  task :gnome2 do
    package(gnome2_base_name, base_files + gnome2_packages)
  end

  ruby_versions = ["2.3.8", "2.4.5", "2.5.3", "2.6.1"]
  namespace :test do
    ruby_base_url = "https://cache.ruby-lang.org/pub/ruby"
    ruby_versions.each do |ruby_version|
      base_dir = "tmp/dist-test-#{version}/#{ruby_version}"
      directory base_dir

      prefix = Pathname.new(base_dir) + "local"
      ruby_tar_bz2_base = "ruby-#{ruby_version}.tar.bz2"
      file "#{base_dir}/#{ruby_tar_bz2_base}" => base_dir do
        generation = ruby_version.scan(/\A\d\.\d/)[0]
        ruby_tar_bz2_url = "#{ruby_base_url}/#{generation}/#{ruby_tar_bz2_base}"
        cd(base_dir) do
          download(ruby_tar_bz2_url)
        end
      end

      ruby_path = "#{prefix}/bin/ruby"
      expanded_ruby_path = File.expand_path(ruby_path)
      file ruby_path => "#{base_dir}/#{ruby_tar_bz2_base}" do
        expanded_prefix = prefix.expand_path
        cd(base_dir) do
          sh("tar", "xvf", ruby_tar_bz2_base)
          cd(File.basename(ruby_tar_bz2_base, ".tar.bz2")) do
            sh("./configure",
               "--prefix=#{expanded_prefix}",
               "--with-out-ext=readline,tk,openssl",
               "--disable-install-doc")
            sh("make", "-j8")
            sh("make", "install")
          end
        end
      end

      rubygems_tgz_base = "rubygems-2.7.6.tgz"
      rubygems_tgz = "#{base_dir}/#{rubygems_tgz_base}"
      file rubygems_tgz => base_dir do
        rubygems_url_base = "http://production.cf.rubygems.org/rubygems"
        cd(base_dir) do
          download("#{rubygems_url_base}/#{rubygems_tgz_base}")
        end
      end

      gem_path = "#{prefix}/bin/gem"
      expanded_gem_path = File.expand_path(gem_path)
      file gem_path => [ruby_path, rubygems_tgz] do
        cd(base_dir) do
          sh("tar", "xvf", rubygems_tgz_base)
          cd(File.basename(rubygems_tgz_base, ".tgz")) do
            sh(expanded_ruby_path, "setup.rb")
          end
        end
      end

      packages = {
        "gtk2" => {
          :archive_name => archive_name(gtk2_base_name),
          :packages => gtk2_packages,
        },
        "gtk3" => {
          :archive_name => archive_name(gtk3_base_name),
          :packages => gtk3_packages,
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
              cd(base_dir) do
                sh("tar", "xvf", tar_gz)
              end
              if /cairo/ !~ `#{expanded_gem_path} list cairo`
                sh(expanded_gem_path,
                   "install",
                   "--clear-sources",
                   "--source", "https://rubygems.org/",
                   "cairo")
              end
            end

            attributes[:packages].each do |sub_package_name|
              task sub_package_name => :prepare do
                cd(base_dir) do
                  package_base_dir = File.basename(tar_gz, ".tar.gz")
                  rm_rf(sub_package_name)
                  sh("cp", "-a",
                     "#{package_base_dir}/#{sub_package_name}", sub_package_name)
                  cd(sub_package_name) do
                    if File.exist?("extconf.rb")
                      sh(expanded_ruby_path, "extconf.rb")
                      sh("make", "-j8")
                      sh("make", "install")
                    end
                  end
                  rm_rf(sub_package_name)
                end
              end
            end
          end
        end

        packages.each do |name, attributes|
          test_tasks = attributes[:packages].collect do |sub_package_name|
            "dist:test:ruby-#{ruby_version}:#{name}:#{sub_package_name}"
          end
          task name => test_tasks
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
task :dist => ["dist:gtk2", "dist:gtk3", "dist:gnome2"]

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
  package_base_names = [gtk2_base_name, gtk3_base_name, gnome2_base_name]
  archive_names = package_base_names.collect do |base_name|
    archive_name(base_name)
  end
  ruby("misc/release.rb", sf_user_name, project_id, project_name,
       package_name, release_name, "README.md:1", "NEWS",
       *archive_names)
end

namespace :gem do
  desc "build all gems"
  task :build do
    gnome2_packages.each do |package|
      cd(package) do
        ruby("-S", "rake", "gem")
      end
    end
  end

  desc "push all gems"
  task :push do
    gnome2_packages.each do |package|
      Dir.glob(File.join(package, "pkg", "*-#{version}.gem")) do |gem_path|
        gem_base_path = File.basename(gem_path, ".gem")
        gem_name = gem_base_path.gsub(/-#{Regexp.escape(version)}\z/, "")
        gem_push(gem_path, gem_name, version)
      end
    end
  end

  desc "install all gems"
  task :install do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "install", "--local", "--user-install",
           *Dir.glob(File.join(package, "pkg", "*-#{version}.gem")))
    end
  end

  desc "uninstall all gems"
  task :uninstall do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "uninstall", "--version", version, package)
    end
  end
end

desc "tag the current release"
task :tag do
  sh("git", "tag", "-a", version, "-m", "release #{version}!!!")
  sh("git", "push", "--tags")
end

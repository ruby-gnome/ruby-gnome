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

def gem_exist?(name, version)
  gem_uri = "https://rubygems.org/gems/#{name}/versions/#{version}"
  begin
    URI.open(gem_uri)
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

packages = [
  "atk",
  "cairo-gobject",
  "clutter",
  "clutter-gdk",
  "clutter-gstreamer",
  "clutter-gtk",
  "gdk3",
  "gdk4",
  "gdk_pixbuf2",
  "gegl",
  "gio2",
  "glib2",
  "gnumeric",
  "gobject-introspection",
  "goffice",
  "gsf",
  "gstreamer",
  "gtk3",
  "gtk4",
  "gtksourceview3",
  "gtksourceview4",
  "libsecret",
  "pango",
  "poppler",
  "rsvg2",
  "vte3",
  "webkit-gtk",
  "webkit2-gtk",
  "wnck3",
]

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

namespace :gem do
  desc "build all gems"
  task :build do
    packages.each do |package|
      cd(package) do
        ruby("-S", "rake", "gem")
      end
    end
  end

  desc "push all gems"
  task :push do
    packages.each do |package|
      Dir.glob(File.join(package, "pkg", "*-#{version}.gem")) do |gem_path|
        gem_base_path = File.basename(gem_path, ".gem")
        gem_name = gem_base_path.gsub(/-#{Regexp.escape(version)}\z/, "")
        gem_push(gem_path, gem_name, version)
      end
    end
  end

  desc "install all gems"
  task :install do
    packages.each do |package|
      ruby("-S", "gem", "install", "--local", "--user-install",
           *Dir.glob(File.join(package, "pkg", "*-#{version}.gem")))
    end
  end

  desc "uninstall all gems"
  task :uninstall do
    packages.each do |package|
      ruby("-S", "gem", "uninstall", "--version", version, package)
    end
  end
end

desc "tag the current release"
task :tag do
  sh("git", "tag", "-a", version, "-m", "release #{version}!!!")
  sh("git", "push", "--tags")
end

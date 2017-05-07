#!/usr/bin/env ruby

# quick & dirty script for running all available tests for each module
# Author: Joachim Glauche

require "rbconfig"
require "pathname"

ruby = File.join(RbConfig::CONFIG['bindir'],
                 RbConfig::CONFIG['RUBY_INSTALL_NAME'] + RbConfig::CONFIG['EXEEXT'])

# for creating a separating line
def separator
  "-" * 80
end

targets = []
includes = []

base_dir = Pathname(File.dirname(__FILE__))
Pathname.glob((base_dir + "*").to_s) do |dir|
  next unless dir.directory?
  dir = dir.expand_path
  ext_dir = dir + "ext" + dir.basename
  includes.concat(["-I", (dir + "lib").to_s, "-I", ext_dir.to_s])
  next unless (dir + "test").directory?
  targets << dir
end

ignored_modules = [
  "atk-no-gi",
  "gdk_pixbuf2-no-gi",
  "gstreamer-no-gi",
  "gdk3-no-gi",
  "gtk3-no-gi",
  "gtksourceview3-no-gi",
  "pango-no-gi",
  "vte3-no-gi",
]

failed_target_names = []
targets.each do |target|
  next if ignored_modules.include?(target.basename.to_s)
  Dir.chdir(target.to_s) do
    puts "#{Time.now.iso8601}: Running test for #{target}"
    puts separator

    dependency_check = "dependency-check/Rakefile"
    if File.exist?(dependency_check)
      unless system(ruby, *includes, "-S",
                    "rake", "--rakefile", dependency_check)
        puts "Failed to resolve dependency: #{target.basename}"
        puts separator
        next
      end
    end

    unless system(ruby, *includes, "test/run-test.rb")
      puts "Failed to run test: #{target.basename}"
      failed_target_names << target.basename.to_s
    end

    puts separator
  end
end

if failed_target_names.empty?
  exit(true)
else
  puts "Failed targets: #{failed_target_names.join(', ')}"
  exit(false)
end

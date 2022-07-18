#!/usr/bin/env ruby

# quick & dirty script for running all available tests for each module
# Author: Joachim Glauche

require "rbconfig"
require "pathname"
require "time"

ruby = RbConfig.ruby

# for creating a separating line
def separator
  "-" * 80
end

targets = []
includes = []

base_dir = Pathname(__dir__)
if ARGV.empty?
  candidates = Pathname.glob((base_dir + "*").to_s)
else
  candidates = ARGV.collect do |package|
    base_dir + package
  end
end
candidates.each do |dir|
  next unless dir.directory?
  source_dir = dir.expand_path
  package_name = dir.basename
  build_dir = Pathname(package_name).expand_path
  lib_dir = source_dir + "lib"
  ext_dir = build_dir + "ext" + package_name
  if ext_dir.exist?
    next unless (ext_dir + "Makefile").exist?
    includes.concat(["-I", lib_dir.to_s, "-I", ext_dir.to_s])
  else
    includes.concat(["-I", lib_dir.to_s])
  end
  next unless (source_dir + "test").directory?
  targets << dir
end

ignored_modules = [
]

failed_target_names = []
targets.each do |target|
  next if target.basename.to_s.end_with?("-no-gi")
  next if ignored_modules.include?(target.basename.to_s)

  puts "::group::Test #{target}"
  puts "#{Time.now.iso8601}: Running test for #{target}"
  puts separator

  dependency_check = target + "dependency-check/Rakefile"
  if dependency_check.exist?
    unless system(ruby, *includes, "-S",
                  "rake", "--rakefile", dependency_check.to_s)
      puts "Failed to resolve dependency: #{target.basename}"
      puts separator
      next
    end
  end

  run_test = target + "test/run-test.rb"
  unless system(ruby, *includes, run_test.to_s)
    puts "Failed to run test: #{target.basename}"
    failed_target_names << target.basename.to_s
  end

  puts separator
  puts "::endgroup::"
end

if failed_target_names.empty?
  exit(true)
else
  puts "Failed targets: #{failed_target_names.join(', ')}"
  exit(false)
end

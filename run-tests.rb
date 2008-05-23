#!/usr/bin/ruby

# quick & dirty script for running all available tests for each module
# Author: Joachim Glauche

require "rbconfig"
require "pathname"

ruby = File.join(Config::CONFIG['bindir'],
                 Config::CONFIG['RUBY_INSTALL_NAME'] +Config::CONFIG['EXEEXT'])

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
  src_dir = dir + "src"
  if src_dir.directory?
    includes.concat(["-I", (src_dir + "lib").to_s, "-I", src_dir.to_s])
  else
    includes.concat(["-I", (dir + "lib").to_s, "-I", dir.to_s])
  end
  next unless (dir + "test").directory?
  targets << dir
end

targets.each do |target|
  Dir.chdir(target.to_s) do
    puts "#{Time.now} running test for #{target}"
    puts separator

    args = includes + ["test/run-test.rb"]
    command = [ruby, *args]
    system(command.collect {|arg| "'#{arg.gsub(/'/, '\\\'')}'"}.join(' '))

    puts separator
  end
end


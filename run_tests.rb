#!/usr/bin/ruby

# quick & dirty script for running all available tests for each module
# Author: Joachim Glauche

require "rbconfig"
require "open3"

ruby = File.join(Config::CONFIG['bindir'],
                 Config::CONFIG['RUBY_INSTALL_NAME'] +Config::CONFIG['EXEEXT'])

# for creating a separating line
def separator
  "-" * 80
end

puts "starting tests..."
File.open("tests.log", "w") do |log|
  Dir.glob("*").each do |dir|
    next unless File.directory?(dir)
    Dir.chdir(dir) do
      if File.exists?("test")
        log.puts "#{Time.now} running test for #{dir}"
        log.puts separator

        stdin, stdout, stderr = Open3.popen3(ruby, "test/run-test.rb")

        log.puts stdout.readlines
        log.puts stderr.readlines
        log.puts
        log.puts
        log.puts separator
      end
    end
  end
end
puts "done. see tests.log for details"


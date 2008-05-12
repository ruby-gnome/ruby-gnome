#!/usr/bin/ruby

# quick & dirty script for running all available tests for each module
# Author: Joachim Glauche

require "open3"

# for creating a seperating line
def seperator()
  str = ""; 
  80.times do
    str << "-"
  end
  str+"\n"
end

puts "starting tests..."
log = File.open("tests.log","w")

dirs = Dir.glob("*")
dirs.each do |dir|
  if File.exists?(dir+"/test")
    log.puts "#{Time.now} running test for #{dir}\n"+seperator()

    stdin, stdout, stderr = Open3.popen3("ruby "+dir+"/test/run-test.rb")
   
    log.puts stdout.readlines
    log.puts stderr.readlines
    log.puts "\n\n"+seperator()        
  end	
 
end
log.close
puts "done. see tests.log for details"


=begin
  exec_make.rb is called by top-level Makefile.

  $Id: exec_make.rb,v 1.4 2004/08/15 15:06:39 mutoh Exp $

  Copyright (C) 2003,2004 Ruby-GNOME2 Project Team
=end

puts "ARGV = " + ARGV.inspect

argv = ARGV
strict = false
if ARGV[0] == "--strict" || ARGV[0] == "-s"
  strict = true
  argv = ARGV[1..-1]
end

SUBDIRS = argv[0].split(" ")
EXECUTE = argv[1..-1].join(' ')

success = []
failure = []
SUBDIRS.each do |subdir| 
	Dir.chdir(subdir)
	begin
		ret = system(EXECUTE)
	ensure
		Dir.chdir("..")
	end

	if ret
		success << subdir 
	else
		failure << subdir
	end
end

success << "NONE" if success.size == 0
failure << "NONE" if failure.size == 0

puts "\n"
puts "-----"
puts "SUCCEEDED: #{success.join(' ')}" if success.size > 0
puts "FAILED: #{failure.join(' ')}" if failure.size > 0
puts "-----"
puts "Done."

if strict && (failure.select { |x| x != "NONE" }.length > 0)
	exit(1)
end

=begin
  exec_make.rb is called by top-level Makefile.

  $Id: exec_make.rb,v 1.2 2003/09/06 09:14:40 sakai Exp $

  Copyright (C) 2003 Ruby-GNOME2 Project Team
=end

SUBDIRS = ARGV[0].split(" ")
EXECUTE = ARGV[1..-1].join(' ')

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


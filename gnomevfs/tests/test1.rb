#!/usr/bin/ruby -w

require 'gnomevfs'

GnomeVFS.init

dir = GnomeVFS::Directory.new ENV["HOME"]
dir.each do |f|
	printf "%s: %s (%d:%d) [%s]\n", f.name, f.ftype, f.uid, f.gid, f.atime
end

uri = GnomeVFS::URI.new "file://"
0.upto 100 do
	uri = uri.append_path ENV["HOME"]
end
puts uri

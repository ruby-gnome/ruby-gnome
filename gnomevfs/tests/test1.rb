#!/usr/bin/ruby -w

require 'gnomevfs'

GnomeVFS.init

dir = GnomeVFS::Directory.new "/home/pcp"
dir.each do |f|
	printf "%s: %s (%d:%d) [%s]\n", f.name, f.ftype, f.uid, f.gid, f.atime
end

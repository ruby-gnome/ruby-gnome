#!/usr/bin/ruby -w

require 'gnomevfs'

GnomeVFS.init

GnomeVFS::Directory.foreach "/home/pcp" do |f|
	printf "%s: %s (%d:%d) [%s]\n", f.name, f.ftype, f.uid, f.gid, f.atime
end

#!/usr/bin/ruby -w

require 'gnomevfs'

GnomeVFS.init

HOME = ENV["HOME"]

GnomeVFS::Directory.foreach HOME do |f|
	printf "%s: %s (%d:%d) [%s]\n", f.name, f.ftype, f.uid, f.gid, f.atime
end

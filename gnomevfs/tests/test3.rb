#!/usr/bin/ruby -w

require 'gnomevfs'

GnomeVFS.init

GnomeVFS::File.open "/tmp/mutt-puritan-4337-13" do |f|
	puts f.read
end

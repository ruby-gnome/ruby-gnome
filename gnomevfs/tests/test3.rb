#!/usr/bin/ruby -w

require 'gnomevfs'

GnomeVFS.init

p file =  File.expand_path(__FILE__)

GnomeVFS::File.open file do |f|
	puts f.read
end

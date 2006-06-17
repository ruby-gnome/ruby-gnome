#!/usr/bin/env ruby

# simple-view.rb

# A very simple program that monitors a single key for changes.
# The key value may be changed with simple-controller.rb
#
# Copyright (C) 2004 Giovanni Corriga
#
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: simple-view.rb,v 1.3 2006/06/17 14:39:54 mutoh Exp $

require 'gtk2'
require 'gconf2'

client = GConf::Client.default
str = client['/extra/test/directory/key']

label = Gtk::Label.new(str ? str : '<unset>')

window = Gtk::Window.new
window.add label

client.add_dir('/extra/test/directory',
		GConf::Client::PRELOAD_RECURSIVE)
client.notify_add('/extra/test/directory/key') { | cli, entry |
	label.text = (entry.value ? entry.value : '<unset>')
}

window.show_all

Gtk.main

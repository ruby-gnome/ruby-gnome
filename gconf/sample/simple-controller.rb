#!/usr/bin/env ruby
#
# simple-controller.rb
#
# A very simple program that sets a single key value when you type
# it in an entry and press return.
# The key value may be inspected with simple-view.rb
#
# Copyright (C) 2004 Giovanni Corriga
#
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: simple-controller.rb,v 1.1 2004/07/03 07:38:02 mutoh Exp $

require 'gtk2'
require 'gconf2'

Gtk.init

window = Gtk::Window.new
entry = Gtk::Entry.new

window.add entry

client = GConf::Client.default
client.add_dir "/extra/test/directory"

entry.signal_connect('activate') { | ent |
	client['/extra/test/directory/key'] = ent.text
}
entry.sensitive = client.key_is_writable?('/extra/test/directory/key')

window.show_all

Gtk.main

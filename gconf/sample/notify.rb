#!/usr/bin/env ruby

require 'gtk2'
require 'gconf2'

GCONF_TEST_KEY = "/extra/test/directory/key"

client = GConf::Client.default
value = client[GCONF_TEST_KEY]
p value

client.add_dir(GCONF_TEST_KEY)
client.notify_add(GCONF_TEST_KEY) {|client, entry|
  p [client, entry, entry.value]
}

client[GCONF_TEST_KEY] = true
client.unset(GCONF_TEST_KEY)
Gtk.main
Gtk.main_quit


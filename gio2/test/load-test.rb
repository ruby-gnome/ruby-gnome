# -*- coding: utf-8 -*-

base = File.dirname(File.expand_path(__FILE__))
root = File.dirname(File.dirname(base))
['glib2', 'gio2'].each do |lib|
  ['ext/%s' % lib, 'lib'].each do |sub|
    $LOAD_PATH.unshift File.join(root, lib, sub)
  end
end

require 'gio2'
require File.join(root, 'glib2/test/glib-test-init')

exit Test::Unit::AutoRunner.run(true, base)

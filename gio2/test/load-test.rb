# -*- coding: utf-8 -*-

root = File.dirname(File.dirname(File.dirname(File.expand_path(__FILE__))))
['glib2', 'gio2'].each do |lib|
  ['ext/%s' % lib, 'lib'].each do |sub|
    $LOAD_PATH.unshift File.join(root, lib, sub)
  end
end

require 'gio2'

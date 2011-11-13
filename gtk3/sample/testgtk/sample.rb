=begin header

  sample.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: sample.rb,v 1.9 2005/07/21 17:47:19 mutoh Exp $

=end

require 'gtk2'

module Sample
  def destroy
    super
    @destroyed = true
  end
  def destroyed?
    @destroyed
  end
end

module SampleClass
  def invoke
    @singleton = nil unless defined? @singleton
    @singleton = new if @singleton.nil? or @singleton.destroyed?
    unless @singleton.visible?
      @singleton.show_all
    else
      @singleton.destroy
    end
  end
end

class SampleWindow < Gtk::Window
  include Sample
  extend SampleClass

  def initialize(title)
    super(title)
    @destroyed = false
    signal_connect("destroy") do destroy end
  end
end

class SampleDialog < Gtk::Dialog
  include Sample
  extend SampleClass

  def initialize(title)
    super(title)
    @destroyed = false
    signal_connect("destroy") do destroy end
  end
end

OptionMenuItem = Struct.new("OptionMenuItem", :name, :block)

def build_option_menu(items, history)
  omenu = Gtk::OptionMenu.new

  menu = Gtk::Menu.new
  group = nil

  items.size.times do |i|
    menu_item = Gtk::RadioMenuItem.new(group, items[i].name)
    menu_item.signal_connect("activate") do |widget|
       items[i].block.call(widget)
    end
    group = menu_item.group
    menu.append(menu_item)
    menu_item.active = true if i == history
    menu_item.show
  end

  omenu.menu = menu
  omenu.history = history

  omenu
end

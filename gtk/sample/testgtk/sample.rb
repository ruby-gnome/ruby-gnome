=begin header

  sample.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: sample.rb,v 1.7 2003/02/01 16:46:23 mutoh Exp $

=end

require 'gtk2'

class Object
  alias :type :class  if defined?(:class)
end

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
  def initialize(title)
    super()
    set_title(title)
    set_border_width(0)
    @destroyed = false
    signal_connect("destroy") do destroy end
  end
end
class << SampleWindow
  include SampleClass
end
class SampleDialog < Gtk::Dialog
  include Sample
  def initialize(title)
    super()
    set_title(title)
    set_border_width(0)
    @destroyed = false
    signal_connect("destroy") do destroy end
  end
end
class << SampleDialog
  include SampleClass
end

def new_pixmap(filename, window, background)
  pixmap, mask = Gdk::Pixmap::create_from_xpm(window, background, filename)
  wpixmap = Gtk::Image.new(pixmap, mask)
end

OptionMenuItem = Struct.new("OptionMenuItem", :name, :block)

def build_option_menu(items, history)
  omenu = Gtk::OptionMenu.new()

  menu = Gtk::Menu.new()
  group = nil

  items.size.times do |i|
    menu_item = Gtk::RadioMenuItem.new(group, items[i].name)
    menu_item.signal_connect("activate") do |widget|
       items[i].block.call(widget)
    end
    group = menu_item.group
    menu.append(menu_item)
    menu_item.set_active(true) if i == history
    menu_item.show
  end

  omenu.set_menu(menu)
  omenu.set_history(history)

  omenu
end

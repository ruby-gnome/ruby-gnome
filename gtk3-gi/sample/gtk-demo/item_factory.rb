# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: item_factory.rb,v 1.4 2005/02/12 23:02:43 kzys Exp $
=begin
= Item Factory

The Gtk::ItemFactory object allows the easy creation of menus
from an array of descriptions of menu items.
=end
require 'common'

module Demo
  class ItemFactory < BasicWindow
    def initialize
      super('Item Factory')

      accel_group = Gtk::AccelGroup.new
      item_factory = Gtk::ItemFactory.new(Gtk::ItemFactory::TYPE_MENU_BAR,
                                          '<main>', accel_group)

      add_accel_group(accel_group)
      set_border_width(0)
      ifactory_cb = proc do |data, widget|
        puts "ItemFactory: activated \"#{Gtk::ItemFactory.path_from_widget(widget)}\""
      end
      menu_items = [
        ['/_File'],
        ['/File/tearoff1',
          '<Tearoff>', nil, nil, ifactory_cb],
        ['/File/_New',
          '<Item>', '<control>N', nil, ifactory_cb],
        ['/File/_Open',
          '<Item>', '<control>O', nil, ifactory_cb],
        ['/File/_Save',
          '<Item>', '<control>S', nil, ifactory_cb],
        ['/File/Save _As...',
          '<Item>', nil, nil, ifactory_cb],
        ['/File/sep1', '<Separator>'],
        ['/File/_Quit',
          '<Item>', '<control>Q', nil, ifactory_cb],

        ['/_Preferences'],
        ['/_Preferences/_Color'],
        [ '/_Preferences/Color/_Red',
          '<RadioItem>', nil, nil, ifactory_cb],
        ['/_Preferences/Color/_Green',
          '/Preferences/Color/Red', nil, nil, ifactory_cb],
        ['/_Preferences/Color/_Blue',
          '/Preferences/Color/Red', nil, nil, ifactory_cb],
        ['/_Preferences/_Shape'],
        ['/_Preferences/Shape/_Square',
          '<RadioItem>', nil, nil, ifactory_cb],
        [ '/_Preferences/Shape/_Rectangle',
          '/Preferences/Shape/Square', nil, nil, ifactory_cb],
        [ '/_Preferences/Shape/_Oval',
          '/Preferences/Shape/Rectangle', nil, nil, ifactory_cb],

        [ '/_Help', '<LastBranch>'],
        [ '/Help/_About', '<Item>', nil, nil, ifactory_cb],
      ]
      item_factory.create_items(menu_items)

      item_factory.get_item('/Preferences/Shape/Oval').set_active(true)

      box1 = Gtk::VBox.new(false, 0)
      add(box1)

      box1.pack_start(item_factory.get_widget('<main>'), :expand => false, :fill => false, :padding => 0)

      label = Gtk::Label.new("Type\n<alt>\nto start")
      label.set_size_request(200, 200)
      label.set_alignment(0.5, 0.5)
      box1.pack_start(label, :expand => true, :fill => true, :padding => 0)

      separator = Gtk::HSeparator.new
      box1.pack_start(separator)


      box2 = Gtk::VBox.new(false, 10)
      box2.set_border_width(10)
      box1.pack_start(box2, :expand => false, :fill => true, :padding => 0)

      button = Gtk::Button.new('close')
      # TODO: Need signal_connect_swapped?
      button.signal_connect('clicked') do
        quit
      end
      box2.pack_start(button, :expand => true, :fill => true, :padding => 0)
      button.set_flags(Gtk::Widget::CAN_DEFAULT)
      button.grab_default
    end
  end
end

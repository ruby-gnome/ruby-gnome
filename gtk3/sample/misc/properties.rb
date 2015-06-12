#!/usr/bin/env ruby
=begin
  properties.rb -

  GLib::Object#properties and Gtk::Container#child_properties sample script.

  Copyright (C) 2004-2006 Masao Mutoh
  Copyright (c) 2004-2015 Ruby-GNOME2 Project Team

  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

class MyButton < Gtk::Button
  type_register

  def initialize(label = nil)
    # XXX:
    # When type_register() is used.
    # super is equivalent to GLib::Object#initialize.
    super("label" => label)
    @fuga = 0
    @hoge = 0
  end

  # define new property "fuga"
  install_property(GLib::Param::Int.new("fuga", # name
                                        "Fuga", # nick
                                        "fuga fuga", # blurb
                                        0,     # min
                                        10_000,# max
                                        0,     # default
                                        GLib::Param::READABLE |
                                        GLib::Param::WRITABLE))

  # define new property "hoge"
  install_child_property(1,GLib::Param::Int.new("hoge", # name
                                                "Hoge", # nick
                                                "hoge hoge", # blurb
                                                0,     # min
                                                10_000,# max
                                                0,     # default
                                                GLib::Param::READABLE |
                                                GLib::Param::WRITABLE)
                        )

  # implementation of the property "fuga"
  def fuga
    puts "MyButton#fuga is called"
    @fuga
  end

  def fuga=(arg)
    puts "MyButton#fuga= is called"
    @fuga = arg
    notify("fuga")
  end

  # implementation of the property "hoge"
  def get_hoge(child)
    puts "MyButton#get_hoge is called"
    @hoge
  end

  def set_hoge(child, arg)
    puts "MyButton#set_hoge is called"
    @hoge = arg
  end
end

b = MyButton.new("Hello")
p b
p b.label
p b.gtype

p MyButton.properties
p b.get_property("fuga")
b.set_property("fuga", 1)
p b.get_property("fuga")

p MyButton.child_properties
p child = b.child
p b.child_get_property(child, "hoge")
b.child_set_property(child, "hoge", 2)
p b.child_get_property(child, "hoge")

p MyButton.ancestors

=begin
  type-register.rb - Sample for GLib::Object

  You also need Ruby/GTK.

  Copyright (C) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2006/06/17 14:31:22 $
  $Id: type-register.rb,v 1.9 2006/06/17 14:31:22 mutoh Exp $
=end

require 'gtk2'

class MyButton < Gtk::Button
  type_register

  def initialize(label = nil)
    # XXX: 
    # When type_register() is used.
    # super is equivalent to GLib::Object#initialize.
    super("label" => label)
    @fuga = 0
  end

  # override existing default handler of "clicked" signal.
  def signal_do_clicked(*args)
    puts "MyButton#signal_do_clicked enter"
    #p caller
    super
    puts "MyButton#signal_do_clicked leave"
  end

  # define new signal "hoge"
  define_signal("hoge",                  # name
                GLib::Signal::RUN_FIRST, # flags
                nil,                     # accumulator (XXX: not supported yet)
                nil,                     # return type (void == nil)
                Integer, Integer         # parameter types
                )
  # define default handler of "hoge" signal
  def signal_do_hoge(a, b)
    puts "MyButton#signal_do_hoge enter"
    #p caller
    puts "MyButton#signal_do_hoge leave"
  end

  # define new property "fuga"
  install_property(GLib::Param::Int.new("fuga", # name
                                        "Fuga", # nick
                                        "fuga hoge", # blurb
                                        0,     # min
                                        10000, # max
                                        0,     # default
                                        GLib::Param::READABLE |
                                        GLib::Param::WRITABLE))
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
end

class MyButton2 < MyButton
  type_register("MyButton2")

  # override default handler of "clicked" signal
  def signal_do_clicked(*args)
    puts "MyButton2#signal_do_clicked enter"
    super(*args)
    puts "MyButton2#signal_do_clicked leave"
  end

  # override default handler of "hoge" signal
  def signal_do_hoge(a, b)
    puts "MyButton2#signal_do_hoge enter"
    puts "a, b = #{a}, #{b}"
    super
    puts "MyButton2#signal_do_hoge leave"
  end
end

b = MyButton2.new("Hello")
p b
p b.label
p b.gtype
b.clicked
b.signal_emit("hoge", 1, 2)

b.signal_connect("notify"){|obj, pspec|
  puts "#{b} notify #{pspec}"
}

p b.get_property("fuga")
b.set_property("fuga", 1)
p b.get_property("fuga")

p MyButton2.ancestors

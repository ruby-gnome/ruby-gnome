#!/usr/bin/env ruby
=begin
  list_store_usage.rb - Ruby/GTK script 
  http://ruby-gnome2.osdn.jp/hiki.cgi?Gtk%3A%3AListStore
  https://developer.gnome.org/gtk3/stable/GtkListStore.html

  Copyright (c) 2015  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end
require "gtk3"

def print_list(list)
  puts ""
  print "\t"
  list.each do |_model, _path, iter|
    print iter[1] + "  "
  end
  print "\n"
  puts ""
end

def load_list
  model = Gtk::ListStore.new(TrueClass, String)

  4.times do |i|
    model.append.set_values([false, "index#{i}"])
  end
  model
end
NOTCH = "-[|]-"

# Insert data
puts "Initial state of the model build with append method"
model = load_list
print_list(model)

puts "Prepend #{NOTCH}"
model = load_list
model.prepend.set_values([true, NOTCH])
print_list(model)

puts "Insert #{NOTCH} at position 3"
model = load_list
model.insert(3).set_values([true, NOTCH])
print_list(model)

puts "Insert #{NOTCH} before item at index 2"
model = load_list
model.insert_before(model.get_iter("2")).set_values([true, NOTCH])
print_list(model)

puts "Insert #{NOTCH} after item at index 2"
model = load_list
model.insert_after(model.get_iter("2")).set_values([true, NOTCH])
print_list(model)

puts "Move iter before position 2 and set #{NOTCH}"
model = load_list
iter = model.iter_first
iter2 = model.get_iter("2")
model.move_before(iter, iter2)
iter.set_values([true, NOTCH])
print_list(model)

puts "Move iter after position 2 and set #{NOTCH}"
model = load_list
iter = model.iter_first
iter2 = model.get_iter("2")
model.move_after(iter, iter2)
iter.set_values([true, NOTCH])
print_list(model)

# Move data
puts "Reorder the list"
model = load_list
model.reorder([3, 2, 1, 0])
print_list(model)

puts "Swap item at index 1 with item at index 3"
model = load_list
model.swap(model.get_iter("1"), model.get_iter("3"))
print_list(model)

# Remove data

puts "Remove data at index 2"
model = load_list
iter = model.get_iter("2")
model.remove(iter)
print_list(model)

puts "Clear and add #{NOTCH}"
model = load_list
model.clear
model.append.set_values([true, NOTCH])
print_list(model)

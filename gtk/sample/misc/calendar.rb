=begin
  calendar.rb - Gtk::Calendar sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: calendar.rb,v 1.2 2002/11/05 10:39:10 mutoh Exp $
=end

require 'gtk2'

Gtk.init

cal = Gtk::Calendar.new
w = Gtk::Window.new
w.add(cal).show_all.signal_connect('delete_event') do
  Gtk::main_quit
end

date = Time.new

cal.select_month(date.month, date.year)
cal.select_day(date.day)
cal.mark_day(date.day)
#cal.clear_marks
cal.display_options(Gtk::Calendar::SHOW_HEADING |
		    Gtk::Calendar::SHOW_DAY_NAMES |
		    Gtk::Calendar::NO_MONTH_CHANGE |
		    Gtk::Calendar::SHOW_WEEK_NUMBERS |
		    Gtk::Calendar::WEEK_START_MONDAY)
year, month, day = cal.date
print "this is #{month} #{day}, #{year}\n"

cal.signal_connect('day_selected') do
  year, month, day = cal.date
  print "selected day: ", day, "\n"
end
cal.signal_connect('month_changed') do
  year, month, day = cal.date
  print "changed month: ", month, "\n"
end
cal.signal_connect('day_selected_double_click') do
  year, month, day = cal.date
  print "dclicked day: ", day, "\n"
end
cal.signal_connect('prev_month') do
  year, month, day = cal.date
  print "prev month: ", month, "\n"
end
cal.signal_connect('next_month') do
  year, month, day = cal.date
  print "next_month: ", month, "\n"
end
cal.signal_connect('prev_year') do
  year, month, day = cal.date
  print "prev_year: ", year, "\n"
end
cal.signal_connect('next_year') do
  year, month, day = cal.date
  print "next year: ", year, "\n"
end

Gtk::main

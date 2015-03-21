=begin
  calendar.rb - Gtk::Calendar sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

cal = Gtk::Calendar.new

window = Gtk::Window.new("Calendar sample")
window.add(cal)
window.show_all
window.signal_connect("delete_event") do
  Gtk.main_quit
end

date = Time.new

cal.select_month(date.month, date.year)
cal.select_day(date.day)
cal.mark_day(date.day)
#cal.clear_marks

#
# Gtk::CalendarDisplayOptions::WEEK_START_MONDAY does not exist anymore
#

cal.set_display_options(Gtk::CalendarDisplayOptions::SHOW_HEADING |
                        Gtk::CalendarDisplayOptions::SHOW_DAY_NAMES |
                        Gtk::CalendarDisplayOptions::NO_MONTH_CHANGE |
                        Gtk::CalendarDisplayOptions::SHOW_WEEK_NUMBERS)
year, month, day = cal.date
puts "this is #{month} #{day}, #{year}"

cal.signal_connect("day_selected") do
  year, month, day = cal.date
  puts "selected day: #{day}"
end
cal.signal_connect("month_changed") do
  year, month, day = cal.date
  puts "changed month: #{month}"
end
cal.signal_connect("day_selected_double_click") do
  year, month, day = cal.date
  puts "dclicked day: #{day}"
end
cal.signal_connect("prev_month") do
  year, month, day = cal.date
  puts "prev month: #{month}"
end
cal.signal_connect("next_month") do
  year, month, day = cal.date
  puts "next_month: #{month}"
end
cal.signal_connect("prev_year") do
  year, month, day = cal.date
  puts "prev_year: #{year}"
end
cal.signal_connect("next_year") do
  year, month, day = cal.date
  puts "next year: #{year}"
end

Gtk.main

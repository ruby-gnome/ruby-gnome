=begin
  calendar.rb - Gtk::Calendar sample script.

  Copyright (c) 2002-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
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
# cal.clear_marks

cal.set_display_options(Gtk::CalendarDisplayOptions::SHOW_HEADING |
                        Gtk::CalendarDisplayOptions::SHOW_DAY_NAMES |
                        Gtk::CalendarDisplayOptions::SHOW_WEEK_NUMBERS)

puts "this is #{cal.month} #{cal.day}, #{cal.year}"

cal.signal_connect("day_selected") do
  puts "selected day: #{cal.day}"
end
cal.signal_connect("month_changed") do
  puts "changed month: #{cal.month}"
end
cal.signal_connect("day_selected_double_click") do
  puts "dclicked day: #{cal.day}"
end
cal.signal_connect("prev_month") do
  puts "prev month: #{cal.month}"
end
cal.signal_connect("next_month") do
  puts "next_month: #{cal.month}"
end
cal.signal_connect("prev_year") do
  puts "prev_year: #{cal.year}"
end
cal.signal_connect("next_year") do
  puts "next year: #{cal.year}"
end

Gtk.main

require 'gtk'

cal = Gtk::Calendar.new
w = Gtk::Window.new(Gtk::WINDOW_TOPLEVEL)
w.add(cal).show_all.signal_connect('delete_event') do
  Gtk::main_quit
end
cal.select_month(7, 1974)
cal.select_day(15)
cal.mark_day(20)
cal.clear_marks
#cal.display_options(Gtk::Calendar::SHOW_HEADING |
#		    Gtk::Calendar::SHOW_DAY_NAMES |
#		    Gtk::Calendar::NO_MONTH_CHANGE |
#		    Gtk::Calendar::SHOW_WEEK_NUMBERS |
#		    Gtk::Calendar::WEEK_START_MONDAY)
year, month, day = cal.get_date
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

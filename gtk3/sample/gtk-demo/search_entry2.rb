# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Entry/Delayed Search Entry

 GtkSearchEntry sets up GtkEntries ready for search. Search entries
 have their "changed" signal delayed and should be used
 when the searched operation is slow such as loads of entries
 to search, or online searches.
=end
module SearchEntry2Demo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.title = "Delayed Search Entry"
    window.transient_for = main_window
    window.resizable = true
    window.set_size_request(200, -1)

    vbox = Gtk::Box.new(:vertical, 0)
    window.add(vbox)
    vbox.border_width = 0

    entry = Gtk::SearchEntry.new
    container = Gtk::Box.new(:horizontal, 10)
    container.halign = :center
    container.pack_start(entry, :expand => false, :fill => false, :padding => 0)

    searchbar = Gtk::SearchBar.new
    searchbar.connect_entry(entry)
    searchbar.show_close_button = false
    searchbar.add(container)
    vbox.pack_start(searchbar, :expand => false, :fill => false, :padding => 0)

    # Hook the search bar to key presses
    window.signal_connect("key-press-event") do |_widget, event|
      searchbar.handle_event(event)
    end

    # Help
    label = Gtk::Label.new("Start Typing to search")
    vbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    # Toggle button
    button = Gtk::ToggleButton.new(:label => "Search")
    button.bind_property("active", searchbar, "search-mode-enabled", :bidirectional)
    vbox.pack_start(button, :expand => true, :fill => true, :padding => 0)

    # Result
    hbox = Gtk::Box.new(:horizontal, 10)
    vbox.pack_start(hbox, :expand => true, :fill => true, :padding => 0)
    hbox.border_width = 0

    label = Gtk::Label.new("Result:")
    label.xalign = 0.0
    label.margin_start = 6
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    label = Gtk::Label.new("")
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    entry.signal_connect("search-changed") do |widget|
      puts "search changed: #{widget.text || ''}"
      label.text = widget.text || ""
    end

    entry.signal_connect("changed") do |_widget|
      puts "changed: #{label.text || ''}"
    end

    hbox = Gtk::Box.new(:horizontal, 10)
    vbox.pack_start(hbox, :expand => true, :fill => true, :padding => 0)
    hbox.border_width = 0

    label = Gtk::Label.new("Signal:")
    label.xalign = 0.0
    label.margin_start = 6
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    label = Gtk::Label.new("")
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    entry.signal_connect("search-changed") do |widget|
      puts "search changed: #{widget.text || ''}"
      label.text = widget.text || ""
    end

    entry.signal_connect("next-match") do |_widget|
      label.text = "next-match"
    end

    entry.signal_connect("previous-match") do |_widget|
      label.text = "previous-match"
    end

    entry.signal_connect("stop-search") do |_widget|
      label.text = "stop-search"
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end

# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Entry/Delayed Search Entry

 GtkSearchEntry sets up GtkEntries ready for search. Search entries
 have their "changed" signal delayed and should be used
 when the searched operation is slow such as loads of entries
 to search, or online searches.
=end
class SearchEntry2Demo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.title = "Delayed Search Entry"
    @window.transient_for = main_window
    @window.resizable = true
    @window.set_size_request(200, -1)

    initialize_vbox

    @entry = Gtk::SearchEntry.new
    initialize_search_bar
    @vbox.pack_start(@searchbar,
                     :expand => false, :fill => false, :padding => 0)

    # Hook the search bar to key presses
    @window.signal_connect("key-press-event") do |_widget, event|
      @searchbar.handle_event(event)
    end

    initialize_help_label
    initialize_toggle_button
    initialize_result_hbox
    initialize_signal_hbox
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def initialize_search_bar
    container = Gtk::Box.new(:horizontal, 10)
    container.halign = :center
    container.pack_start(@entry,
                         :expand => false, :fill => false, :padding => 0)

    @searchbar = Gtk::SearchBar.new
    @searchbar.connect_entry(@entry)
    @searchbar.show_close_button = false
    @searchbar.add(container)
  end

  def initialize_vbox
    @vbox = Gtk::Box.new(:vertical, 0)
    @window.add(@vbox)
  end

  def initialize_help_label
    # Help
    label = Gtk::Label.new("Start Typing to search")
    @vbox.pack_start(label, :expand => true, :fill => true, :padding => 0)
  end

  def initialize_toggle_button
    # Toggle button
    button = Gtk::ToggleButton.new(:label => "Search")
    button.bind_property("active", @searchbar, "search-mode-enabled",
                         :bidirectional)
    @vbox.pack_start(button, :expand => true, :fill => true, :padding => 0)
  end

  def initialize_result_hbox
    hbox = Gtk::Box.new(:horizontal, 10)
    @vbox.pack_start(hbox, :expand => true, :fill => true, :padding => 0)

    # Result
    label = Gtk::Label.new("Result:")
    label.xalign = 0.0
    label.margin_start = 6
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)
    label = Gtk::Label.new("")
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    @entry.signal_connect("search-changed") do |widget|
      puts "search changed: #{widget.text || ''}"
      label.text = widget.text || ""
    end
    @entry.signal_connect("changed") { puts "changed: #{label.text || ''}" }
  end

  def initialize_signal_hbox
    hbox = Gtk::Box.new(:horizontal, 10)
    @vbox.pack_start(hbox, :expand => true, :fill => true, :padding => 0)

    label = Gtk::Label.new("Signal:")
    label.xalign = 0.0
    label.margin_start = 6
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    label = Gtk::Label.new("")
    hbox.pack_start(label, :expand => true, :fill => true, :padding => 0)

    @entry.signal_connect("search-changed") { label.text = "search-changed" }
    @entry.signal_connect("next-match") { label.text = "next-match" }
    @entry.signal_connect("previous-match") { label.text = "previous-match" }
    @entry.signal_connect("stop-search") { label.text = "stop-search" }
  end
end

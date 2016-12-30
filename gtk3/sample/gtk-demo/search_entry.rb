# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Entry/Search Entry

 GtkEntry allows to display icons and progress information.
 This demo shows how to use these features in a search entry.
=end
class SearchEntryDemo
  def initialize(main_window)
    @main_window = main_window

    initialize_window
    initialize_box
    initialize_entry
    initialize_menu
    @menu.attach_to_widget(@entry)
    initialize_notebook
    @window.signal_connect("destroy") { finish_search }
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

  def initialize_window
    @window = Gtk::Window.new(:toplevel)
    @window.screen = @main_window.screen
    @window.title = "Search Entry"
  end

  def initialize_box
    @vbox = Gtk::Box.new(:vertical, 5)
    @window.add(@vbox)
    @vbox.margin = 5

    label = Gtk::Label.new("")
    label.markup = "Search entry demo"
    @vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    @hbox = Gtk::Box.new(:horizontal, 10)
    @vbox.pack_start(@hbox, :expand => true, :fill => true, :padding => 0)
  end

  def initialize_entry
    # Create our entry
    @entry = Gtk::SearchEntry.new
    @hbox.pack_start(@entry, :expand => false, :fill => false, :padding => 0)

    # add accessible alternatives for icon functionality
    @entry.set_property("populate-all", true)
    @entry.signal_connect "populate-popup" do |widget, menu|
      item = Gtk::SeparatorMenuItem.new
      item.show
      menu.append(item)

      item = Gtk::MenuItem.new(:label => "C_lear", :use_underline => true)
      item.show

      item.signal_connect("activate") { widget.text = "" }
      menu.append(item)
      item.sensitive = !widget.text.empty?

      item = Gtk::MenuItem.new(:label => "Search by")
      item.show
      initialize_menu
      item.submenu = @menu
      menu.append(item)
    end
  end

  def initialize_notebook
    # Create the find and cancel buttons
    @notebook = Gtk::Notebook.new
    @notebook.show_tabs = false
    @notebook.show_border = false
    @hbox.pack_start(@notebook, :expand => false, :fill => false, :padding => 0)

    @search_progress_id = nil
    @finish_progress_id = nil

    find_button = Gtk::Button.new(:label => "Find")
    find_button.signal_connect("clicked") { start_search }
    @notebook.append_page(find_button)
    find_button.show

    cancel_button = Gtk::Button.new(:label => "Cancel")
    cancel_button.signal_connect "clicked" do
      if @finish_search_id
        GLib::Source.remove(@finish_search_id)
        @finish_search_id = nil
      end
      finish_search
    end
    @notebook.append_page(cancel_button)
    cancel_button.show

    @entry.set_icon_tooltip_text(:primary, <<-TOOLTIP)
Search by name
Click here to change the search type.
TOOLTIP
    @entry.placeholder_text = "name"

    @entry.signal_connect "icon-press" do |_widget, position, event|
      @menu.popup(nil, nil, event.button, event.time) if position == :primary
    end

    @entry.signal_connect "activate" do
      start_search unless @search_progress_id
    end
  end

  def show_find_page
    @notebook.current_page = 0
  end

  def show_cancel_page
    @notebook.current_page = 1
  end

  def start_search
    show_cancel_page
    @search_progress_id = GLib::Timeout.add_seconds(1) do
      @search_progress_id = GLib::Timeout.add(100) do
        @entry.progress_pulse
        GLib::Source::CONTINUE
      end
      GLib::Source::REMOVE
    end

    @finish_search_id = GLib::Timeout.add_seconds(15) do
      finish_search
    end
  end

  def finish_search
    show_find_page
    if @search_progress_id
      GLib::Source.remove(@search_progress_id)
      @search_progress_id = nil
      @entry.progress_fraction = 0.0
    end
    GLib::Source::REMOVE
  end

  def initialize_menu
    @menu = Gtk::Menu.new

    item = Gtk::MenuItem.new(:label => "Search by _name",
                             :use_underline => true)
    item.signal_connect("activate") do
      @entry.set_icon_tooltip_text(:primary, <<-TOOLTIP)
                                   Search by name
                                   Click here to change the search type
TOOLTIP
      @entry.placeholder_text = "name"
    end
    @menu.append(item)

    item = Gtk::MenuItem.new(:label => "Search by _description",
                             :use_underline => true)
    item.signal_connect("activate") do
      @entry.set_icon_tooltip_text(:primary, <<-TOOLTIP)
                                   Search by description
                                   Click here to change the search type
TOOLTIP
      @entry.placeholder_text = "description"
    end
    @menu.append(item)

    @menu.show_all
    item = Gtk::MenuItem.new(:label => "Search by _file",
                             :use_underline => true)
    item.signal_connect("activate") do
      @entry.set_icon_tooltip_text(:primary, <<-TOOLTIP)
                                   Search by file
                                   Click here to change the search type
TOOLTIP

      @entry.placeholder_text = "file"
    end
    @menu.append(item)

    @menu.show_all
  end
end

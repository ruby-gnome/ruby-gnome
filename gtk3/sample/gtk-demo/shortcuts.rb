# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Shortcuts Window

 GtkShortcutsWindow is a window that provides a help overlay
 for shortcuts and gestures in an application.
=end
class ShortcutsDemo
  def initialize(main_window)
    @icons_added = false
    unless @icons_added
      theme = Gtk::IconTheme.default
      theme.add_resource_path("/icons")
      @icons_added = true
    end

    ui = "/shortcuts/shortcuts.ui"
    @builder = Gtk::Builder.new(:resource => ui)
    builder_add_callback_symbols
    @builder.connect_signals {}

    @window = @builder["window1"]
    @window.screen = main_window.screen
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

  def builder_add_callback_symbols
    @builder.connect_signals do |name|
      case name
      when "builder_shortcuts"
        builder_shortcuts
      when "gedit_shortcuts"
        gedit_shortcuts
      when "clocks_shortcuts"
        clocks_shortcuts
      when "clocks_shortcuts_stopwatch"
        clocks_shortcuts_stopwatch
      when "boxes_shortcuts"
        boxes_shortcuts
      when "boxes_shortcuts_wizard"
        boxes_shortcuts_wizard
      when "boxes_shortcuts_display"
        boxes_shortcuts_display
      end
    end
  end

  def show_shortcuts(id, view)
    path = "/shortcuts/#{id}.ui"
    builder = Gtk::Builder.new(:resource => path)
    overlay = builder[id]
    overlay.set_transient_for(@window)
    overlay.view_name = view if view
    overlay.show
  end

  def builder_shortcuts
    proc do
      show_shortcuts("shortcuts-builder", nil)
    end
  end

  def gedit_shortcuts
    proc do
      show_shortcuts("shortcuts-gedit", nil)
    end
  end

  def clocks_shortcuts
    proc do
      show_shortcuts("shortcuts-clocks", nil)
    end
  end

  def clocks_shortcuts_stopwatch
    proc do
      show_shortcuts("shortcuts-clocks", "stopwatch")
    end
  end

  def boxes_shortcuts
    proc do
      show_shortcuts("shortcuts-boxes", nil)
    end
  end

  def boxes_shortcuts_wizard
    proc do
      show_shortcuts("shortcuts-boxes", "wizard")
    end
  end

  def boxes_shortcuts_display
    proc do
      show_shortcuts("shortcuts-boxes", "display")
    end
  end
end

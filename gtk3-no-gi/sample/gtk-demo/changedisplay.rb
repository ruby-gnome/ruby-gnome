# -*- coding: utf-8 -*-
#
# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: changedisplay.rb,v 1.6 2005/07/30 11:22:15 mutoh Exp $
=begin
= Change Display

Demonstrates migrating a window between different displays and
screens. A display is a mouse and keyboard with some number of
associated monitors. A screen is a set of monitors grouped
into a single physical work area. The neat thing about having
multiple displays is that they can be on a completely separate
computers, as long as there is a network connection to the
computer where the application is running.

Only some of the windowing systems where GTK+ runs have the
concept of multiple displays and screens. (The X Window System
is the main example.) Other windowing systems can only
handle one keyboard and mouse, and combine all monitors into
a single screen.

This is a moderately complex example, and demonstrates:

- Tracking the currently open displays and screens

- Changing the screen for a window

- Letting the user choose a window by clicking on it

- Using Gtk::ListStore and Gtk::TreeView

- Using Gtk::Dialog
=end

require 'common'

module Demo
  class ChangeDisplay < Gtk::Dialog
    # These enumerations provide symbolic names for the columns
    # in the two GtkListStore models.
    DISPLAY_COLUMN_NAME, DISPLAY_COLUMN_DISPLAY = 0, 1
    SCREEN_COLUMN_NUMBER, SCREEN_COLUMN_SCREEN = 0, 1

    # Main entry point. If the dialog for this demo doesn't yet exist, creates
    # it. Otherwise, destroys it.
    def initialize
      @size_group = nil

      @display_model = nil
      @screen_model = nil

      @screen_selection = nil

      @current_display = nil
      @current_screen = nil

      super(
        :title => 'Change Screen or display',
        :buttons => [
          [:close, :close],
          ['Change', :ok]
        ]
      )

      set_default_size(300, 400)
      signal_connect('response') do |dialog, response_id|
        if response_id == Gtk::ResponseType::OK
          query_change_display
        else
          destroy
        end
      end

      vbox = Gtk::Box.new(:vertical, 5)
      vbox.set_border_width(8)

      self.child.pack_start(vbox, :expand => true, :fill => true)

      @size_group = Gtk::SizeGroup.new(:horizontal)

      frame = create_display_frame
      vbox.pack_start(frame, :expand => true, :fill => true)

      frame = create_screen_frame
      vbox.pack_start(frame, :expand => true, :fill => true)

      initialize_displays
    end

    # Adds all currently open displays to our list of displays,
    # and set up a signal connection so that we'll be notified
    # when displays are opened in the future as well.
    def initialize_displays
      manager = Gdk::DisplayManager.get

      manager.displays.each do |display|
        add_display(display)
      end

      handler_id = manager.signal_connect('display_opened') do |display|
        add_display(display)
      end
      signal_connect('destroy') do
        manager.signal_handler_disconnect(handler_id)
      end
    end

    # Fills in the screen list based on the current display
    def fill_screens
      @screen_model.clear

      if @current_display
        n_screens = @current_display.n_screens

        n_screens.times do |i|
          iter = @screen_model.append
          iter.set_value(SCREEN_COLUMN_NUMBER, i)
          iter.set_value(SCREEN_COLUMN_SCREEN, @current_display.get_screen(i))

          if i == 0
            @screen_selection.select_iter(iter)
          end
        end
      end
    end

    # Creates the 'Display' frame in the main window.
    def create_display_frame
      frame, tree_view, button_vbox = create_frame('Display')

      button = left_align_button_new('_Open…')
      button.signal_connect('clicked') do
        open_display_cb
      end
      button_vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)

      button = left_align_button_new('_Close')
      button.signal_connect('clicked') do
        if @current_display
          @current_display.close
        end
      end
      button_vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)

      @display_model = Gtk::ListStore.new(String, Gdk::Display)
      tree_view.model = @display_model

      column = Gtk::TreeViewColumn.new('Name',
                                       Gtk::CellRendererText.new,
                                       {'text' => DISPLAY_COLUMN_NAME})
      tree_view.append_column(column)

      selection = tree_view.selection
      selection.signal_connect('changed') do
        display_changed_cb(selection)
      end

      return frame
    end

    # Creates the 'Screen' frame in the main window.
    def create_screen_frame
      frame, tree_view, button_vbox = create_frame('Screen')

      @screen_model = Gtk::ListStore.new(Integer, Gdk::Screen)
      tree_view.model = @screen_model

      column = Gtk::TreeViewColumn.new('Number',
                                       Gtk::CellRendererText.new,
                                       {'text' => SCREEN_COLUMN_NUMBER})
      tree_view.append_column(column)

      @screen_selection = tree_view.selection
      @screen_selection.signal_connect('changed') do |selection|
        @current_screen = if iter = selection.selected
                            iter.get_value(SCREEN_COLUMN_SCREEN)
                          else
                            nil
                          end
      end

      return frame
    end

    # This function is used both for creating the 'Display' and
    # 'Screen' frames, since they have a similar structure. The
    # caller hooks up the right context for the value returned
    # in tree_view, and packs any relevant buttons into button_vbox.
    def create_frame(title)
      frame = Gtk::Frame.new(title)

      hbox = Gtk::Box.new(:horizontal, 8)
      hbox.set_border_width(8)
      frame.add(hbox)

      scrollwin = Gtk::ScrolledWindow.new
      scrollwin.set_policy(:never, :automatic)
      scrollwin.shadow_type = :in
      hbox.pack_start(scrollwin, :expand => true, :fill => true)

      tree_view = Gtk::TreeView.new
      tree_view.headers_visible = false
      scrollwin.add(tree_view)

      selection = tree_view.selection
      selection.mode = :browse

      button_vbox = Gtk::Box.new(:vertical, 5)
      hbox.pack_start(button_vbox, :expand => false, :fill => false)

      @size_group.add_widget(button_vbox)

      return frame, tree_view, button_vbox
    end

    # If we have a stack of buttons, it often looks better if their contents
    # are left-aligned, rather than centered. This function creates a button
    # and left-aligns it contents.
    def left_align_button_new(label)
      button = Gtk::Button.new(:mnemonic => label)
      button.child.set_alignment(0.0, 0.5)

      return button
    end


    # Prompts the user for a toplevel window to move, and then moves
    # that window to the currently selected display
    def query_change_display
      screen = self.screen

      toplevel = query_for_toplevel(screen,
                                    "Please select the toplevel\n"+
                                    "to move to the new screen")

      if toplevel
        toplevel.screen = @current_screen
      else
        screen.display.beep
      end
    end

    # Asks the user to click on a window, then waits for them click
    # the mouse. When the mouse is released, returns the toplevel
    # window under the pointer, or nil, if there is none.
    def query_for_toplevel(screen, prompt)
      toplevel = nil

      display = screen.display

      popup = Gtk::Window.new(:popup)
      popup.screen = screen
      popup.modal = true
      popup.window_position = :center

      frame = Gtk::Frame.new
      frame.set_shadow_type(:out)
      popup.add(frame)

      label = Gtk::Label.new(prompt)
      label.set_padding(10, 10)
      frame.add(label)

      popup.show_all

      # TODO: Gdk::Cursor.new(screen.display, Gdk::Cursor::CROSSHAIR)
      cursor = Gdk::Cursor.new(:crosshair)

      if Gdk::pointer_grab(
        popup.window,
        false,
        :button_release_mask,
        nil,
        cursor,
        Gdk::Event::CURRENT_TIME
      ) == Gdk::GRAB_SUCCESS
      clicked = false

        popup.signal_connect('button-release-event') do
          clicked = true
        end

        # Process events until clicked is set by button_release_event_cb.
        # We pass in may_block = true since we want to wait if there
        # are no events currently.
        until clicked
          Gtk.main_iteration # TODO: GLib::main_context_iteration(nil, true)

          toplevel = find_toplevel_at_pointer(screen.display)
          if toplevel == popup
            toplevel = nil
          end
        end

        popup.destroy
        Gdk.flush # Really release the grab

        return toplevel
      end
    end

    # Finds the toplevel window under the mouse pointer, if any.
    def find_toplevel_at_pointer(display)
      pointer_window, x, y = display.window_at_pointer

      # The user data field of a GdkWindow is used to store a pointer
      # to the widget that created it.
      if pointer_window
        widget = pointer_window.user_data
      end

      return (if widget
                widget.toplevel
              else
                nil
              end)
    end

    # Called when the user clicks on 'Open...' in the display
    # frame. Prompts for a new display, and then opens a connection
    # to that display.
    def open_display_cb
      dialog = Gtk::Dialog.new(
        :title => 'Open Display',
        :parent => self,
        :flags => :modal,
        :buttons => [
          [:cancel, :cancel],
          [:ok, :ok]
         ]
      )

      dialog.default_response = Gtk::ResponseType::OK
      display_entry = Gtk::Entry.new
      display_entry.activates_default = true
      dialog_label =
        Gtk::Label.new("Please enter the name of\nthe new display\n")

      dialog.child.add(dialog_label)
      dialog.child.add(display_entry)

      display_entry.grab_focus
      dialog.child.show_all

      result = nil
      until result
        response_id = dialog.run
        break if response_id != Gtk::ResponseType::OK

        new_screen_name = display_entry.text

        unless new_screen_name.empty?
          begin
            result = Gdk::Dispaly.open(new_screen_name)
          rescue
            dialog_label.text = "Can't open display :\n\t%s\nplease try another one\n" % [new_screen_name]
          end
        end
      end
      dialog.destroy
    end

    # Called when the selected row in the display list changes.
    # Updates info.current_display, then refills the list of
    # screens.
    def display_changed_cb(selection)
      @current_display =
        if iter = selection.selected
          iter.get_value(DISPLAY_COLUMN_DISPLAY)
        else
          nil
        end
      fill_screens
    end

    # Adds a new display to our list of displays, and connects
    # to the 'closed' signal so that we can remove it from the
    # list of displays again.
    def add_display(display)
      iter = @display_model.append
      iter.set_value(DISPLAY_COLUMN_NAME, display.name)
      iter.set_value(DISPLAY_COLUMN_DISPLAY, display)

      handler_id = display.signal_connect('closed') do
        display_closed_cb(display)
      end

      signal_connect('destroy') do
        display.signal_handler_disconnect(handler_id)
      end
    end

    # Called when one of the currently open displays is closed.
    # Remove it from our list of displays.
    def display_closed_cb(display)
      @display_model.each do |model, path, iter|
        tmp_display = iter.get_value( DISPLAY_COLUMN_DISPLAY)
        if tmp_display == display
          @display_model.remove(iter)
          break
        end
      end
    end
  end
end

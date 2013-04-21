# Copyright (c) 2013 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Assistant

Demonstrates a sample multi-step assistant. Assistants are used to divide an operation into several simpler sequential steps, and to guide the user through these steps.
=end
require 'common'

module Demo
  class Assistant < Gtk::Assistant
    def initialize
      super()
      self.set_default_size -1, 300

      create_page1
      create_page2
      create_page3
      create_page4

      self.signal_connect(:cancel)  {on_assistant_close_cancel}
      self.signal_connect(:close)   {on_assistant_close_cancel}
      self.signal_connect(:apply)   {on_assistant_apply}
      self.signal_connect(:prepare) {on_assistant_prepare}
      self.show
    end

    def create_page1
      box = Gtk::Box.new :horizontal, 12 
      box.border_width = 12

      label = Gtk::Label.new 'You must fill out this entry to continue:'
      box.pack_start label, :expand => false, :fill => false, :padding => 0

      entry = Gtk::Entry.new
      entry.activates_default = true
      box.pack_start entry, :expand => true, :fill => true, :padding => 0
      entry.signal_connect(:changed) do
        page_number = self.current_page
        current_page = self.get_nth_page page_number
        text = entry.text
        self.set_page_complete current_page, text.length() > 0
      end

      box.show_all
      self.append_page box
      self.set_page_title box, 'Page 1'
      self.set_page_type box, :intro
    end

    def create_page2
      box = Gtk::Box.new :vertical, 12
      box.border_width = 12

      checkbutton = Gtk::CheckButton.new 'This is optional data, you may ' +
                                         'continue even if you do not check this'
      box.pack_start checkbutton, :expand => false, :fill => false, :padding => 0

      box.show_all
      self.append_page box
      self.set_page_complete box, true
      self.set_page_title box, 'Page 2'
    end

    def create_page3
      label = Gtk::Label.new 'This is a confirmation page, press \'Apply\' to apply changes'

      label.show
      self.append_page label
      self.set_page_type label, :confirm
      self.set_page_complete label, true
      self.set_page_title label, 'Confirmation'
    end

    def create_page4
      @progress_bar = Gtk::ProgressBar.new
      @progress_bar.halign = :center
      @progress_bar.valign = :center

      @progress_bar.show
      self.append_page @progress_bar
      self.set_page_type @progress_bar, :progress
      self.set_page_title @progress_bar, 'Applying changes'

      # This prevents the assistant window from being
      # closed while we're "busy" applying changes.
      self.set_page_complete @progress_bar, false
    end

    def on_assistant_prepare
      current_page = self.current_page
      n_pages = self.n_pages

      title = "Sample assistant (%d of %d)" % [current_page + 1, n_pages]
      self.title = title

      # The fourth page (counting from zero) is the progress page.  The
      # user clicked Apply to get here so we tell the assistant to commit,
      # which means the changes up to this point are permanent and cannot
      # be cancelled or revisited.
      self.commit if current_page == 3
    end

    def on_assistant_apply
      # Start a timer to simulate changes taking a few seconds to apply.
      GLib::Timeout.add(100) do
        # Work, work, work...
        fraction = @progress_bar.fraction
        fraction += 0.05

        if fraction < 1.0
            @progress_bar.fraction = fraction
            true
        else
            # Close automatically once changes are fully applied.
            self.destroy
            false
        end
      end
    end

    def on_assistant_close_cancel
      self.destroy
    end
  end
end
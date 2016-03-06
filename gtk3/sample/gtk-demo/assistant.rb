# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Assistant

 Demonstrates a sample multi-step assistant. Assistants are used to divide
 an operation into several simpler sequential steps, and to guide the user
 through these steps.
=end
module AssistantDemo
  def self.run_demo(main_window)
    assistant = Gtk::Assistant.new
    assistant.screen = main_window.screen

    create_page1(assistant)
    create_page2(assistant)
    create_page3(assistant)
    progress_bar = create_page4(assistant)

    assistant.signal_connect("cancel", &:destroy)

    assistant.signal_connect("close", &:destroy)

    assistant.signal_connect "apply" do |widget|
      GLib::Timeout.add(100) do
        fraction = progress_bar.fraction + 0.05
        if fraction < 1.0
          progress_bar.fraction = fraction
        else
          widget.destroy
          GLib::Source::REMOVE
        end
      end
    end

    assistant.signal_connect "prepare" do |widget, _page|
      current_page = widget.current_page
      n_pages = widget.n_pages

      widget.title = "Sample assistant (#{current_page + 1} of #{n_pages}"

      widget.commit if current_page == 3
    end

    if !assistant.visible?
      assistant.show_all
    else
      assistant.destroy
    end
    assistant
  end

  def self.create_page1(assistant)
    box = Gtk::Box.new(:horizontal, 12)
    box.border_width = 12

    label = Gtk::Label.new("You must fill out this entry to continue")
    box.pack_start(label, :expand => true, :fill => true, :padding => 0)

    entry = Gtk::Entry.new
    entry.activates_default = true
    box.pack_start(entry, :expand => true, :fill => true, :padding => 0)

    entry.signal_connect "changed" do |widget|
      page_number = assistant.current_page
      current_page = assistant.get_nth_page(page_number)

      if widget.text
        assistant.set_page_complete(current_page, true)
      else
        assistant.set_page_complete(current_page, false)
      end
    end

    box.show_all
    assistant.append_page(box)
    assistant.set_page_title(box, "Page 1")
    assistant.set_page_type(box, :intro)
  end

  def self.create_page2(assistant)
    box = Gtk::Box.new(:horizontal, 12)
    box.border_width = 12

    checkbutton = Gtk::CheckButton.new(<<-LABEL)
This is optional data, you may continue
even if you do not check this
LABEL
    box.pack_start(checkbutton, :expand => false, :fill => false, :padding => 0)

    box.show_all
    assistant.append_page(box)
    assistant.set_page_complete(box, true)
    assistant.set_page_title(box, "Page 1")
  end

  def self.create_page3(assistant)
    label = Gtk::Label.new("This is a confirmation page, press 'Apply' to apply changes")
    label.show

    assistant.append_page(label)
    assistant.set_page_type(label, :confirm)
    assistant.set_page_complete(label, true)
    assistant.set_page_title(label, "Confirmation")
  end

  def self.create_page4(assistant)
    progress_bar = Gtk::ProgressBar.new
    progress_bar.halign = :center
    progress_bar.valign = :center

    progress_bar.show
    assistant.append_page(progress_bar)
    assistant.set_page_type(progress_bar, :progress)
    assistant.set_page_title(progress_bar, "Applying changes")

    # This prevents the assistant window from being
    # closed while we're "busy" applying changes.
    assistant.set_page_complete(progress_bar, false)
    progress_bar
  end
end

# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Spinner

GtkSpinner allows to show that background activity is on-going.
=end
module SpinnerDemo
  def self.run_demo(main_window)
    window = Gtk::Dialog.new(:title => "Spinner",
                             :parent => main_window,
                             :flags => nil,
                             :buttons => [[:close, :none]])

    window.set_resizable(false)
    window.signal_connect("response") { window.destroy }
    window.signal_connect("destroy") { window.destroy }

    content_area = window.content_area

    vbox = Gtk::Box.new(:vertical, 5)
    content_area.pack_start(vbox, :expand => true, :fill => true, :padding => 0)
    vbox.set_border_width(5)

    # Sensitive
    hbox = Gtk::Box.new(:horizontal, 5)
    spinner_sensitive = Gtk::Spinner.new
    hbox.add(spinner_sensitive)
    hbox.add(Gtk::Entry.new)
    vbox.add(hbox)

    # Disabled
    hbox = Gtk::Box.new(:horizontal, 5)
    spinner_insensitive = Gtk::Spinner.new
    hbox.add(spinner_insensitive)
    hbox.add(Gtk::Entry.new)
    vbox.add(hbox)
    hbox.set_sensitive(false)

    button = Gtk::Button.new(:stock_id => :media_play)
    button.signal_connect "clicked" do
      spinner_sensitive.start
      spinner_insensitive.start
    end
    vbox.add(button)

    button = Gtk::Button.new(:stock_id => :media_stop)
    button.signal_connect "clicked" do
      spinner_sensitive.stop
      spinner_insensitive.stop
    end
    vbox.add(button)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end

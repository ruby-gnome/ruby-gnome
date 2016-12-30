# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Spinner

GtkSpinner allows to show that background activity is on-going.
=end
class SpinnerDemo
  def initialize(main_window)
    @window = Gtk::Dialog.new(:title => "Spinner",
                              :parent => main_window,
                              :flags => nil,
                              :buttons => [[:close, :none]])

    @window.resizable = false
    @window.signal_connect("response") { @window.destroy }
    @window.signal_connect("destroy") { @window.destroy }

    initialize_vertical_box
    initialize_sensitive_box
    initialize_insensitive_box

    button = Gtk::Button.new(:stock_id => :media_play)
    button.signal_connect "clicked" do
      @spinner_sensitive.start
      @spinner_insensitive.start
    end
    @vbox.add(button)

    button = Gtk::Button.new(:stock_id => :media_stop)
    button.signal_connect "clicked" do
      @spinner_sensitive.stop
      @spinner_insensitive.stop
    end
    @vbox.add(button)
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

  def initialize_vertical_box
    content_area = @window.content_area

    @vbox = Gtk::Box.new(:vertical, 5)
    content_area.pack_start(@vbox,
                            :expand => true, :fill => true, :padding => 0)
    @vbox.margin = 5
  end

  def initialize_sensitive_box
    # Sensitive
    hbox = Gtk::Box.new(:horizontal, 5)
    @spinner_sensitive = Gtk::Spinner.new
    hbox.add(@spinner_sensitive)
    hbox.add(Gtk::Entry.new)
    @vbox.add(hbox)
  end

  def initialize_insensitive_box
    # Disabled
    hbox = Gtk::Box.new(:horizontal, 5)
    @spinner_insensitive = Gtk::Spinner.new
    hbox.add(@spinner_insensitive)
    hbox.add(Gtk::Entry.new)
    hbox.sensitive = false
    @vbox.add(hbox)
  end
end

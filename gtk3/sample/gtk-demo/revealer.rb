# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Revealer

GtkRevealer is a container that animates showing and hiding
of its sole child with nice transitions.
=end
class RevealerDemo
  def initialize(main_window)
    @builder = Gtk::Builder.new(:resource => "/revealer/revealer.ui")
    @builder.connect_signals {}

    @timeout = nil
    @count = 0

    @window = @builder["window"]
    @window.screen = main_window.screen
    @window.signal_connect "destroy" do
      if @timeout
        GLib::Source.remove(@timeout)
        @timeout = nil
      end
    end
  end

  def run
    if !@window.visible?
      add_timeout
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def add_timeout
    @timeout = GLib::Timeout.add(690) do
      name = "revealer#{@count}"

      revealer = @builder[name]
      revealer.reveal_child = true

      revealer.signal_connect "notify::child-revealed" do |widget|
        if widget.mapped?
          revealed = widget.child_revealed?
          widget.reveal_child = !revealed
        end
      end

      @count += 1
      if @count >= 9
        @timeout = nil
        false
      else
        true
      end
    end
  end
end

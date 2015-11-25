# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Revealer

GtkRevealer is a container that animates showing and hiding
of its sole child with nice transitions.
=end
module RevealerDemo
  def self.run_demo(main_window)
    builder = Gtk::Builder.new(:resource => "/revealer/revealer.ui")
    builder.connect_signals {}

    timeout = nil
    count = 0

    window = builder["window"]
    window.screen = main_window.screen
    window.signal_connect "destroy" do
      if timeout
        GLib::Source.remove(timeout)
        timeout = nil
      end
    end

    if !window.visible?
      timeout = GLib::Timeout.add(690) do
        name = "revealer#{count}"

        revealer = builder[name]
        revealer.set_reveal_child(true)

        revealer.signal_connect "notify::child-revealed" do |widget|
          revealed = widget.child_revealed?
          widget.set_reveal_child(!revealed)
        end

        count += 1
        if count >= 9
          timeout = nil
          false
        else
          true
        end
      end
      window.show_all
    else
      window.destroy
    end
    window
  end
end

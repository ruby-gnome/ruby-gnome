# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Links

GtkLabel can show hyperlinks. The default action is to call
gtk_show_uri() on their URI, but it is possible to override
this with a custom handler.
=end
module LinksDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Links")
    window.set_border_width(12)

    label = Gtk::Label.new(<<EOF)
Some <a href="http://en.wikipedia.org/wiki/Text"
title="plain text">text</a> may be marked up
as hyperlinks, which can be clicked
or activated via <a href="keynav">keynav</a>
and they work fine with other markup, like when
searching on <a href="http://www.google.com/">
<span color="#0266C8">G</span><span color="#F90101">o</span>
<span color="#F2B50F">o</span><span color="#0266C8">g</span>
<span color="#00933B">l</span><span color="#F90101">e</span>
</a>.
EOF
    label.set_use_markup(true)

    label.signal_connect "activate-link" do |_widget, uri|
      if uri == "keynav"
        parent = label.toplevel
        dialog = Gtk::MessageDialog.new(:parent => parent,
                                        :flags => :destroy_with_parent,
                                        :type => :info,
                                        :buttons => :ok,
                                        :message => <<EOF)
The term <i>keynav</i> is a shorthand for
keyboard navigation and refers to the process of using
a program (exclusively) via keyboard input.
EOF
        dialog.set_use_markup(true)
        dialog.set_modal(true)
        dialog.present
        dialog.signal_connect "response" do
          dialog.destroy
        end
        true
      else
        false
      end
    end

    window.add(label)
    label.show

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end

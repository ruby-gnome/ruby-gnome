# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Links

GtkLabel can show hyperlinks. The default action is to call
gtk_show_uri() on their URI, but it is possible to override
this with a custom handler.
=end
class LinksDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Links"

    label = Gtk::Label.new(<<-MESSAGE)
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
    MESSAGE
    label.use_markup = true

    label.signal_connect "activate-link" do |widget, uri|
      if uri == "keynav"
        generate_dialog(widget)
        true
      else
        false
      end
    end

    @window.add(label)
    label.show
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

  def generate_dialog(widget)
    dialog = Gtk::MessageDialog.new(:parent => widget.toplevel,
                                    :flags => :destroy_with_parent,
                                    :type => :info,
                                    :buttons => :ok,
                                    :message => <<-MESSAGE)
The term <i>keynav</i> is a shorthand for
keyboard navigation and refers to the process of using
a program (exclusively) via keyboard input.
        MESSAGE
    dialog.use_markup = true
    dialog.modal = true
    dialog.present
    dialog.signal_connect("response", &:destroy)
  end
end

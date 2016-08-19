# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Expander

GtkExpander allows to provide additional content that is initially hidden.
This is also known as "disclosure triangle".

This example also shows how to make the window resizable only if the expander is expanded.
=end
class ExpanderDemo
  def initialize(main_window)
    toplevel = main_window.toplevel
    message = "<big><b>Something went wrong</b></big>"
    @window = Gtk::MessageDialog.new(:parent => toplevel,
                                     :flags  => :destroy_with_parent,
                                     :type   => :error,
                                     :buttons_type => :close,
                                     :message      => message)

    @window.use_markup = true

    message = "Here are some more details but not the full story."
    @window.secondary_text = message

    area = @window.message_area
    box = area.parent
    box.parent.child_set_property(box, "expand", true)
    box.parent.child_set_property(box, "fill", true)

    area.each do |child|
      child.parent.child_set_property(child, "expand", false)
    end

    expander = Gtk::Expander.new("Details:")
    sw = Gtk::ScrolledWindow.new
    sw.set_min_content_height(100)
    sw.set_shadow_type(:in)
    sw.set_policy(:never, :automatic)

    tv = initialize_text_view
    sw.add(tv)
    expander.add(sw)
    area.pack_end(expander, :expand => true, :fill => true, :padding => 0)
    expander.show_all

    expander.signal_connect "notify::expanded" do
      @window.resizable = expander.expanded?
    end

    @window.signal_connect "response" do |dialog, _response_id|
      dialog.destroy
    end
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end

  private

  def initialize_text_view
    tv = Gtk::TextView.new
    tv.editable  = false
    tv.wrap_mode = :word
    tv.buffer.text = <<TEXT
Finally, the full story with all details.
And all the inside information, including
error codes, etc etc. Pages of information,
you might have to scroll down to read it all,
or even resize the window - it works !

A second paragraph will contain even more
innuendo, just to make you scroll down or
resize the window. Do it already !"
TEXT
    tv
  end
end

=begin

  dialog.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  $Id: dialog.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

=end

require 'sample'

class DialogSample < SampleDialog

  def initialize
    super("Gtk::Dialog")
    set_default_size(200, 100)

    button = Gtk::Button::new("OK")
    button.flags |= Gtk::Widget::CAN_DEFAULT
    action_area.pack_start(button)
    button.grab_default

    button = Gtk::Button::new("Toggle")
    button.signal_connect("clicked") do label_toggle end
    button.flags |= Gtk::Widget::CAN_DEFAULT
    action_area.pack_start(button)

    @label = nil
  end

  private
  def label_toggle
    if @label == nil then
      @label = Gtk::Label::new("Dialog Test")
      @label.signal_connect("destroy") do @label = nil end
      @label.set_padding(10, 10)
      vbox.pack_start(@label)
      @label.show
    else
      @label.destroy
    end
  end

end

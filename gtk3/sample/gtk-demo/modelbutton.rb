# Copyright (c) 2008-2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Model Button

GtkModelButton is a button widget that is designed to be used with
a GAction as model. The button will adjust its appearance according
to the kind of action it is connected to.

It is also possible to use GtkModelButton without a GAction. In this
case, you should set the "role" attribute yourself, and connect to the
"clicked" signal as you would for any other button.

A common use of GtkModelButton is to implement menu-like content
in popovers.
=end
module ModelbuttonDemo

  def self.run_demo(main_window)
    builder = Gtk::Builder.new(:resource => "/modelbutton/modelbutton.ui")
    
    builder.add_callback_symbol("tool_clicked") 
    # Wait for issue # 623 
    window = builder["window1"]
    window.screen = main_window.screen
    actions = Gio::SimpleActionGroup.new
    actions.add_actions( [
      {:name => "color", :parameter_type => "s", :state => "red"},
      {:name => "chocolate", :state => true},
      {:name => "vanilla", :state => false},
      {:name => "sprinkles"}
    ])
    window.insert_action_group("win", actions)
    if !window.visible?
      window.show_all
    else
      window.destroy
    end
  end
end

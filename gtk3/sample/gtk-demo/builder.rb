# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Builder

 Demonstrates an interface loaded from a XML description.
=end
module BuilderDemo
  def self.run_demo(_main_window)
    builder = Gtk::Builder.new(:resource => "/builder/demo.ui")
    builder.connect_signals {}

    window = builder["window1"]
    toolbar = builder["toolbar1"]
    toolbar.style_context.add_class("primary-toolbar")

    actions = Gio::SimpleActionGroup.new

    action = Gio::SimpleAction.new("quit")
    action.signal_connect "activate" do |_simple_action, _parameter|
      window.destroy
    end
    actions.add_action(action)

    action = Gio::SimpleAction.new("about")
    actions.add_action(action)
    action.signal_connect "activate" do |_simple_action, _parameter|
      about_dlg = builder["aboutdialog1"]
      about_dlg.run
      about_dlg.hide
    end

    action = Gio::SimpleAction.new("help")
    actions.add_action(action)
    action.signal_connect "activate" do |_simple_action, _parameter|
      puts "Help not available"
    end

    window.insert_action_group("win", actions)

    accel_group = Gtk::AccelGroup.new
    window.add_accel_group(accel_group)

    builder["new_item"].add_accelerator("activate", accel_group,
                                        Gdk::Keyval::KEY_n,
                                        Gdk::ModifierType::CONTROL_MASK,
                                        Gtk::AccelFlags::VISIBLE)
    builder["open_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_o,
                                         Gdk::ModifierType::CONTROL_MASK,
                                         Gtk::AccelFlags::VISIBLE)
    builder["save_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_s,
                                         Gdk::ModifierType::CONTROL_MASK,
                                         Gtk::AccelFlags::VISIBLE)
    builder["quit_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_q,
                                         Gdk::ModifierType::CONTROL_MASK,
                                         Gtk::AccelFlags::VISIBLE)
    builder["copy_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_c,
                                         Gdk::ModifierType::CONTROL_MASK,
                                         Gtk::AccelFlags::VISIBLE)
    builder["cut_item"].add_accelerator("activate", accel_group,
                                        Gdk::Keyval::KEY_x,
                                        Gdk::ModifierType::CONTROL_MASK,
                                        Gtk::AccelFlags::VISIBLE)
    builder["paste_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_v,
                                          Gdk::ModifierType::CONTROL_MASK,
                                          Gtk::AccelFlags::VISIBLE)
    builder["help_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_F1,
                                         0,
                                         Gtk::AccelFlags::VISIBLE)
    builder["about_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_F7,
                                          0,
                                          Gtk::AccelFlags::VISIBLE)
    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end

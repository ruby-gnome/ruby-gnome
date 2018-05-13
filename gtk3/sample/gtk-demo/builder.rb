# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Builder

 Demonstrates an interface loaded from a XML description.
=end
class BuilderDemo
  def initialize(_main_window)
    if Gtk::Version.or_later?(3, 20)
      ui_path = "/builder/demo.ui"
    else
      ui_path = "/builder/demo-3.18.ui"
    end
    @builder = Gtk::Builder.new(:resource => ui_path)
    @builder.connect_signals {}

    @window = @builder["window1"]
    toolbar = @builder["toolbar1"]
    toolbar.style_context.add_class("primary-toolbar")


    add_actions
    add_accelerators
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

  def add_actions
    actions = Gio::SimpleActionGroup.new
    actions.add_action(quit_action)
    actions.add_action(about_action)
    actions.add_action(help_action)
    @window.insert_action_group("win", actions)
  end

  def quit_action
    action = Gio::SimpleAction.new("quit")
    action.signal_connect "activate" do |_simple_action, _parameter|
      @window.destroy
    end
    action
  end

  def about_action
    action = Gio::SimpleAction.new("about")
    action.signal_connect "activate" do |_simple_action, _parameter|
      about_dlg = @builder["aboutdialog1"]
      about_dlg.run
      about_dlg.hide
    end
    action
  end

  def help_action
    action = Gio::SimpleAction.new("help")
    action.signal_connect "activate" do |_simple_action, _parameter|
      puts "Help not available"
    end
    action
  end

  def add_accelerators
    accel_group = Gtk::AccelGroup.new

    @builder["new_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_n,
                                         Gdk::ModifierType::CONTROL_MASK,
                                         Gtk::AccelFlags::VISIBLE)
    @builder["open_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_o,
                                          Gdk::ModifierType::CONTROL_MASK,
                                          Gtk::AccelFlags::VISIBLE)
    @builder["save_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_s,
                                          Gdk::ModifierType::CONTROL_MASK,
                                          Gtk::AccelFlags::VISIBLE)
    @builder["quit_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_q,
                                          Gdk::ModifierType::CONTROL_MASK,
                                          Gtk::AccelFlags::VISIBLE)
    @builder["copy_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_c,
                                          Gdk::ModifierType::CONTROL_MASK,
                                          Gtk::AccelFlags::VISIBLE)
    @builder["cut_item"].add_accelerator("activate", accel_group,
                                         Gdk::Keyval::KEY_x,
                                         Gdk::ModifierType::CONTROL_MASK,
                                         Gtk::AccelFlags::VISIBLE)
    @builder["paste_item"].add_accelerator("activate", accel_group,
                                           Gdk::Keyval::KEY_v,
                                           Gdk::ModifierType::CONTROL_MASK,
                                           Gtk::AccelFlags::VISIBLE)
    @builder["help_item"].add_accelerator("activate", accel_group,
                                          Gdk::Keyval::KEY_F1,
                                          0,
                                          Gtk::AccelFlags::VISIBLE)
    @builder["about_item"].add_accelerator("activate", accel_group,
                                           Gdk::Keyval::KEY_F7,
                                           0,
                                           Gtk::AccelFlags::VISIBLE)
    @window.add_accel_group(accel_group)
  end
end

# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: panes.rb,v 1.5 2005/02/12 23:06:07 kzys Exp $
=begin
= Paned Widgets

The Gtk::HPaned and Gtk::VPaned Widgets divide their content
area into two panes with a divider in between that the
user can adjust. A separate child is placed into each
pane.

There are a number of options that can be set for each pane.
This test contains both a horizontal (HPaned) and a vertical
(VPaned) widget, and allows you to adjust the options for
each side of each widget.
=end
require 'common'

module Demo
  class Panes < BasicWindow
    def initialize
      super('Panes')
      self.border_width = 0

      vbox = Gtk::VBox.new(false, 0)
      add(vbox)

      vpaned = Gtk::VPaned.new
      vbox.pack_start(vpaned, true, true, 0)
      vpaned.border_width = 5

      hpaned = Gtk::HPaned.new
      vpaned.add1(hpaned)

      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      frame.set_size_request(60, 60)
      hpaned.add1(frame)

      button = Gtk::Button.new('_Hi there', true)
      frame.add(button)

      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      frame.set_size_request(80, 60)
      hpaned.add2(frame)

      frame = Gtk::Frame.new
      frame.shadow_type = Gtk::SHADOW_IN
      frame.set_size_request(60, 80)
      vpaned.add2(frame)

      # Now create toggle buttons to control sizing

      vbox.pack_start(create_pane_options(hpaned,
					  'Horizontal', 'Left', 'Right'),
		      false, false, 0)

      vbox.pack_start(create_pane_options(vpaned,
					  'Vertical', 'Top', 'Bottom'),
		      false, false, 0)
    end

    def create_pane_options(paned, frame_label, label1, label2)
      frame = Gtk::Frame.new(frame_label)
      frame.border_width = 4

      table = Gtk::Table.new(3, 2, true)
      frame.add(table)

      label = Gtk::Label.new(label1)
      table.attach_defaults(label, 0, 1, 0, 1)

      check_button = Gtk::CheckButton.new('_Resize', true)
      table.attach_defaults(check_button, 0, 1, 1, 2)
      check_button.signal_connect('toggled') do
	toggle_resize(paned.child1)
      end

      check_button = Gtk::CheckButton.new('_Shrink', true)
      table.attach_defaults(check_button, 0, 1, 2, 3)
      check_button.active = true
      check_button.signal_connect('toggled') do
	toggle_shrink(paned.child1)
      end

      label = Gtk::Label.new(label2)
      table.attach_defaults(label, 1, 2, 0, 1)

      check_button = Gtk::CheckButton.new('_Resize')
      table.attach_defaults(check_button, 1, 2, 1, 2)
      check_button.active = true
      check_button.signal_connect('toggled') do
	toggle_resize(paned.child2)
      end

      check_button = Gtk::CheckButton.new('_Shrink')
      table.attach_defaults(check_button, 1, 2, 2, 3)
      check_button.active = true
      check_button.signal_connect('toggled') do
	toggle_shrink(paned.child2)
      end

      return frame
    end

    def toggle_resize(child)
      paned = child.parent
      is_child1 = (child == paned.child1)

      resize = if is_child1
		 paned.child1_resize?
	       else
		 paned.child2_resize?
	       end

      shrink = if is_child1
		 paned.child1_shrink?
	       else
		 paned.child2_shrink?
	       end

      child.parent.remove(child)
      if is_child1
	paned.pack1(child, !resize, shrink)
      else
	paned.pack2(child, !resize, shrink)
      end
    end

    def toggle_shrink(child)
      paned = child.parent
      is_child1 = (child == paned.child1)

      resize = if is_child1
		 paned.child1_resize?
	       else
		 paned.child2_resize?
	       end

      shrink = if is_child1
		 paned.child1_shrink?
	       else
		 paned.child2_shrink?
	       end

      child.parent.remove(child)
      if is_child1
	paned.pack1(child, resize, !shrink)
      else
	paned.pack2(child, resize, !shrink)
      end
    end
  end
end

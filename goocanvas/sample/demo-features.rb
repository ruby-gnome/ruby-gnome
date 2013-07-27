#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/demo-features.c. It is licensed
# under the terms of the GNU Library General Public License, version
# 2 or (at your option) later.
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

class CanvasSampleFeatures < Gtk::Box
  def initialize
    super(:vertical, 4)
    set_border_width(4)
    show
    create_canvas_features(self)
  end

  private
  def create_canvas_features(box)
    instructions(box)
    root = frame_and_canvas(box)
    parent1 = first_parent_and_box(root)
    parent2 = second_parent_and_box(root)
    big_circle_to_be_reparented(parent1, parent2)
    a_group_to_be_reparented(parent1, parent2)
  end

  def instructions(box)
    label = Gtk::Label.new(<<-EOT)
Reparent test:  click on the items to switch them between parents
    EOT
    box.pack_start(label, :expand  => false,
                          :fill    => false,
                          :padding => 0)
    label.show
  end

  def frame_and_canvas(box)
    alignment = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
    box.pack_start(alignment, :expand  => false,
                              :fill    => false,
                              :padding => 0)
    alignment.show

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    alignment.add(frame)
    frame.show

    canvas = Goo::Canvas.new
    root = canvas.root_item

    canvas.set_size_request(400, 200)
    canvas.set_bounds(0, 0, 400, 200)
    frame.add(canvas)
    canvas.show

    root
  end

  def first_parent_and_box(root)
    parent1 = Goo::CanvasGroup.new(:parent => root)
    Goo::CanvasRect.new(:parent     => parent1,
                        :x          => 0,
                        :y          => 0,
                        :width      => 200,
                        :height     => 200,
                        :fill_color => "tan")
    parent1
  end

  def second_parent_and_box(root)
    parent2 = Goo::CanvasGroup.new(:parent => root)
    parent2.translate(200, 0)
    Goo::CanvasRect.new(:parent     => parent2,
                        :x          => 0,
                        :y          => 0,
                        :width      => 200,
                        :height     => 200,
                        :fill_color => "#204060")
    parent2
  end

  def big_circle_to_be_reparented(parent1, parent2)
    item = Goo::CanvasEllipse.new(:parent       => parent1,
                                  :center_x     => 100,
                                  :center_y     => 100,
                                  :radius_x     => 90,
                                  :radius_y     => 90,
                                  :stroke_color => "black",
                                  :fill_color   => "mediumseagreen",
                                  :line_width   => 3.0)

    item.signal_connect("button-press-event") do |widget, event|
      on_button_press(widget, event, parent1, parent2)
    end
  end

  def a_group_to_be_reparented(parent1, parent2)
    group = Goo::CanvasGroup.new(:parent => parent2)
    group.translate(100, 100)

    Goo::CanvasEllipse.new(:parent       => group,
                           :center_x     => 0,
                           :center_y     => 0,
                           :radius_x     => 50,
                           :radius_y     => 50,
                           :stroke_color => "black",
                           :fill_color   => "wheat",
                           :line_width   => 3.0)

    Goo::CanvasEllipse.new(:parent     => group,
                           :center_x   => 0,
                           :center_y   => 0,
                           :radius_x   => 25,
                           :radius_y   => 25,
                           :fill_color => "steelblue")

    group.signal_connect("button-press-event") do |widget, event|
      on_button_press(widget, event, parent1, parent2)
    end
  end

  def on_button_press(item, event, parent1, parent2)
    puts "In on_button_press"
    parent = item.parent
    item.remove
    if parent == parent1
      parent2.add_child(item, -1)
    else
      parent1.add_child(item, -1)
    end
  end
end

#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/demo-events.c. It is licensed
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

class CanvasSampleEvents < Gtk::Box
  def initialize
    super(:vertical, 4)
    border_width = 4
    show
    create_events_page(self)
  end

  private
  def create_events_page(vbox)
    # Instructions

    label = Gtk::Label.new(<<-EOT)
Move the mouse over the items to check they receive the right motion events.
The first 2 items in each group are 1) invisible and 2) visible but unpainted.
    EOT
    vbox.pack_start(label, :expand  => false,
                           :fill    => false,
                           :padding => 0)
    label.show

    # Frame and canvas

    alignment = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
    vbox.pack_start(alignment, :expand  => false,
                               :fill    => false,
                               :padding => 0)
    alignment.show

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    alignment.add(frame)
    frame.show

    canvas = Goo::Canvas.new

    canvas.set_size_request(600, 450)
    canvas.set_bounds(0, 0, 600, 450)
    frame.add(canvas)
    canvas.show

    create_events_area(canvas, 0, :none, "none")
    create_events_area(canvas, 1, :visible_painted, "visible-painted")
    create_events_area(canvas, 2, :visible_fill, "visible-fill")
    create_events_area(canvas, 3, :visible_stroke, "visible-stroke")
    create_events_area(canvas, 4, :visible, "visible")
    create_events_area(canvas, 5, :painted, "painted")
    create_events_area(canvas, 6, :fill, "fill")
    create_events_area(canvas, 7, :stroke, "stroke")
    create_events_area(canvas, 8, :all, "all")
  end

  def create_events_area(canvas, area_num, pointer_events, label)
    row = area_num / 3
    col = area_num % 3
    x = col * 200
    y = row * 150

    root = canvas.root_item

    dash = Goo::CanvasLineDash.new(5.0, 5.0)

    # Create invisible item.
    rect = Goo::CanvasRect.new(:parent         => root,
                               :x              => x + 45,
                               :y              => y + 35,
                               :width          => 30,
                               :height         => 30,
                               :fill_color     => "red",
                               :visibility     => :invisible,
                               :line_width     => 5.0,
                               :pointer_events => pointer_events)
    view_id = sprintf("%s invisible", label)
    setup_item_signals(rect, view_id)

    # Display a thin rect around it to indicate it is there.
    rect = Goo::CanvasRect.new(:parent       => root,
                               :x            => x + 42.5,
                               :y            => y + 32.5,
                               :width        => 36,
                               :height       => 36,
                               # TODO: [BUG] Segmentation fault
                               #:line_dash    => dash,
                               :line_width   => 1.0,
                               :stroke_color => "gray")

    # Create unpainted item.
    rect = Goo::CanvasRect.new(:parent         => root,
                               :x              => x + 85,
                               :y              => y + 35,
                               :width          => 30,
                               :height         => 30,
                               :stroke_pattern => nil,
                               :line_width     => 5.0,
                               :pointer_events => pointer_events)
    view_id = sprintf("%s unpainted", label)
    setup_item_signals(rect, view_id)

    # Display a thin rect around it to indicate it is there.
    rect = Goo::CanvasRect.new(:parent       => root,
                               :x            => x + 82.5,
                               :y            => y + 32.5,
                               :width        => 36,
                               :height       => 36,
                               # TODO: [BUG] Segmentation fault
                               #:line_dash    => dash,
                               :line_width   => 1.0,
                               :stroke_color => "gray")

    # Create stroked item.
    rect = Goo::CanvasRect.new(:parent         => root,
                               :x              => x + 125,
                               :y              => y + 35,
                               :width          => 30,
                               :height         => 30,
                               :line_width     => 5.0,
                               :pointer_events => pointer_events)
    view_id = sprintf("%s stroked", label)
    setup_item_signals(rect, view_id)

    # Create filled item.
    rect = Goo::CanvasRect.new(:parent         => root,
                               :x              => x + 60,
                               :y              => y + 75,
                               :width          => 30,
                               :height         => 30,
                               :fill_color     => "red",
                               :stroke_pattern => nil,
                               :line_width     => 5.0,
                               :pointer_events => pointer_events)
    view_id = sprintf("%s filled", label)
    setup_item_signals(rect, view_id)

    # Create stroked & filled item.
    rect = Goo::CanvasRect.new(:parent         => root,
                               :x              => x + 100,
                               :y              => y + 75,
                               :width          => 30,
                               :height         => 30,
                               :fill_color     => "red",
                               :line_width     => 5.0,
                               :pointer_events => pointer_events)
    view_id = sprintf("%s stroked & filled", label)
    setup_item_signals(rect, view_id)

    Goo::CanvasText.new(:parent     => root,
                        :text       => label,
                        :x          => x + 100,
                        :y          => y + 130,
                        :width      => -1,
                        :anchor     => :center,
                        :font       => "Sans 12",
                        :fill_color => "blue")
  end

  def setup_item_signals(item, item_id)
    item.signal_connect("motion_notify_event") do |widget, event|
      if item_id
        printf("%s item received 'motion-notify' signal\n", item_id)
      end
      false
    end
  end
end

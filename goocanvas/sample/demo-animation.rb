#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/demo-animation.c. It is licensed
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

class CanvasSampleAnimation < Gtk::Box
  def initialize
    super(:vertical, 4)
    set_border_width(4)
    show
    create_animation_page(self)
  end

  private
  def create_animation_page(vbox)
    hbox = Gtk::Box.new(:horizontal, 4)
    vbox.pack_start(hbox, :expand  => false,
                          :fill    => false,
                          :padding => 0)
    hbox.show

    start_button = Gtk::Button.new(:label => "Start Animation")
    hbox.pack_start(start_button, :expand  => false,
                                  :fill    => false,
                                  :padding => 0)
    start_button.show

    stop_button = Gtk::Button.new(:label => "Stop Animation")
    hbox.pack_start(stop_button, :expand  => false,
                                 :fill    => false,
                                 :padding => 0)
    stop_button.show

    scrolled_win = Gtk::ScrolledWindow.new
    scrolled_win.shadow_type = :in
    vbox.pack_start(scrolled_win, :expand  => true,
                                  :fill    => true,
                                  :padding => 0)
    scrolled_win.show

    canvas = Goo::Canvas.new
    canvas.set_size_request(600, 450)
    canvas.set_bounds(0, 0, 1000, 1000)
    canvas.show
    scrolled_win.add(canvas)

    items = setup_canvas(canvas)

    start_button.signal_connect("clicked") do
      start_animation_clicked(*items)
    end

    stop_button.signal_connect("clicked") do
      stop_animation_clicked(*items)
    end
  end

  def setup_canvas(canvas)
    root = canvas.root_item

    # Absolute.
    ellipse1 = Goo::CanvasEllipse.new(:parent     => root,
                                      :center_x   => 0,
                                      :center_y   => 0,
                                      :radius_x   => 25,
                                      :radius_y   => 15,
                                      :fill_color => "blue")
    ellipse1.translate(100, 100)
    ellipse1.signal_connect("animation_finished") do |_, event|
      on_animation_finished(event, ellipse1)
    end

    rect1 = Goo::CanvasRect.new(:parent     => root,
                                :x          => -10,
                                :y          => -10,
                                :width      => 20,
                                :height     => 20,
                                :fill_color => "blue")
    rect1.translate(100, 200)

    rect3 = Goo::CanvasRect.new(:parent     => root,
                                :x          => -10,
                                :y          => -10,
                                :width      => 20,
                                :height     => 20,
                                :fill_color => "blue")
    rect3.translate(200, 200)

    # Relative.
    ellipse2 = Goo::CanvasEllipse.new(:parent     => root,
                                      :center_x   => 0,
                                      :center_y   => 0,
                                      :radius_x   => 25,
                                      :radius_y   => 15,
                                      :fill_color => "red")
    ellipse2.translate(100, 400)

    rect2 = Goo::CanvasRect.new(:parent     => root,
                                :x          => -10,
                                :y          => -10,
                                :width      => 20,
                                :height     => 20,
                                :fill_color => "red")
    rect2.translate(100, 500)

    rect4 = Goo::CanvasRect.new(:parent     => root,
                                :x          => -10,
                                :y          => -10,
                                :width      => 20,
                                :height     => 20,
                                :fill_color => "red")
    rect4.translate(200, 500)

    [ellipse1, rect1, rect3, ellipse2, rect2, rect4]
  end

  def start_animation_clicked(ellipse1, rect1, rect3,
                              ellipse2, rect2, rect4)
    # Absolute.
    ellipse1.set_simple_transform(100, 100, 1, 0)
    ellipse1.animate(500, 100, 2, 720, true, 2000, 40,
                     Goo::CanvasAnimateType::BOUNCE)

    rect1.set_simple_transform(100, 200, 1, 0)
    rect1.animate(100, 200, 1, 350, true, 40 * 36, 40,
                  Goo::CanvasAnimateType::RESTART)

    rect3.set_simple_transform(200, 200, 1, 0)
    rect3.animate(200, 200, 3, 0, true, 400, 40,
                  Goo::CanvasAnimateType::BOUNCE)

    # Relative.
    ellipse2.set_simple_transform(100, 400, 1, 0)
    ellipse2.animate(400, 0, 2, 720, false, 2000, 40,
                     Goo::CanvasAnimateType::BOUNCE)

    rect2.set_simple_transform(100, 500, 1, 0)
    rect2.animate(0, 0, 1, 350, false, 40 * 36, 40,
                  Goo::CanvasAnimateType::RESTART)

    rect4.set_simple_transform(200, 500, 1, 0)
    rect4.animate(0, 0, 3, 0, false, 400, 40,
                  Goo::CanvasAnimateType::BOUNCE)
  end

  def stop_animation_clicked(*items)
    items.each do |item|
      item.stop_animation
    end
  end

  def on_animation_finished(event, ellipse1)
    puts "Animation finished stopped: #{event}"
    # Test starting another animation. */
    #ellipse1.animate(500, 100, 2, 720, true, 2000, 40,
    #                 Goo::CanvasAnimateType::BOUNCE)
  end
end

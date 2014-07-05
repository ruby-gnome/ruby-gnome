# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: cairo-operator.rb,v 1.1 2005/10/12 05:38:30 ktou Exp $
=begin
= cairo/Operator

This demo shows how to use GDK and cairo to use Cairo::OPERATOR_*
From http://cairographics.org/samples/operator_add.html,
     http://cairographics.org/samples/operator_atop.html,
     http://cairographics.org/samples/operator_atop_reverse.html,
     http://cairographics.org/samples/operator_in.html,
     http://cairographics.org/samples/operator_in_reverse.html,
     http://cairographics.org/samples/operator_out.html,
     http://cairographics.org/samples/operator_out_reverse.html,
     http://cairographics.org/samples/operator_over.html,
     http://cairographics.org/samples/operator_over_reverse.html,
     http://cairographics.org/samples/operator_saturate.html and
     http://cairographics.org/samples/operator_xor.html
=end
require "common"

module Demo
  class CairoOperator < CairoWindow
    def initialize
      super("cairo operator")
      @operator = Cairo::OPERATOR_ADD

      set_default_size(400, 400)

      @drawing_area = child
      remove(@drawing_area)

      vbox = Gtk::Box.new(:vertical)
      vbox.pack_start(@drawing_area, :expand => true, :fill => true)
      vbox.pack_start(operator_selector, :expand => false, :fill => false)
      add(vbox)
    end

    def draw(cr)
      cr.save do
        image = Cairo::ImageSurface.from_png("ruby-gnome2-logo.png")
        cr.translate(0.5, 0.5)
        cr.rotate(-45 * Math::PI / 180)
        cr.scale(0.8 / image.width, 0.8 / image.height)
        cr.translate(-0.5 * image.width, -0.5 * image.height)
        cr.set_source(image, 0.0, 0.0)
        cr.paint
      end

      cr.set_operator(@operator)

      cr.set_source_rgba(1, 0, 0, 0.5)
      cr.rectangle(0.2, 0.2, 0.5, 0.5)
      cr.fill
      cr.set_source_rgba(0, 1, 0)
      cr.rectangle(0.4, 0.4, 0.4, 0.4)
      cr.fill
      cr.set_source_rgba(0, 0, 1)
      cr.rectangle(0.6, 0.6, 0.3, 0.3)
      cr.fill
    end

    def operator_selector
      combo = Gtk::ComboBoxText.new
      operators = []
      Cairo.constants.each do |name|
        operators << name if /^OPERATOR_/ =~ name
      end
      operators.sort.each_with_index do |name, i|
        combo.append_text(name.to_s)
        combo.set_active(i) if Cairo.const_get(name) == @operator
      end

      combo.signal_connect("changed") do |widget|
        text = widget.active_text
        @operator = Cairo.const_get(text) if text
        @drawing_area.queue_draw
      end
      combo
    end
  end
end

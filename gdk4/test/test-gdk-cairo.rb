# -*- coding: utf-8 -*-
#
# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

require "stringio"

class TestGdkCairo < Test::Unit::TestCase
  def setup
    output = StringIO.new
    surface = Cairo::PDFSurface.new(output, 10, 10)
    @context = Cairo::Context.new(surface)
  end

  def test_set_source_color
    color = Gdk::Color.new(0xffff, 0x0000, 0xffff)
    @context.source_color = color
    assert_equal(Cairo::Color::RGB.new(1.0, 0.0, 1.0),
                 @context.source.color)
  end

  sub_test_case "#set_source_rgba" do
    sub_test_case "RGBA" do
      def test_rgba
        rgba = Gdk::RGBA.new(0.1, 0.2, 0.3, 0.4)
        @context.source_rgba = rgba
        assert_equal(Cairo::Color::RGB.new(0.1, 0.2, 0.3, 0.4),
                     @context.source.color)
      end

      def test_values
        @context.set_source_rgba(0.1, 0.2, 0.3, 0.4)
        assert_equal([0.1, 0.2, 0.3, 0.4],
                     @context.source.rgba)
      end

      def test_array
        @context.set_source_rgba([0.1, 0.2, 0.3, 0.4])
        assert_equal([0.1, 0.2, 0.3, 0.4],
                     @context.source.rgba)
      end
    end

    sub_test_case "RGB" do
      def test_values
        @context.set_source_rgb(0.1, 0.2, 0.3)
        assert_equal([0.1, 0.2, 0.3, 1.0],
                    @context.source.rgba)
      end

      def test_array
        @context.set_source_rgb([0.1, 0.2, 0.3])
        assert_equal([0.1, 0.2, 0.3, 1.0],
                    @context.source.rgba)
      end
    end
  end
end

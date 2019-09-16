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

class TestGdkPixbuf < Test::Unit::TestCase
  def test_from_window
    gdk_window = Gdk::Window.default_root_window
    src_x = 0
    src_y = 0
    width = 0
    height = 0

    assert_nothing_raised do
      Gdk::Pixbuf.from_window(gdk_window,
                              src_x,
                              src_y,
                              width,
                              height)
    end
  end

  def test_from_surface
    surface = Cairo::ImageSurface.new(:argb32, 290, 200)
    src_x = 0
    src_y = 0
    width = 290
    height = 200

    pixbuf = Gdk::Pixbuf.from_surface(surface,
                                      src_x,
                                      src_y,
                                      width,
                                      height)

    assert_kind_of(Gdk::Pixbuf, pixbuf)
  end
end

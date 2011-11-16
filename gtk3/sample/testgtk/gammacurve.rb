=begin header

  gammacurve.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: gammacurve.rb,v 1.6 2005/07/17 16:55:27 mutoh Exp $

  Rewritten by TAKAHASHI Hitoshi <thitoshi@ne.scphys.kyoto-u.ac.jp>

Original Copyright:

  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

class GammaCurveSample < SampleWindow
  include Math

  def initialize
    super("Gtk::GammaCurve")
    set_border_width(10)

    @curve = Gtk::GammaCurve.new
    add(@curve)
    @count = 0
  end

  def update
    max = 127 + (@count % 2) * 128
    @curve.curve.set_range(0, max, 0, max)
    vec = []
    max.times do |i|
      vec[i] = (127 / sqrt(max)) * sqrt(i)
    end
    @curve.curve.set_vector(max, vec)
    @count += 1
  end

  attr_reader :count

  def GammaCurveSample.invoke
    @singleton = new if @singleton.nil? or @singleton.destroyed?
    @singleton.update
    unless @singleton.visible?
      @singleton.show_all
    end
    if @singleton.count == 4
      @singleton.destroy
    end
  end
end

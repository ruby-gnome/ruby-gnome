=begin header

  less.rb - Less test rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

Original Copyright:
 
  Author : Richard Hestilow <hestgray@ionet.net>

  Copyright (C) 1998 Free Software Foundation

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

=end

require 'sample'

class LessSample < SampleApp
  def initialize
    super(true, "testGNOME", "Less")
    less = Gnome::Less.new
    set_contents(less)
    less.set_fixed_font(true)
    less.show
    show
    less.show_command("ls")
  end
end
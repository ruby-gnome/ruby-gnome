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

class TestPty < Test::Unit::TestCase
  def setup
    flag = Vte::PtyFlags::DEFAULT
    @pty = Vte::Pty.new(flag)
  end

  def test_fd
    assert_nothing_raised do
      @pty.fd
    end
  end

  def test_size
    row = 80
    col = 24
    @pty.set_size(row, col)
    assert_equal([row, col], @pty.size)
  end

  def test_term
    term_type = "vt100"
    @pty.term = term_type
    assert_equal(term_type, @pty.term)
  end

  def test_utf8
    assert_nothing_raised do
      @pty.utf8 = true
    end
  end
end

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

class TestGdkKeyval < Test::Unit::TestCase
  def test_consntant_name
    assert_equal("a".unpack("c")[0], Gdk::Keyval::KEY_a)
  end
  
  def test_is_lower
    assert_equal(Gdk::Keyval.is_lower(Gdk::Keyval::KEY_A), false)
    assert_equal(Gdk::Keyval.is_lower(Gdk::Keyval::KEY_a), true)
  end

  def test_is_upper
    assert_equal(Gdk::Keyval.is_upper(Gdk::Keyval::KEY_A), true)
    assert_equal(Gdk::Keyval.is_upper(Gdk::Keyval::KEY_a), false)
  end
  
  def test_is_name
    assert_equal(Gdk::Keyval.name(Gdk::Keyval::KEY_A), 'A')
  end
  
  def test_to_lower
    assert_equal(Gdk::Keyval.to_lower(Gdk::Keyval::KEY_A), 97)
    assert_equal(Gdk::Keyval.to_lower(Gdk::Keyval::KEY_a), 97)
  end
  
  def test_to_upper
    assert_equal(Gdk::Keyval.to_upper(Gdk::Keyval::KEY_a), 65)
    assert_equal(Gdk::Keyval.to_upper(Gdk::Keyval::KEY_A), 65)
  end
  
  def test_to_unicode
    assert_equal(Gdk::Keyval.to_unicode(Gdk::Keyval::KEY_Adiaeresis), 196)
  end
end

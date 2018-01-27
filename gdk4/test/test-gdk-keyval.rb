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

  def test_to_name
    assert_equal("A", Gdk::Keyval.to_name(Gdk::Keyval::KEY_A))
  end

  def test_from_name
    assert_equal("A".unpack("c")[0], Gdk::Keyval.from_name("A"))
  end

  sub_test_case "#lower?" do
    def test_lower
      assert_true(Gdk::Keyval.lower?(Gdk::Keyval::KEY_a))
    end

    def test_upper
      assert_false(Gdk::Keyval.lower?(Gdk::Keyval::KEY_A))
    end
  end

  sub_test_case "#upper?" do
    def test_lower
      assert_false(Gdk::Keyval.upper?(Gdk::Keyval::KEY_a))
    end

    def test_upper
      assert_true(Gdk::Keyval.upper?(Gdk::Keyval::KEY_A))
    end
  end

  sub_test_case "#to_lower" do
    def test_lower
      assert_equal(Gdk::Keyval::KEY_a,
                   Gdk::Keyval.to_lower(Gdk::Keyval::KEY_a))
    end

    def test_upper
      assert_equal(Gdk::Keyval::KEY_a,
                   Gdk::Keyval.to_lower(Gdk::Keyval::KEY_A))
    end
  end

  sub_test_case "#to_upper" do
    def test_lower
      assert_equal(Gdk::Keyval::KEY_A,
                   Gdk::Keyval.to_upper(Gdk::Keyval::KEY_a))
    end

    def test_upper
      assert_equal(Gdk::Keyval::KEY_A,
                   Gdk::Keyval.to_upper(Gdk::Keyval::KEY_A))
    end
  end

  sub_test_case "#to_unicode" do
    def test_to_unicode
      assert_equal("\u02D8", # BREVE
                   [Gdk::Keyval.to_unicode(Gdk::Keyval::KEY_breve)].pack("U"))
    end
  end
end

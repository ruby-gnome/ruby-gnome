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

class TestGtkContainer < Test::Unit::TestCase
  class TestAdd < self
    def setup
      @container = Gtk::Layout.new
    end

    def test_shift
      element1 = Gtk::EventBox.new
      element2 = Gtk::EventBox.new
      element3 = Gtk::EventBox.new
      @container << element1 << element2 << element3
      assert_equal([element1, element2, element3],
                   @container.children)
    end
  end

  class TestEach < self
    def setup
      @container = Gtk::Layout.new
      @element1 = Gtk::EventBox.new
      @element2 = Gtk::EventBox.new
      @element3 = Gtk::EventBox.new
      @container << @element1 << @element2 << @element3
    end

    def test_enumerable
      assert_equal([@element1, @element2, @element3],
                   @container.to_a)
    end
  end

  class TestFocusChain < self
    def setup
      @container = Gtk::Layout.new
    end

    def test_set_explicitly
      @container.focus_chain = []
      assert_equal([], @container.focus_chain)
    end

    def test_not_set_explicitly
      assert_nil(@container.focus_chain)
    end
  end
end

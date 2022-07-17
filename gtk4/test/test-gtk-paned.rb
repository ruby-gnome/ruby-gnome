# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestGtkPaned < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @paned = Gtk::Paned.new(:horizontal)
  end

  sub_test_case "#pack1" do
    def setup
      super
      @child = Gtk::EventBox.new
    end

    test "no options" do
      @paned.pack1(@child)
      assert_equal({
                     :resize => false,
                     :shrink => true,
                   },
                   {
                     :resize => @paned.child_get_property(@child, :resize),
                     :shrink => @paned.child_get_property(@child, :shrink),
                   })
    end

    test ":resize => true" do
      @paned.pack1(@child, :resize => true)
      assert_equal({
                     :resize => true,
                     :shrink => true,
                   },
                   {
                     :resize => @paned.child_get_property(@child, :resize),
                     :shrink => @paned.child_get_property(@child, :shrink),
                   })
    end

    test ":shrink => false" do
      @paned.pack1(@child, :shrink => false)
      assert_equal({
                     :resize => false,
                     :shrink => false,
                   },
                   {
                     :resize => @paned.child_get_property(@child, :resize),
                     :shrink => @paned.child_get_property(@child, :shrink),
                   })
    end
  end

  sub_test_case "#pack2" do
    def setup
      super
      @child = Gtk::EventBox.new
    end

    test "no options" do
      @paned.pack2(@child)
      assert_equal({
                     :resize => true,
                     :shrink => true,
                   },
                   {
                     :resize => @paned.child_get_property(@child, :resize),
                     :shrink => @paned.child_get_property(@child, :shrink),
                   })
    end

    test ":resize => false" do
      @paned.pack2(@child, :resize => false)
      assert_equal({
                     :resize => false,
                     :shrink => true,
                   },
                   {
                     :resize => @paned.child_get_property(@child, :resize),
                     :shrink => @paned.child_get_property(@child, :shrink),
                   })
    end

    test ":shrink => false" do
      @paned.pack2(@child, :shrink => false)
      assert_equal({
                     :resize => true,
                     :shrink => false,
                   },
                   {
                     :resize => @paned.child_get_property(@child, :resize),
                     :shrink => @paned.child_get_property(@child, :shrink),
                   })
    end
  end
end

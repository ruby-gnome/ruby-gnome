# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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

  sub_test_case "#set_start_child" do
    def setup
      super
      @child = Gtk::Box.new(:horizontal)
    end

    test "no options" do
      @paned.set_start_child(@child)
      assert_equal({
                     resize: true,
                     shrink: true,
                   },
                   {
                     resize: @paned.resize_start_child?,
                     shrink: @paned.shrink_start_child?,
                   })
    end

    test "resize: true" do
      @paned.set_start_child(@child, resize: true)
      assert_equal({
                     resize: true,
                     shrink: true,
                   },
                   {
                     resize: @paned.resize_start_child?,
                     shrink: @paned.shrink_start_child?,
                   })
    end

    test "shrink: false" do
      @paned.set_start_child(@child, shrink: false)
      assert_equal({
                     resize: true,
                     shrink: false,
                   },
                   {
                     resize: @paned.resize_start_child?,
                     shrink: @paned.shrink_start_child?,
                   })
    end
  end

  sub_test_case "#set_end_child" do
    def setup
      super
      @child = Gtk::Box.new(:horizontal)
    end

    test "no options" do
      @paned.set_end_child(@child)
      assert_equal({
                     resize: true,
                     shrink: true,
                   },
                   {
                     resize: @paned.resize_end_child?,
                     shrink: @paned.shrink_end_child?,
                   })
    end

    test "resize: false" do
      @paned.set_end_child(@child, resize: false)
      assert_equal({
                     resize: false,
                     shrink: true,
                   },
                   {
                     resize: @paned.resize_end_child?,
                     shrink: @paned.shrink_end_child?,
                   })
    end

    test "shrink: false" do
      @paned.set_end_child(@child, shrink: false)
      assert_equal({
                     resize: true,
                     shrink: false,
                   },
                   {
                     resize: @paned.resize_end_child?,
                     shrink: @paned.shrink_end_child?,
                   })
    end
  end
end

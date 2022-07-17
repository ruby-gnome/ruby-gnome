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

class TestGtkAssistant < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @assistant = Gtk::Assistant.new
  end

  test "#set_forward_page_func" do
    page = nil
    next_page = 2
    @assistant.set_forward_page_func do |current_page|
      page = current_page
      next_page
    end

    page1 = Gtk::EventBox.new
    page2 = Gtk::EventBox.new
    page3 = Gtk::EventBox.new
    @assistant.append_page(page1)
    @assistant.append_page(page2)
    @assistant.append_page(page3)

    assert_equal(-1, @assistant.current_page)
    @assistant.next_page
    assert_equal([-1, next_page],
                 [page, @assistant.current_page])
  end
end

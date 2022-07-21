# Copyright (C) 2008-2022  Ruby-GNOME Project Team
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

class TestGtkAboutDialog < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @dialog = Gtk::AboutDialog.new
  end

  def test_program_name
    assert_equal("gtk4-test", @dialog.program_name)
    @dialog.program_name = "my-program"
    assert_equal("my-program", @dialog.program_name)
  end
end

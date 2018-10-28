# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

class TestNode < Test::Unit::TestCase
  include GeglTestHelper

  test "#process" do
    graph = Gegl::Node.new

    load = graph.create_child("gegl:load")
    load[:path] = fixture_path("gnome-logo-icon.png")

    save = graph.create_child("gegl:save")
    output = Tempfile.new(["node-save", ".jpeg"])
    save[:path] = output.path
    load << save
    save.process

    output.open
    assert_equal("\xFF\xD8\xFF\xE0\x00\x10JFIF".b,
                 output.read(10))
  end
end

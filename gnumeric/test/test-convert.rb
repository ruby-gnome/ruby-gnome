# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class ConvertTest < Test::Unit::TestCase
  include GnumericTestUtils

  test ".gnumeric -> .csv" do
    # TODO: Improve API
    input_path = fixture_file("hello.gnumeric")
    output = Tempfile.new(["test-gnumeric", ".csv"])
    command_context = Gnm::CmdContextStderr.new
    Gnm.plugins_init(command_context)
    io_context = GOffice::IOContext.new(command_context)
    view = Gnm::WorkbookView.new("file://#{input_path}",
                                 nil,
                                 io_context,
                                 "utf-8")
    saver = GOffice::FileSaver.for_file_name(output.path)
    Gnm.wb_view_save_as(view, saver, "file://#{output.path}", command_context)
    assert_equal("hello\n", File.read(output.path))
  end
end

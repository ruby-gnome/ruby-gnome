# Copyright (C) 2014-2023  Ruby-GNOME Project Team
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

class TestTerminal < Test::Unit::TestCase
  include VteTestUtils

  def setup
    @terminal = Vte::Terminal.new
  end

  def test_font
    font = Pango::FontDescription.new("Monospace 16")
    @terminal.font = font
    assert_equal(font, @terminal.font)
  end

  sub_test_case "#spawn" do
    setup do
      @loop = GLib::MainLoop.new
      @wait_child_exited = false
      @child_exit_callback_id = @terminal.signal_connect("child-exited") do
        @loop.quit
      end

      if Vte::Version.or_later?(0, 64)
        omit("VTE 0.64 or later doesn't work in Docker by default")
        # Fedora Rawhide test in Docker reports:
        #   Failed to fdwalk: Operation not permitted
        # not NotFound.
        #
        # The following discussions may be related:
        #   https://github.com/mviereck/x11docker/issues/346
        #   https://github.com/containers/podman/issues/10130
      end
    end

    teardown do
      unless @wait_child_exited
        GLib::Idle.add do
          @loop.quit
          GLib::Source::REMOVE
        end
      end
      @loop.run
      @terminal.signal_handler_disconnect(@child_exit_callback_id)
    end

    test "success" do
      pid = @terminal.spawn(:argv => ["echo"])
      @wait_child_exited = true
      assert do
        pid > 0
      end
    end

    test "failure" do
      assert_raise(Gio::IOError::NotFound) do
        @terminal.spawn(:argv => ["/bin/nonexistent"])
        @wait_child_exited = true
      end
    end
  end
end

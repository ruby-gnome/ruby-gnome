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

class TestTerminalSignals < Test::Unit::TestCase
  def setup
    @terminal = Vte::Terminal.new
  end

  def test_beep_signal
    called = false
    @terminal.signal_connect("beep") do
      called = true
    end
    @terminal.signal_emit("beep")
    assert_true(called)
  end

  def test_copy_clipboard_signal
    called = false
    @terminal.signal_connect("copy-clipboard") do
      called = true
    end
    @terminal.signal_emit("copy-clipboard")
    assert_true(called)
  end

  def test_paste_clipboard_signal
    called = false
    @terminal.signal_connect("paste-clipboard") do
      called = true
    end
    @terminal.signal_emit("paste-clipboard")
    assert_true(called)
  end
end

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

class TestTerminal < Test::Unit::TestCase
  def setup
    @terminal = Vte::Terminal.new
  end

  def test_initialize
    assert_kind_of(Vte::Terminal, @terminal)
  end

  def test_allow_bold
    @terminal.allow_bold = false
    assert_false(@terminal.allow_bold?)
  end

  def test_audible_bell
    @terminal.audible_bell = false
    assert_false(@terminal.audible_bell?)
  end

  def test_background_image_file
    image_name = "image_file"
    @terminal.background_image_file = image_name
    assert_equal(image_name, @terminal.background_image_file)
  end

  def test_emulation
    terminal_type = "vt100"
    @terminal.emulation = terminal_type
    assert_equal(terminal_type, @terminal.emulation)
  end

  def test_encoding
    encoding = "UTF-16"
    @terminal.encoding = encoding
    assert_equal(encoding, @terminal.encoding)
  end

  def test_font_desc
    font = Pango::FontDescription.new("Monospace 15")
    @terminal.font_desc = font
    assert_equal(font, @terminal.font_desc)
  end

  def test_pointer_autohide
    hide = true
    @terminal.pointer_autohide = hide
    assert_true(@terminal.pointer_autohide?)
  end

  def test_scroll_on_keystroke
    scroll = true
    @terminal.scroll_on_keystroke = scroll
    assert_true(@terminal.scroll_on_keystroke?)
  end

  def test_scroll_on_output
    scroll = false
    @terminal.scroll_on_output = scroll
    assert_false(@terminal.scroll_on_output?)
  end

  def test_scrollback_lines
    lines = 1024
    @terminal.scrollback_lines = lines
    assert_equal(lines, @terminal.scrollback_lines)
  end

  def test_visible_bell
    visible = true
    @terminal.visible_bell = visible
    assert_true(@terminal.visible_bell?)
  end

  def test_set_font
    font = Pango::FontDescription.new("Monospace 16")
    @terminal.font = font
    assert_equal(font, @terminal.font)
  end

  def test_fork_command_full
    flags = 0
    assert_nothing_raised do
      @terminal.fork_command_full(flags, nil, nil)
    end
  end
end

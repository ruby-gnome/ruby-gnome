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

class TestContext < Test::Unit::TestCase
  include PangoTestUtils
  require "gtk3"

  def setup
    @text = "this is a test"
    @label = Gtk::Label.new(@text)
    @layout = @label.layout
    @context = @layout.context
    @window = Gtk::Window.new
    @window.signal_connect("destroy") do
        Gtk.main_quit
    end
    @window.add(@label)
  end

  def test_set_font_map
    font_map = Pango::CairoFontMap.default
    @context.font_map = font_map
    assert_equal(font_map, @context.font_map)
  end

  def test_set_font_description
    description = Pango::FontDescription.new("monospace")
    description.size = 10 * Pango::SCALE
    @context.font_description = description
    assert_equal("monospace 10", @context.font_description.to_s)
  end

  def test_set_language
    @context.language = Pango::Language.new("fr")
    assert_equal("fr", @context.language.to_s)
  end

  def test_set_base_dir
    @context.base_dir = Pango::Context::DIRECTION_LTR
    assert_equal(Pango::Context::DIRECTION_LTR, @context.base_dir)
  end

  def test_set_gravity
    @context.gravity_hint = Pango::Gravity::HINT_STRONG
    assert_equal(Pango::Gravity::HINT_STRONG, @context.gravity_hint)
  end

  def test_set_matrix
    matrix = Pango::Matrix.new(0.1, 0.2, 0.3, 0.4, 0.5, 0.6)
    @context.matrix = matrix
    assert_equal(0.1, @context.matrix.xx)
    assert_equal(0.2, @context.matrix.xy)
    assert_equal(0.3, @context.matrix.yx)
    assert_equal(0.4, @context.matrix.yy)
    assert_equal(0.5, @context.matrix.x0)
    assert_equal(0.6, @context.matrix.y0)
  end

  def test_load_font
    description = Pango::FontDescription.new("monospace")
    description.size = 10 * Pango::SCALE
    font = @context.load_font(description)
    # TODO don't work
    # assert_equal("monospace 10", font.describe.to_s)
  end

  def test_load_fontset
    description = Pango::FontDescription.new("monospace")
    description.size = 10 * Pango::SCALE
    language = Pango::Language.new("fr")
    font_set = @context.load_fontset(description, language)
    # TODO don't work
    # assert_equal("monospace 10", font_set.name)
  end

  def test_get_metrics
    description = Pango::FontDescription.new("monospace")
    description.size = 10 * Pango::SCALE
    language = Pango::Language.new("fr")
    metrics = @context.get_metrics(description, language)
    # TODO Don't know how to test is neither
  end

  def test_families
    @context.families
    # TODO don't know how to test
  end

  def test_font_options
    options = Cairo::FontOptions.new
    @context.font_options = options
    assert(options.eql?(@context.font_options))
  end

  def test_resolution
    dpi = 72
    @context.resolution = dpi
    assert_equal(dpi, @context.resolution)
  end
end

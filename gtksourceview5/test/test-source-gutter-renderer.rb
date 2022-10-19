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

class TestGutterRendererText < Test::Unit::TestCase
  def setup
    @renderer = GtkSource::GutterRendererText.new
  end

  def test_markup
    markup_text = "ruby"
    @renderer.markup = markup_text
    assert_equal(markup_text, @renderer.markup)
  end

  def test_text
    render_text = "gtksourceview"
    @renderer.text = render_text
    assert_equal(render_text, @renderer.text)
  end
end

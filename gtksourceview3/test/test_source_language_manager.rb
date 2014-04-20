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

class TestLanguageManager < Test::Unit::TestCase
  def setup
    @lang = GtkSource::LanguageManager.new
    @view = GtkSource::View.new
  end

  def test_language
    language = @lang.get_language("ruby")
    @view.buffer.language = language
    assert_equal(language, @view.buffer.language)

    highlight = true
    @view.buffer.highlight_syntax = highlight
    assert_true(@view.buffer.highlight_syntax?)

    @view.buffer.highlight_matching_brackets = highlight
    assert_true(@view.buffer.highlight_matching_brackets?)
  end

  def test_language_ids
    ids = @lang.language_ids
    assert_include(ids, "ruby")
  end

  def test_search_path
    custom_path = "/path/to/search"
    @lang.search_path = custom_path
    path = @lang.search_path
    assert_equal([custom_path], @lang.search_path)
  end
end

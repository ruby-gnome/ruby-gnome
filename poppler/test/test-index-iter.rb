# Copyright (C) 2017-2021  Ruby-GNOME Project Team
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

class TestIndexIter < Test::Unit::TestCase
  include PopplerTestUtils

  def test_title
    document = Poppler::Document.new(outline_pdf)
    assert_equal([
                   [
                     "Heading1",
                     [
                       [
                         "Heading2",
                         [
                           [
                             "Heading3",
                           ],
                         ],
                       ],
                     ],
                   ],
                 ],
                 collect_titles(document.index_iter))
  end

  private
  def collect_titles(iter)
    iter.collect do
      titles = [iter.action.title]
      child_iter = iter.child
      if child_iter
        titles << collect_titles(child_iter)
      end
      titles
    end
  end
end

# Copyright (C) 2017-2019  Ruby-GNOME2 Project Team
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

class TestAttrList < Test::Unit::TestCase
  include PangoTestUtils

  def setup
    @attrs = Pango::AttrList.new
  end

  def test_insert
    attribute = Pango::AttrLanguage.new(Pango::Language.default)
    @attrs.insert(attribute)
    assert_equal([
                   [attribute.value.to_s],
                   [],
                 ],
                 @attrs.collect {|iter| iter.attrs.collect {|a| a.value.to_s}})
  end

  def test_change
    only_gi_version(1, 60)
    first_attribute = Pango::AttrLanguage.new(Pango::Language.new("ja-jp"))
    @attrs.insert(first_attribute)
    second_attribute = Pango::AttrLanguage.new(Pango::Language.new("en-us"))
    @attrs.change(second_attribute)
    assert_equal([
                   [second_attribute.value.to_s],
                   [],
                 ],
                 @attrs.collect {|iter| iter.attrs.collect {|a| a.value.to_s}})
  end
end

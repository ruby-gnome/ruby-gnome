# Copyright (C) 2013-2018  Ruby-GNOME2 Project Team
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

class TestPangoMatrix < Test::Unit::TestCase
  include PangoTestUtils

  def test_new
    matrix = Pango::Matrix.new
    assert_equal([
                   1.0, 0.0,
                   0.0, 1.0,
                   0.0,
                   0.0,
                 ],
                 matrix.to_a)
  end

  def test_translate
    matrix = Pango::Matrix.new
    new_matrix = matrix.translate(10, 20)
    assert_equal([
                   1.0, 0.0,
                   0.0, 1.0,
                   10.0,
                   20.0,
                 ],
                 new_matrix.to_a)
    assert_equal(Pango::Matrix.new.to_a,
                 matrix.to_a)
  end

  def test_translate!
    matrix = Pango::Matrix.new
    matrix.translate!(10, 20)
    assert_equal([
                   1.0, 0.0,
                   0.0, 1.0,
                   10.0,
                   20.0,
                 ],
                 matrix.to_a)
  end

  def test_scale
    matrix = Pango::Matrix.new
    new_matrix = matrix.scale(3, 5)
    assert_equal([
                   3.0, 0.0,
                   0.0, 5.0,
                   0.0,
                   0.0,
                 ],
                 new_matrix.to_a)
    assert_equal(Pango::Matrix.new.to_a,
                 matrix.to_a)
  end

  def test_scale!
    matrix = Pango::Matrix.new
    matrix.scale!(3, 5)
    assert_equal([
                   3.0, 0.0,
                   0.0, 5.0,
                   0.0,
                   0.0,
                 ],
                 matrix.to_a)
  end

  def test_rotate
    matrix = Pango::Matrix.new
    new_matrix = matrix.rotate(60)
    sin = Math.sin(60 * Math::PI / 180)
    cos = Math.cos(60 * Math::PI / 180)
    assert_equal([
                   cos, sin,
                   -sin, cos,
                   0.0,
                   0.0,
                 ],
                 new_matrix.to_a)
    assert_equal(Pango::Matrix.new.to_a,
                 matrix.to_a)
  end

  def test_rotate!
    matrix = Pango::Matrix.new
    matrix.rotate!(60)
    sin = Math.sin(60 * Math::PI / 180)
    cos = Math.cos(60 * Math::PI / 180)
    n_digits = 10
    assert_equal([
                   cos, sin,
                   -sin, cos,
                   0.0,
                   0.0,
                 ].collect {|number| number.round(n_digits)},
                 matrix.to_a.collect {|number| number.round(n_digits)})
  end

  def test_concat
    matrix = Pango::Matrix.new
    new_matrix = matrix.concat(Pango::Matrix.new.scale(3, 5))
    assert_equal([
                   3.0, 0.0,
                   0.0, 5.0,
                   0.0,
                   0.0,
                 ],
                 new_matrix.to_a)
    assert_equal(Pango::Matrix.new.to_a,
                 matrix.to_a)
  end

  def test_plus
    matrix = Pango::Matrix.new
    new_matrix = matrix + Pango::Matrix.new.scale(3, 5)
    assert_equal([
                   3.0, 0.0,
                   0.0, 5.0,
                   0.0,
                   0.0,
                 ],
                 new_matrix.to_a)
    assert_equal(Pango::Matrix.new.to_a,
                 matrix.to_a)
  end

  def test_concat!
    matrix = Pango::Matrix.new
    matrix.concat!(Pango::Matrix.new.scale(3, 5))
    assert_equal([
                   3.0, 0.0,
                   0.0, 5.0,
                   0.0,
                   0.0,
                 ],
                 matrix.to_a)
  end
end

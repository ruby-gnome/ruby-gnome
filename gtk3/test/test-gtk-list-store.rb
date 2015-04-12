# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301  USA

class TestGtkListStore < Test::Unit::TestCase
  include GtkTestUtils

  ID = 0
  NAME = 1

  COLUMNS = {
    ID => Integer,
    NAME => String,
  }

  def setup
    columns = COLUMNS.keys.sort.collect do |key|
      COLUMNS[key]
    end
    @store = Gtk::ListStore.new(*columns)
  end

  sub_test_case(".new") do
    test "no columns" do
      assert_raise(ArgumentError.new("No column type is specified")) do
        Gtk::ListStore.new
      end
    end
  end

  def test_set_values
    iter = @store.append
    assert_nothing_raised do
      @store.set_values(iter, [0, '1'])
    end
    assert_equal([0, '1'], [iter[0], iter[1]])

    iter = @store.append
    assert_nothing_raised do
      @store.set_values(iter, [2, '3'])
    end
    assert_equal([2, '3'], [iter[0], iter[1]])

    assert_nothing_raised do
      @store.set_values(iter, [])
    end
    assert_equal([2, '3'], [iter[0], iter[1]])
  end

  def test_set_values_with_hash
    iter = @store.append
    assert_nothing_raised do
      @store.set_values(iter, {ID => 0, NAME => 'me'})
    end
    assert_equal([0, 'me'], [iter[ID], iter[NAME]])

    iter = @store.append
    assert_nothing_raised do
      @store.set_values(iter, {NAME => 'you', ID => 2})
    end
    assert_equal([2, 'you'], [iter[ID], iter[NAME]])

    assert_nothing_raised do
      @store.set_values(iter, {NAME => "she"})
    end
    assert_equal([2, 'she'], [iter[ID], iter[NAME]])

    assert_nothing_raised do
      @store.set_values(iter, {})
    end
    assert_equal([2, 'she'], [iter[ID], iter[NAME]])
  end

  def test_iter_gc
    GC.start
    n_iterators = count_objects(Gtk::TreeIter)
    50.times do |i|
      iter = @store.append
      iter[ID] = i
      iter[NAME] = i.to_s
    end
    100.times do
      @store.iter_first
    end
    iter = @store.iter_first
    while @store.remove(iter); end
    assert_equal(0, @store.to_enum(:each).to_a.size)
    GC.start
    assert_equal(n_iterators + 1, count_objects(Gtk::TreeIter))
  end

  private
  def count_objects(klass)
    n_objects = ObjectSpace.each_object(Gtk::TreeIter) do
      # do nothing
    end
    n_objects
  end
end

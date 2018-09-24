# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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

  sub_test_case("#set_values") do
    test "Array" do
      iter = @store.append
      @store.set_values(iter, [0, '1'])
      assert_equal([0, '1'], [iter[0], iter[1]])

      iter = @store.append
      @store.set_values(iter, [2, '3'])
      assert_equal([2, '3'], [iter[0], iter[1]])

      @store.set_values(iter, [])
      assert_equal([2, '3'], [iter[0], iter[1]])
    end

    test "Hash" do
      iter = @store.append
      @store.set_values(iter, {ID => 0, NAME => 'me'})
      assert_equal([0, 'me'], [iter[ID], iter[NAME]])

      iter = @store.append
      @store.set_values(iter, {NAME => 'you', ID => 2})
      assert_equal([2, 'you'], [iter[ID], iter[NAME]])

      @store.set_values(iter, {NAME => "she"})
      assert_equal([2, 'she'], [iter[ID], iter[NAME]])

      @store.set_values(iter, {})
      assert_equal([2, 'she'], [iter[ID], iter[NAME]])
    end
  end

  sub_test_case("Add data") do
    test "#append" do
      iter = @store.append
      @store.set_values(iter, [0, '1'])
      assert_equal("0", iter.path.to_s)

      iter = @store.append
      @store.set_values(iter, [2, '3'])

      assert_equal("1", iter.path.to_s)
      assert_equal(0, @store.iter_first[0])
      assert_equal(2, @store.get_iter("1")[0])
    end

    test "#prepend" do
      iter = @store.append
      @store.set_values(iter, [0, '1'])
      assert_equal("0", iter.path.to_s)

      iter = @store.prepend
      @store.set_values(iter, [2, '3'])

      assert_equal("0", iter.path.to_s)
      assert_equal(2, @store.iter_first[0])
      assert_equal(0, @store.get_iter("1")[0])
    end

    sub_test_case "#insert" do
      test "no values" do
        iter = @store.append
        @store.set_values(iter, [0, '1'])
        assert_equal("0", iter.path.to_s)

        iter = @store.append
        @store.set_values(iter, [2, '3'])
        assert_equal("1", iter.path.to_s)

        iter = @store.insert(1)
        @store.set_values(iter, [4, '5'])

        assert_equal(0, @store.get_iter("0")[0])
        assert_equal(4, @store.get_iter("1")[0])
        assert_equal(2, @store.get_iter("2")[0])
      end

      test "values" do
        iter = @store.append
        @store.set_values(iter, [0, '1'])
        assert_equal("0", iter.path.to_s)

        iter = @store.append
        @store.set_values(iter, [2, '3'])
        assert_equal("1", iter.path.to_s)

        iter = @store.insert(1, [4, '5'])

        assert_equal(0, @store.get_iter("0")[0])
        assert_equal(4, @store.get_iter("1")[0])
        assert_equal(2, @store.get_iter("2")[0])
      end
    end

    test "#insert_before" do
      iter = @store.append
      @store.set_values(iter, [0, '1'])
      assert_equal("0", iter.path.to_s)

      iter = @store.append
      @store.set_values(iter, [2, '3'])
      assert_equal("1", iter.path.to_s)

      sibling = @store.get_iter("1")
      iter = @store.insert_before(sibling)
      @store.set_values(iter, [4, '5'])

      assert_equal(0, @store.get_iter("0")[0])
      assert_equal(4, @store.get_iter("1")[0])
      assert_equal(2, @store.get_iter("2")[0])
    end

    test "#insert_after" do
      iter = @store.append
      @store.set_values(iter, [0, '1'])
      assert_equal("0", iter.path.to_s)

      iter = @store.append
      @store.set_values(iter, [2, '3'])
      assert_equal("1", iter.path.to_s)

      sibling = @store.get_iter("0")
      iter = @store.insert_after(sibling)
      @store.set_values(iter, [4, '5'])

      assert_equal(0, @store.get_iter("0")[0])
      assert_equal(4, @store.get_iter("1")[0])
      assert_equal(2, @store.get_iter("2")[0])
    end
  end

  test "#each" do
    first_iter = @store.append
    first_iter[ID] = 0
    first_iter[NAME] = "Hello"
    second_iter = @store.append
    second_iter[ID] = 1
    second_iter[NAME] = "World"

    normalized_data = []
    @store.each do |model, path, iter|
      normalized_data << [model, path.to_s, iter.class]
    end
    assert_equal([
                   [@store, first_iter.path.to_s, first_iter.class],
                   [@store, second_iter.path.to_s, second_iter.class],
                 ],
                 normalized_data)
  end

  sub_test_case "#set_column_types" do
    class SubListStore < Gtk::ListStore
      type_register
    end

    def setup
      @store = SubListStore.new
    end

    test "Array" do
      @store.set_column_types([String, Integer])
      iter = @store.append
      iter[0] = "string"
      iter[1] = 29
      assert_equal(["string", 29],
                   [iter[0], iter[1]])
    end

    test "String, Integer" do
      @store.set_column_types(String, Integer)
      iter = @store.append
      iter[0] = "string"
      iter[1] = 29
      assert_equal(["string", 29],
                   [iter[0], iter[1]])
    end
  end

  sub_test_case "GC" do
    test "iter" do
      GC.start
      n_iterators = count_objects(Gtk::TreeIter)
      n_items = 50
      n_iterators_created = 100
      n_iterators_for_remove = 1

      n_items.times do |i|
        iter = @store.append
        iter[ID] = i
        iter[NAME] = i.to_s
      end
      n_iterators_created.times do
        @store.iter_first
      end
      iter = @store.iter_first
      while @store.remove(iter); end
      assert_equal(0, @store.to_enum(:each).to_a.size)

      GC.start
      assert do
        count_objects(Gtk::TreeIter) <
          (n_iterators + n_items + n_iterators_created + n_iterators_for_remove)
      end
    end

    private
    def count_objects(klass)
      n_objects = ObjectSpace.each_object(Gtk::TreeIter) do
        # do nothing
      end
      n_objects
    end
  end
end

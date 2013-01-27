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

  def test_set_values
    only_gtk_version(2, 12)

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
    only_gtk_version(2, 12)

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
    iter = nil
    assert_equal(0, @store.to_enum(:each).to_a.size)
    GC.start
    assert_equal(n_iterators, count_objects(Gtk::TreeIter))
  end

  private
  def count_objects(klass)
    n_objects = ObjectSpace.each_object(Gtk::TreeIter) do
      # do nothing
    end
    n_objects
  end
end


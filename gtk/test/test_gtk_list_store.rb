class TestGtkListStore < Test::Unit::TestCase
  include GtkTestUtils

  def test_set_values
    only_gtk_version(2, 12)

    store = Gtk::ListStore.new(Integer, String)
    iter = store.append
    assert_nothing_raised do
      store.set_values(iter, [0, '1'])
    end
    assert_equal([0, '1'], [iter[0], iter[1]])

    iter = store.append
    assert_nothing_raised do
      store.set_values(iter, [2, '3'])
    end
    assert_equal([2, '3'], [iter[0], iter[1]])

    assert_nothing_raised do
      store.set_values(iter, [])
    end
    assert_equal([2, '3'], [iter[0], iter[1]])
  end
end

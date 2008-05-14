class TestGtkListStore < Test::Unit::TestCase
  include GtkTestUtils

  def test_set_values
    only_gtk_version(2, 12)

    store = Gtk::ListStore.new(Integer, String)
    itr = store.append
    assert_nothing_raised do
      store.set_values(itr, [0, '1'])
    end
    assert_equal(0, itr[0])
    assert_equal('1', itr[1])

    itr = store.append
    assert_nothing_raised do
      store.set_values(itr, [2, '3'])
    end

    assert_equal(2, itr[0])
    assert_equal('3', itr[1])

    assert_nothing_raised do
      store.set_values(itr, [])
    end
  end
end

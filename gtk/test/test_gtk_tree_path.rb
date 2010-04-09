class TestGtkTreePath < Test::Unit::TestCase
  include GtkTestUtils

  def test_compare
    assert_equal(-1, tree_path("0") <=> tree_path("1"))
    assert_equal(0, tree_path("0") <=> tree_path("0"))
    assert_equal(1, tree_path("1") <=> tree_path("0"))
  end

  private
  def tree_path(path)
    Gtk::TreePath.new(path)
  end
end

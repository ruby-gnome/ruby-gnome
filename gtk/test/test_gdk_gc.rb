class TestGdkGC < Test::Unit::TestCase
  include GtkTestUtils

  def test_constant
    assert_const_defined(Gdk, :GC)
  end
end

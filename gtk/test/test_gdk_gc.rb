class TestGdkGC < Test::Unit::TestCase
  include GtkTestUtils

  def test_constant
    assert_nothing_raised do
      foo = Gdk::GC::LINE_ON_OFF_DASH
    end
  end
end

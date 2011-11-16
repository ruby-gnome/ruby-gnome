class TestGdkKeymap < Test::Unit::TestCase
  include GtkTestUtils

  def test_have_bidi_layouts?
    only_gtk_version(2, 12, 0)
    assert_boolean(Gdk::Keymap.default.have_bidi_layouts?)
  end
end

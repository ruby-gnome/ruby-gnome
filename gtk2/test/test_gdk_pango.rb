class TestGdkPango < Test::Unit::TestCase
  include GtkTestUtils

  def test_emboss_color_attribute
    only_gtk_version(2, 12, 0)
    color = Gdk::PangoAttrEmbossColor.new(Gdk::Color.parse("red"))
    assert_equal([65535, 0, 0], color.value.to_a)
  end
end

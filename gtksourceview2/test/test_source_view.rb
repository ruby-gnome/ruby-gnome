# -*- coding: utf-8 -*-

class TestSourceView < Test::Unit::TestCase
  def test_mark_category_background
    view = Gtk::SourceView.new

    red_rgb = [65535, 0, 0]
    red_color = Gdk::Color.parse("red")

    assert_nil(view.get_mark_category_background("new-category"))

    view.set_mark_category_background("new-category", red_color)
    assert_equal(red_rgb, view.get_mark_category_background("new-category").to_a)

    view.set_mark_category_background("new-category", nil)
    assert_nil(view.get_mark_category_background("new-category"))
  end
end

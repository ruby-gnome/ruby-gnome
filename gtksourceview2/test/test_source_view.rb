# -*- coding: utf-8 -*-

class TestSourceView < Test::Unit::TestCase
  def test_mark_category_background
    view = Gtk::SourceView.new

    red_rgb = [65535, 0, 0]
    red_color = Gdk::Color.parse("red")
    assert_not_equal(red_rgb, view.get_mark_category_background("XXX").to_a)
    view.set_mark_category_background("XXX", red_color)
    assert_equal(red_rgb, view.get_mark_category_background("XXX").to_a)
  end
end

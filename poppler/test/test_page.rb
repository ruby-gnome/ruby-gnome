class TestPage < Test::Unit::TestCase
  def test_get_image
    document = Poppler::Document.new(image_pdf)
    page, mapping = find_first_image_mapping(document)
    if later_version?(0, 7, 2)
      assert_kind_of(Cairo::ImageSurface, page.get_image(mapping.image_id))
      assert_kind_of(Cairo::ImageSurface, mapping.image)
    else
      assert_kind_of(Gdk::Pixbuf, mapping.image)
    end
  end

  def test_render_to_pixbuf
    document = Poppler::Document.new(image_pdf)
    page = document[0]
    width, height = page.size
    pixbuf = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8,
                             width / 2, height / 2)
    pixbuf.fill!(0)
    assert_equal("\0" * 10, pixbuf.pixels[0, 10])
    page.render(0, 0, width, height, 0.5, 0, pixbuf)
    assert_not_equal("\0" * 10, pixbuf.pixels[0, 10])
  end

  private
  def find_first_image_mapping(document)
    document.each do |page|
      page.image_mapping.each do |mapping|
        return [page, mapping]
      end
    end
    nil
  end
end

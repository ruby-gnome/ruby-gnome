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

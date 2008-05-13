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

  def test_render_to_pixbuf_for_printing
    only_poppler_version(0, 7, 2)

    document = Poppler::Document.new(image_pdf)
    page = document[0]
    width, height = page.size
    pixbuf = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8,
                             width / 2, height / 2)
    pixbuf.fill!(0)
    assert_equal("\0" * 10, pixbuf.pixels[0, 10])
    page.render_for_printing(0, 0, width, height, 0.5, 0, pixbuf)
    assert_not_equal("\0" * 10, pixbuf.pixels[0, 10])
  end

  def test_thumbnail_pixbuf
    omit("We doesn't have a PDF that has a thumbnail...")
    document = Poppler::Document.new(thumbnail_pdf)
    page = document[0]
    assert_kind_of(Gdk::Pixbuf, page.thumbnail_pixbuf)
  end

  def test_selection_region
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    rectangle = Poppler::Rectangle.new(0, 0, *page.size)
    region = page.get_selection_region(0.5, :word, rectangle)
    if later_version?(0, 7, 2)
      assert_kind_of(Poppler::Rectangle, region[0])
    else
      assert_kind_of(Gdk::Region, region)
    end
  end

  def test_annotation_mapping
    only_poppler_version(0, 7, 2)
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    assert_equal([Poppler::AnnotationMapping],
                 page.annotation_mapping.collect {|mapping| mapping.class}.uniq)
    mapping = page.annotation_mapping[0]
    assert_kind_of(Poppler::Rectangle, mapping.area)
    assert_kind_of(Poppler::Annotation, mapping.annotation)
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

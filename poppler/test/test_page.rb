class TestPage < Test::Unit::TestCase
  def test_get_image
    omit("We don't have PDF that has image...")
    document = Poppler::Document.new(image_pdf)
    page, mapping = find_first_image_mapping(document)
    assert_kind_of(Cairo::ImageSurface, page.get_image(mapping.image_id))
    assert_kind_of(Cairo::ImageSurface, mapping.image)
  end

  def test_selection_region
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    rectangle = Poppler::Rectangle.new(0, 0, *page.size)
    region = page.get_selection_region(0.5, :word, rectangle)
    assert_kind_of(Poppler::Rectangle, region[0])
  end

  def test_annotation_mapping
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    assert_equal([Poppler::AnnotationMapping],
                 page.annotation_mapping.collect {|mapping| mapping.class}.uniq)
    mapping = page.annotation_mapping[0]
    assert_kind_of(Poppler::Rectangle, mapping.area)
    assert_kind_of(Poppler::Annotation, mapping.annotation)
  end

  def test_text_layout
    only_poppler_version(0, 16, 0)
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    layout = page.text_layout
    assert_equal([60, 31, 79, 60],
                 layout[0].to_a.collect(&:round))
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

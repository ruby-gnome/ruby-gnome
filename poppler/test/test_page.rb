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

  def test_render
    document = Poppler::Document.new(image_pdf)
    page = document[0]
    png = StringIO.new
    png.set_encoding("ASCII-8BIT")
    Cairo::ImageSurface.new(:argb32, *page.size) do |surface|
      Cairo::Context.new(surface) do |context|
        page.render(context)
      end
      surface.write_to_png(png)
    end
    File.open(image_png, "rb") do |image|
      # TODO: support image diff
      assert_equal(image.read, png.string)
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

class TestAnnotation < Test::Unit::TestCase
  def test_type
    return unless later_version?(0, 7, 2)
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    annotation = page.annotation_mapping[0].annotation
    assert_kind_of(Poppler::AnnotationType, annotation.type)
  end
end

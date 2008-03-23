class TestAnnotation < Test::Unit::TestCase
  def test_type
    return unless later_version?(0, 7, 2)
    assert_kind_of(Poppler::AnnotationType, annotation.type)
  end

  def test_contents
    return unless later_version?(0, 7, 2)
    assert_nil(annotation.contents)
  end

  def test_name
    return unless later_version?(0, 7, 2)
    assert_nil(annotation.name)
  end

  def test_modified
    return unless later_version?(0, 7, 2)
    assert_nil(annotation.modified)
  end

  def test_flags
    return unless later_version?(0, 7, 2)
    assert_kind_of(Poppler::AnnotationFlag, annotation.flags)
  end

  def test_color
    return unless later_version?(0, 7, 2)
    assert_nil(annotation.color)
  end

  private
  def annotation
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    page.annotation_mapping[0].annotation
  end
end

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

  def test_markup
    return unless later_version?(0, 7, 2)
    # We don't have a PDF that has annotation markup...
    assert_method_defined(Poppler::AnnotationMarkup, :label)
    assert_method_defined(Poppler::AnnotationMarkup, :popup_is_open?)
    assert_method_defined(Poppler::AnnotationMarkup, :opacity)
    assert_method_defined(Poppler::AnnotationMarkup, :date)
    assert_method_defined(Poppler::AnnotationMarkup, :subject)
    assert_method_defined(Poppler::AnnotationMarkup, :reply_to)
    assert_method_defined(Poppler::AnnotationMarkup, :external_data)
  end

  def test_text
    return unless later_version?(0, 7, 2)
    # We don't have a PDF that has annotation text...
    assert_method_defined(Poppler::AnnotationText, :open?)
    assert_method_defined(Poppler::AnnotationText, :icon)
    assert_method_defined(Poppler::AnnotationText, :state)
  end

  def test_free_text
    return unless later_version?(0, 7, 2)
    # We don't have a PDF that has annotation free text...
    assert_method_defined(Poppler::AnnotationFreeText, :quadding)
    assert_method_defined(Poppler::AnnotationFreeText, :callout_line)
  end

  private
  def annotation
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    page.annotation_mapping[0].annotation
  end

  def assert_method_defined(object, method)
    assert_send([object, :method_defined?, method])
  end
end

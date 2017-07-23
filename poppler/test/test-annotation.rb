class TestAnnotation < Test::Unit::TestCase
  def test_type
    only_poppler_version(0, 7, 2)
    assert_kind_of(Poppler::AnnotType, annotation)
  end

  def test_contents
    only_poppler_version(0, 7, 2)
    assert_nil(annotation.contents)
  end

  def test_name
    only_poppler_version(0, 7, 2)
    assert_nil(annotation.name)
  end

  def test_modified
    only_poppler_version(0, 7, 2)
    assert_nil(annotation.modified)
  end

  def test_flags
    only_poppler_version(0, 7, 2)
    assert_kind_of(Poppler::AnnotFlag, annotation.flags)
  end

  def test_color
    only_poppler_version(0, 7, 2)
    assert_nil(annotation.color)
  end

  def test_markup
    only_poppler_version(0, 7, 2)
    # We don't have a PDF that has annotation markup...
    assert_method_defined(Poppler::AnnotMarkup, :label)
    assert_method_defined(Poppler::AnnotMarkup, :popup_is_open?)
    assert_method_defined(Poppler::AnnotMarkup, :opacity)
    assert_method_defined(Poppler::AnnotMarkup, :date)
    assert_method_defined(Poppler::AnnotMarkup, :subject)
    assert_method_defined(Poppler::AnnotMarkup, :reply_to)
    assert_method_defined(Poppler::AnnotMarkup, :external_data)
  end

  def test_text
    only_poppler_version(0, 7, 2)
    # We don't have a PDF that has annotation text...
    assert_method_defined(Poppler::AnnotText, :open?)
    assert_method_defined(Poppler::AnnotText, :icon)
    assert_method_defined(Poppler::AnnotText, :state)
  end

  def test_free_text
    only_poppler_version(0, 7, 2)
    # We don't have a PDF that has annotation free text...
    assert_method_defined(Poppler::AnnotFreeText, :quadding)
    assert_method_defined(Poppler::AnnotFreeText, :callout_line)
  end

  def test_callout_line
    only_poppler_version(0, 7, 2)
    callout_line = Poppler::AnnotCalloutLine.new(true,
                                                 1.1, 2.2, 3.3,
                                                 4.4, 5.5, 6.6)
    assert(callout_line.multiline?)
    assert_equal(1.1, callout_line.x1)
    assert_equal(2.2, callout_line.y1)
    assert_equal(3.3, callout_line.x2)
    assert_equal(4.4, callout_line.y2)
    assert_equal(5.5, callout_line.x3)
    assert_equal(6.6, callout_line.y3)
    assert_equal([true, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6], callout_line.to_a)
  end

  private
  def annotation
    document = Poppler::Document.new(form_pdf)
    page = document[0]
    page.annot_mapping[0].annot
  end

  def assert_method_defined(object, method)
    assert do
      object.method_defined?(method)
    end
  end
end

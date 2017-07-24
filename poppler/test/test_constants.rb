class TestConstants < Test::Unit::TestCase
  def test_annotation
    assertion = :assert_const_defined
    send(assertion, Poppler, :AnnotationType)
    if assertion == :assert_const_defined
      assert_equal("3d", Poppler::AnnotationType::TYPE_3D.nick)
    end
    send(assertion, Poppler, :AnnotationFlag)
    send(assertion, Poppler, :AnnotationMarkupReplyType)
    send(assertion, Poppler, :AnnotationExternalDataType)
    if assertion == :assert_const_defined
      assert_equal("3d", Poppler::AnnotationExternalDataType::TYPE_3D.nick)
    end
    assert_not_const_defined(Poppler, :AnnotationTextIcon)
    send(assertion, Poppler, :AnnotationTextIcon)
    send(assertion, Poppler, :AnnotationTextState)
    send(assertion, Poppler, :AnnotationFreeTextQuadding)
  end

  def test_permissions
    assert_const_defined(Poppler, :Permissions)
  end
end

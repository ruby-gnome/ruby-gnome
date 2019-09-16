class TestConstants < Test::Unit::TestCase
  def test_annotation
    if later_version?(0, 7, 2)
      assertion = :assert_const_defined
    else
      assertion = :assert_not_const_defined
    end
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
    if later_version?(0, 9, 0)
      assert_not_const_defined(Poppler, :AnnotationTextIcon)
    else
      send(assertion, Poppler, :AnnotationTextIcon)
    end
    send(assertion, Poppler, :AnnotationTextState)
    send(assertion, Poppler, :AnnotationFreeTextQuadding)
  end

  def test_permissions
    assert_const_defined(Poppler, :Permissions)
  end
end

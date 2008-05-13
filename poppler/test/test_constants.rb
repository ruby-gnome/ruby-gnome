class TestConstants < Test::Unit::TestCase
  def test_annotation
    if later_version?(0, 7, 2)
      assertion = :assert_const_defined
    else
      assertion = :assert_const_not_defined
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
      assert_const_not_defined(Poppler, :AnnotationTextIcon)
    else
      assert_const_defined(Poppler, :AnnotationTextIcon)
    end
    send(assertion, Poppler, :AnnotationTextState)
    send(assertion, Poppler, :AnnotationFreeTextQuadding)
  end

  def test_permissions
    assert_const_defined(Poppler, :Permissions)
  end

  private
  def assert_const_defined(parent, name)
    assert_block("<#{parent}::#{name}> expected to be defined.") do
      parent.const_defined?(name)
    end
  end

  def assert_const_not_defined(parent, name)
    assert_block("<#{parent}::#{name}> expected to be not defined.") do
      !parent.const_defined?(name)
    end
  end
end

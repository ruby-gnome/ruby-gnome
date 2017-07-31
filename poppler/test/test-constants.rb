class TestConstants < Test::Unit::TestCase
  def test_annotation
    assert_const_defined(Poppler, :AnnotType)
    assert_equal("3d", Poppler::AnnotType::TYPE_3D.nick)
    assert_const_defined(Poppler, :AnnotFlag)
    assert_const_defined(Poppler, :AnnotMarkupReplyType)
    assert_const_defined(Poppler, :AnnotExternalDataType)
    assert_equal("3d", Poppler::AnnotExternalDataType::TYPE_3D.nick)
    assert_const_defined(Poppler, :AnnotTextState)
    assert_const_defined(Poppler, :AnnotFreeTextQuadding)
  end

  def test_permissions
    assert_const_defined(Poppler, :Permissions)
  end
end

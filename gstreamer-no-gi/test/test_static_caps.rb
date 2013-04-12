class TestStaticCaps < Test::Unit::TestCase
  def test_description
    assert_equal("ANY", file_src_caps.description)
  end

  def test_to_caps
    static_caps = file_src_caps
    assert_kind_of(Gst::StaticCaps, static_caps)
    assert_kind_of(Gst::Caps, static_caps.to_caps)
  end

  private
  def file_src_caps
    file_src = Gst::ElementFactory.find("filesrc")
    assert_not_nil(file_src)
    file_src.pad_templates[0].caps
  end
end

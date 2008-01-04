class TestCaps < Test::Unit::TestCase
  def test_size
    assert_equal(1, smpte_caps.size)
  end

  def test_get_structure
    assert_equal("video/x-raw-yuv", smpte_caps[0].name)
  end

  def test_each
    assert_equal(["video/x-raw-yuv"],
                 smpte_caps.collect {|structure| structure.name})
  end

  private
  def smpte_caps
    smpte = Gst::ElementFactory.find("smpte")
    assert_not_nil(smpte)
    smpte.pad_templates.find do |template|
      template.direction == Gst::Pad::SRC
    end.caps.to_caps
  end
end


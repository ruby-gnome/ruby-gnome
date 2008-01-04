class TestStaticPad < Test::Unit::TestCase
  def test_description
    descriptions = file_src.pad_templates.collect do |template|
      template.caps.description
    end
    assert_equal(["ANY"], descriptions)
  end

  private
  def file_src
    file_src = Gst::ElementFactory.find("filesrc")
    assert_not_nil(file_src)
    file_src
  end
end

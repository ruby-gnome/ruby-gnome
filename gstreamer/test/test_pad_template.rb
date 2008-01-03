class TestPadTemplate < Test::Unit::TestCase
  priority :must
  def test_reader
    templates = file_src.pad_templates.collect do |template|
      [template.name, template.direction, template.presence]
    end
    assert_equal([["src", Gst::Pad::SRC, Gst::Pad::ALWAYS]],
                 templates)
  end

  private
  def file_src
    file_src = Gst::ElementFactory.find("filesrc")
    assert_not_nil(file_src)
    file_src
  end
end

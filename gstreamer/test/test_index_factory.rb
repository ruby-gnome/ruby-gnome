class TestIndexFactory < Test::Unit::TestCase
  def test_description
    index = Gst::IndexFactory.find("fileindex")
    assert_not_nil(index)
    assert_equal("A index that stores entries in file", index.description)
  end
end

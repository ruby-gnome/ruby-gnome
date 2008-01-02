class TestStructure < Test::Unit::TestCase
  def test_empty_new
    assert_raise(ArgumentError) do
      Gst::Structure.new
    end
  end

  def test_new_with_name
    structure = Gst::Structure.new("anonymous")
    assert_equal("anonymous", structure.name)
  end
end

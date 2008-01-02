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

  def test_name
    structure = Gst::Structure.new("anonymous")
    assert(structure.have_name?("anonymous"))

    assert(!structure.have_name?("me"))
    structure.name = "me"
    assert(structure.have_name?("me"))
    assert(!structure.have_name?("anonymous"))
  end

  def test_accessor
    structure = Gst::Structure.new("anonymous")

    structure["string"] = "value"
    structure["integer"] = 1

    assert_nil(structure["non-existence"])
    assert_equal("value", structure["string"])
    assert_equal(1, structure["integer"])
  end
end

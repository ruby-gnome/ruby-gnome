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

  def test_delete
    structure = Gst::Structure.new("anonymous")

    structure["string"] = "value"
    structure["integer"] = 1

    assert_equal("value", structure["string"])
    structure.delete("string")
    assert_nil(structure["string"])
    assert_equal(1, structure["integer"])
  end

  def test_clear
    structure = Gst::Structure.new("anonymous")

    structure["string"] = "value"
    structure["integer"] = 1

    assert_equal("value", structure["string"])
    assert_equal(1, structure["integer"])
    structure.clear
    assert_nil(structure["string"])
    assert_nil(structure["integer"])
  end

  def test_each_break
    structure = Gst::Structure.new("anonymous")

    structure["string"] = "value"
    structure["integer"] = 1

    result = []
    structure.each do |key, value|
      result << [key, value]
      break
    end
    assert_equal([["string", "value"]], result)
  end

  def test_collect
    structure = Gst::Structure.new("anonymous")
    assert_equal([], structure.collect {|key, value| [key, value]})

    structure["string"] = "value"
    structure["integer"] = 1
    assert_equal([
                  ["string", "value" * 2],
                  ["integer", 1 * 2],
                 ],
                 structure.collect {|key, value| [key, value * 2]})

    assert_equal("value", structure["string"])
    assert_equal(1, structure["integer"])
  end

  def test_collect!
    structure = Gst::Structure.new("anonymous")
    assert_equal([], structure.collect {|key, value| [key, value]})

    structure["string"] = "value"
    structure["integer"] = 1
    structure.collect! {|key, value| value * 2}

    assert_equal("value" * 2, structure["string"])
    assert_equal(1 * 2, structure["integer"])
  end

  def test_size
    structure = Gst::Structure.new("anonymous")

    assert_equal(0, structure.size)
    assert(structure.empty?)

    structure["string"] = "value"
    structure["integer"] = 1
    assert_equal(2, structure.size)
    assert(!structure.empty?)
  end
end

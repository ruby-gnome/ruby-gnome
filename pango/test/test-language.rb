class TestLanguage < Test::Unit::TestCase
  include PangoTestUtils

  def test_default
    assert_match(/\A(?:[a-z]{2}-[a-z]{2}|(?i:c))\z/,
                 Pango::Language.default.to_s)
  end

  def test_sample_string
    if Pango::Version.or_later?(1, 56, 4)
      expected = "The wizard quickly jinxed the gnomes before they vaporized."
    else
      expected = "The quick brown fox jumps over the lazy dog."
    end
    assert_equal(expected, Pango::Language.new("en").sample_string)
  end
end

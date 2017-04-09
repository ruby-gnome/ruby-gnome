class TestLanguage < Test::Unit::TestCase
  include PangoTestUtils

  def test_default
    assert_match(/\A(?:[a-z]{2}-[a-z]{2}|(?i:c))\z/,
                 Pango::Language.default.to_s)
  end

  def test_sample_string
    assert_equal("The quick brown fox jumps over the lazy dog.",
                 Pango::Language.new("en").sample_string)
  end
end

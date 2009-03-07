class TestLanguage < Test::Unit::TestCase
  include PangoTestUtils

  def test_default
    assert_match(/\A[a-z]{2}-[a-z]{2}\z/, Pango::Language.default.to_s)
  end
end

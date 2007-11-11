class TestPluginFeature < Test::Unit::TestCase
  def test_load!
    feature = Gst::ElementFactory.find("playbin")
    assert(feature.load!)
  end

  def test_rank
    feature = Gst::ElementFactory.find("playbin")
    assert_equal(Gst::RANK_NONE, feature.rank)
    feature.rank = :primary
    assert_equal(Gst::RANK_PRIMARY, feature.rank)
  end
end

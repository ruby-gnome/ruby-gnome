class TestPluginFeature < Test::Unit::TestCase
  def test_load!
    feature = Gst::ElementFactory.find("playbin")
    assert(feature.load!)
  end
end

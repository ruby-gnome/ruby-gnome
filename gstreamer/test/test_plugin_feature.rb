class TestPluginFeature < Test::Unit::TestCase
  def test_load!
    feature = Gst::ElementFactory.find("playbin")
    assert_nothing_raised do
      feature.load!
    end
  end
end

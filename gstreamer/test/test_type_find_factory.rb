class TestTypeFindFactory < Test::Unit::TestCase
  def test_extensions
    factories = Gst::TypeFindFactory.list
    html_factory = factories.find {|f| f.name == "text/html"}
    assert_not_nil(html_factory)
    assert_equal(["htm", "html"], html_factory.extensions)
  end
end

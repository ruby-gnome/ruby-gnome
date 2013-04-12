class TestInstallPlugins < Test::Unit::TestCase
  def test_create_return
    assert_nothing_raised do
      Gst::InstallPluginsReturn.new(Gst::InstallPluginsReturn::SUCCESS)
    end
  end

  def test_return_name
    ret = Gst::InstallPluginsReturn.new(Gst::InstallPluginsReturn::SUCCESS)
    assert_equal("success", ret.name)
  end

  def test_create_context
    assert_nothing_raised do
      Gst::InstallPluginsContext.new
    end
  end
end

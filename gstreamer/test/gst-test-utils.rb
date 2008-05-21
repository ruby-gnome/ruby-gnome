module GstTestUtils
  private
  def only_gst_version(major, minor, micro)
    unless Gst.check_version?(major, minor, micro)
      omit("Require GStreamer >= #{major}.#{minor}.#{micro}")
    end
  end

  def find_element_factory(name)
    factory = Gst::ElementFactory.find(name)
    assert_not_nil(factory, "#{name}'s factory should be found.")
    factory
  end

  def create_element(element_name, name=nil)
    find_element_factory(element_name).create(name)
  end
end

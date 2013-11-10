# -*- coding: utf-8 -*-

class TestFileEnumerator < Test::Unit::TestCase
  include GioGITestUtils

  def test_responds_to_properties
    omit_not_implemented
    a = Gio::File.new_for_path('/').enumerate_children
    assert a.respond_to?(:container)
  end
end

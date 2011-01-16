# -*- coding: utf-8 -*-

class TestFileEnumerator < Test::Unit::TestCase
  def test_responds_to_properties
    a = GLib::File.new_for_path('/').enumerate_children
    assert a.respond_to?(:container)
  end
end

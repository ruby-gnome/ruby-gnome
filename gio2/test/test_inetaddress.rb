# -*- coding: utf-8 -*-

class TestInetAddress < Test::Unit::TestCase
  def test_responds_to_properties
    a = GLib::InetAddress.new_any(GLib::Socket::FAMILY_IPV4)
    assert a.respond_to?(:any?)
  end
end

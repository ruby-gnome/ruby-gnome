# -*- coding: utf-8 -*-

class TestInetAddress < Test::Unit::TestCase
  def test_responds_to_properties
    a = Gio::InetAddress.new_any(Gio::Socket::FAMILY_IPV4)
    assert a.respond_to?(:any?)
  end
end

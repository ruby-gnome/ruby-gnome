# -*- coding: utf-8 -*-

class TestInetAddress < Test::Unit::TestCase
  def test_responds_to_properties
    a = Gio::InetAddress.new(Gio::SocketFamily::IPV4)
    assert a.respond_to?(:any?)
  end
end

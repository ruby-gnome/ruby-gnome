# Copyright (C) 2014  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

class TestPollableInputStream < Test::Unit::TestCase
  include GioTestUtils::SocketClient

  def setup
    setup_socket_client
    @stream = @connection.input_stream
  end

  def teardown
    teardown_socket_client
  end

  def test_create_socket
    called = false
    source = @stream.create_source do |stream|
      called = true
      false
    end
    source.attach
    client = @server.accept
    client.write("Hello\n")
    client.flush
    GLib::Idle.add do
      client.close
      @loop.quit
    end
    @loop.run
    assert_true(called)
  end

  def test_read_nonblocking
    data = "Hello\n"
    client = @server.accept
    client.write(data)
    client.flush
    assert_equal(data, @stream.read_nonblocking)
  end
end

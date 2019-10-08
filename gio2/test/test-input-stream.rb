# Copyright (C) 2014-2019  Ruby-GNOME Project Team
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

class TestInputStream < Test::Unit::TestCase
  include GioTestUtils::SocketClient

  def setup
    setup_socket_client
    @stream = @connection.input_stream
  end

  def teardown
    teardown_socket_client
  end

  sub_test_case(".open") do
    def setup
      super
      @data = "Hello\n"
      client = @server.accept
      client.write(@data)
      client.flush
      client.close
    end

    def test_no_block
      input = Gio::BufferedInputStream.open(@stream)
      assert_equal(@data[0, 4], input.read(4))
      input.close
    end

    def test_block
      input = nil
      read_data = Gio::BufferedInputStream.open(@stream) do |i|
        input = i
        input.read(4)
      end
      assert_equal([@data[0, 4], true],
                   [read_data, input.closed?])
    end

    def test_block_closed
      input = nil
      read_data = Gio::BufferedInputStream.open(@stream) do |i|
        input = i
        data = input.read(4)
        input.close
        data
      end
      assert_equal([@data[0, 4], true],
                   [read_data, input.closed?])
    end
  end

  sub_test_case("#read") do
    def test_with_size
      data = "Hello\n"
      client = @server.accept
      client.write(data)
      client.flush
      assert_equal("Hell", @stream.read(4))
    end

    def test_without_size
      data = "Hello\n"
      client = @server.accept
      client.write(data)
      client.flush
      client.close
      assert_equal(data, @stream.read)
    end
  end

  def test_read_all
    client = @server.accept
    client.write("He")
    client.flush
    thread = Thread.new do
      sleep(0.1)
      client.write("llo")
      client.flush
    end
    assert_equal("Hell", @stream.read_all(4))
    thread.join
  end
end

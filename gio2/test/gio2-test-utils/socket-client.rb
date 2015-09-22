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

require "socket"

module GioTestUtils
  module SocketClient
    def setup_socket_client
      setup_server
      setup_client
      setup_loop
    end

    def teardown_socket_client
      teardown_loop
      teardown_client
      teardown_server
    end

    def setup_server
      @host = "127.0.0.1"
      @server = TCPServer.new(@host, 0)
      @port = @server.addr[1]
    end

    def teardown_server
      @server.close
    end

    def setup_client
      @client = Gio::SocketClient.new
      @connection = @client.connect_to_host(@host, @port)
    end

    def teardown_client
      @connection.close
    end

    def setup_loop
      @loop = GLib::MainLoop.new
    end

    def teardown_loop
    end
  end
end

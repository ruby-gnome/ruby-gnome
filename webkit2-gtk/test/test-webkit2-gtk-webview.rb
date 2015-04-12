# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestWebKit2GtkWebView < Test::Unit::TestCase
  def setup
    @view = WebKit2Gtk::WebView.new
    @http_server = WEBrick::HTTPServer.new(:Port => 0)
    @http_server.mount_proc("/") do |request, response|
      response.body = "Hello"
    end
    @http_server_thread = Thread.new do
      @http_server.start
    end
  end

  def teardown
    @http_server.shutdown
    @http_server_thread.join
  end

  def http_url
    port = @http_server[:Port]
    "http://127.0.0.1:#{port}/"
  end

  test "#load_uri" do
    loaded = false

    loop = GLib::MainLoop.new
    timeout_id = GLib::Timeout.add(1000) do
      timeout_id = nil
      loop.quit
      GLib::Source::REMOVE
    end
    @view.signal_connect("load-changed") do |view, event|
      case event
      when WebKit2Gtk::LoadEvent::FINISHED
        GLib::Source.remove(timeout_id) if timeout_id
        loaded = true
        loop.quit
      end
    end
    @view.load_uri(http_url)
    loop.run

    assert_true(loaded)
  end
end

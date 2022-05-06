# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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
  include WebKit2GtkTestUtils

  sub_test_case(".new") do
    sub_test_case("Hash form") do
      test "with context" do
        only_webkit2_gtk_version(2, 8)
        context = WebKit2Gtk::WebContext.new
        webview = WebKit2Gtk::WebView.new(context: context)
        assert_equal(context, webview.context)
      end

      test "with settings" do
        settings = WebKit2Gtk::Settings.new
        webview = WebKit2Gtk::WebView.new(settings: settings)
        assert_equal(settings, webview.settings)
      end

      test "with user content manager" do
        manager = WebKit2Gtk::UserContentManager.new
        webview = WebKit2Gtk::WebView.new(user_content_manager: manager)
        assert_equal(manager, webview.user_content_manager)
      end

      test "with related view" do
        settings = WebKit2Gtk::Settings.new
        related = WebKit2Gtk::WebView.new(settings: settings)
        webview = WebKit2Gtk::WebView.new(related_view: related)
        assert_equal(settings, webview.settings)
      end

      test "with unknown option" do
        assert_raises do
          WebKit2Gtk::WebView.new(foo: 'bar')
        end
      end
    end

    sub_test_case("legacy form") do
      test "with unknown argument" do
        assert_raises ArgumentError do
          WebKit2Gtk::WebView.new('foo')
        end
      end
    end
  end

  sub_test_case("#load_uri") do
    def almalinux8?
      return false unless File.exist?("/etc/system-release")
      File.read("/etc/system-release").start_with?("AlmaLinux release 8")
    end

    def setup
      omit("Don't know why but this is unstable...") if almalinux8?

      @view = WebKit2Gtk::WebView.new
      @http_server = WEBrick::HTTPServer.new(:Port => 0)
      @http_server.mount_proc("/") do |request, response|
        response.body = "Hello"
      end
      @http_server_thread = Thread.new do
        @http_server.start
      end
      begin
        yield
      ensure
        @http_server.shutdown
        @http_server_thread.join
      end
    end

    def http_url
      port = @http_server[:Port]
      "http://127.0.0.1:#{port}/"
    end

    test "#load_uri" do
      loaded = false

      loop = GLib::MainLoop.new
      timeout_id = GLib::Timeout.add(30000) do
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
end

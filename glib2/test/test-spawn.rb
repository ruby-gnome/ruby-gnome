# Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

require "rbconfig"

class TestGLibSpawn < Test::Unit::TestCase
  include GLibTestUtils

  def setup
  end

  def teardown
  end

  def test_async_keep_environment
    if File.exist?('/bin/true')
      assert_nothing_raised do
        GLib::Spawn.async(Dir.pwd, ['/bin/true'], nil, 0)
      end
    end
  end

  def test_async_clear_environment
    only_not_windows
    if RbConfig.respond_to?(:ruby)
      ruby = RbConfig.ruby
    else
      ruby = File.join(RbConfig::CONFIG['bindir'],
                       RbConfig::CONFIG['RUBY_INSTALL_NAME'] +
                         RbConfig::CONFIG['EXEEXT'])
    end
    test_env_name = "RUBY_GLIB2_TEST"
    ENV[test_env_name] = "yes"
    pid = GLib::Spawn.async(Dir.pwd,
                            [
                              ruby,
                              "-e",
                              "exit(!ENV.key?(#{test_env_name.dump}))",
                            ],
                            [],
                            GLib::Spawn::DO_NOT_REAP_CHILD)
    _, status = *Process.waitpid2(pid)
    assert do
      status.success?
    end
  end
end

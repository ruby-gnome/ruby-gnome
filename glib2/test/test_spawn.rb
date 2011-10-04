# -*- coding: utf-8 -*-
require 'rbconfig'

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
    if RbConfig.respond_to?(:ruby)
      ruby = RbConfig.ruby
    else
      ruby = File.join(RbConfig::CONFIG['bindir'],
                       RbConfig::CONFIG['RUBY_INSTALL_NAME'] +
                         RbConfig::CONFIG['EXEEXT'])
    end
    pid = GLib::Spawn.async(Dir.pwd, [ruby, '-e', 'exit 1 unless ENV.empty?'], [], GLib::Spawn::DO_NOT_REAP_CHILD)
    _, status = *Process.waitpid2(pid)
    assert status.success?
  end
end

#!/usr/bin/env ruby

require 'test/unit'
require 'gtk2'
require 'gconf2'

class TestDigest < Test::Unit::TestCase
  GCONF_TEST_KEY = "/extra/test/directory/key"

  @@client = GConf::Client.default

  def t(val)
    @@client[GCONF_TEST_KEY] = val
    assert_equal(val, @@client[GCONF_TEST_KEY])
  end

  def ex(val, e)
    assert_raises(e) {
      @@client[GCONF_TEST_KEY] = val
    }
  end

  def ae(val)
    ex(val, ArgumentError)
  end

  def te(val)
    ex(val, TypeError)
  end

  def test_bool
    t(true)
    t(false)
  end

  def test_int
    t(1)
    t(0)
    t(-1)
    t((2**31)-1)
    t(-(2**31)+1)
    ex(2**100, RangeError)
  end

  def test_string
    t("")
    t("string")
  end

  def test_invalid_type
    te(nil)
    te(:symbol)
    te(Hash.new)
    te(self)
  end

  def test_pair
    t([true, false])
    t(%w(foo bar))
    t([1, 2])
    t([0.5, 0.25])
  end

  def test_invalid_pair
    ae([0.5, 1])
    ae([1, 1.5])
    ae([true, "foo"])
    ae(["foo", true])
    ae([true, 0])
  end

  def test_empty_array
    ae([])
  end

  def test_array
    t([true,false,true])
    ae([true, 1, 2.0])
    ae([true,false,[true]])
    t(%w(foo bar baz))
    t([1, 2, 3])
    t([1.0, 2.0, 3.0])
    ae([1, 2.0, 3])
    ae([1.0, 2, 3.0])
  end

  def test_unset
    @@client.unset(GCONF_TEST_KEY)
    assert_nil(@@client[GCONF_TEST_KEY])
  end
end

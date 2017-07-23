class TestColor < Test::Unit::TestCase
  def test_initialize
    rose = Poppler::Color.new(65535, 0, 32639)
    assert_equal([65535, 0, 32639], [rose.red, rose.green, rose.blue])
    assert_equal([65535, 0, 32639], rose.to_a)
  end

  def test_accessor
    white = Poppler::Color.new(65535, 65535, 65535)
    red = white.dup
    red.green = 0
    red.blue = 0
    assert_equal([65535, 65535, 65535], [white.red, white.green, white.blue])
    assert_equal([65535, 0, 0], [red.red, red.green, red.blue])
  end

  def test_inspect
    color = Poppler::Color.new(0, 32767, 65535)
    rgb = '[0, 32767, 65535]'
    assert_match(/\A#<Poppler::Color:.*: #{Regexp.escape(rgb)}>\z/,
                 color.inspect)
  end
end

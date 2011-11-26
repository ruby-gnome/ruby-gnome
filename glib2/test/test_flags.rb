# -*- coding: utf-8 -*-

require 'test/unit'
require 'glib2'

class TestFlags < Test::Unit::TestCase
  {
    '<=>' => [
      [0b0000, 0.0, nil],
      [0b0000, 0b0000, 0],
      [0b0000, 0b0110, -1],
      [0b0000, 0b1111, -1],
      [0b0110, 0b0000, 1],
      [0b0110, 0b0011, nil],
      [0b0110, 0b0110, 0],
      [0b0110, 0b0111, -1],
      [0b0110, 0b1001, nil],
      [0b0110, 0b1100, nil],
      [0b0110, 0b1110, -1],
      [0b0110, 0b1111, -1],
      [0b1111, 0b0000, 1],
      [0b1111, 0b0110, 1],
      [0b1111, 0b1111, 0],
    ],
    '==' => [
      [0b0000, 0.0, nil],
      [0b0000, 0b0000, true],
      [0b0000, 0b0110, false],
      [0b0000, 0b1111, false],
      [0b0110, 0b0000, false],
      [0b0110, 0b0011, false],
      [0b0110, 0b0110, true],
      [0b0110, 0b0111, false],
      [0b0110, 0b1001, false],
      [0b0110, 0b1100, false],
      [0b0110, 0b1110, false],
      [0b0110, 0b1111, false],
      [0b1111, 0b0000, false],
      [0b1111, 0b0110, false],
      [0b1111, 0b1111, true],
    ],
    '>=' => [
      [0b0000, 0.0, nil],
      [0b0000, 0b0000, true],
      [0b0000, 0b0110, false],
      [0b0000, 0b1111, false],
      [0b0110, 0b0000, true],
      [0b0110, 0b0011, false],
      [0b0110, 0b0110, true],
      [0b0110, 0b0111, false],
      [0b0110, 0b1001, false],
      [0b0110, 0b1100, false],
      [0b0110, 0b1110, false],
      [0b0110, 0b1111, false],
      [0b1111, 0b0000, true],
      [0b1111, 0b0110, true],
      [0b1111, 0b1111, true],
    ],
    '<=' => [
      [0b0000, 0.0, nil],
      [0b0000, 0b0000, true],
      [0b0000, 0b0110, true],
      [0b0000, 0b1111, true],
      [0b0110, 0b0000, false],
      [0b0110, 0b0011, false],
      [0b0110, 0b0110, true],
      [0b0110, 0b0111, true],
      [0b0110, 0b1001, false],
      [0b0110, 0b1100, false],
      [0b0110, 0b1110, true],
      [0b0110, 0b1111, true],
      [0b1111, 0b0000, false],
      [0b1111, 0b0110, false],
      [0b1111, 0b1111, true],
    ],
    '>' => [
      [0b0000, 0.0, nil],
      [0b0000, 0b0000, false],
      [0b0000, 0b0110, false],
      [0b0000, 0b1111, false],
      [0b0110, 0b0000, true],
      [0b0110, 0b0011, false],
      [0b0110, 0b0110, false],
      [0b0110, 0b0111, false],
      [0b0110, 0b1001, false],
      [0b0110, 0b1100, false],
      [0b0110, 0b1110, false],
      [0b0110, 0b1111, false],
      [0b1111, 0b0000, true],
      [0b1111, 0b0110, true],
      [0b1111, 0b1111, false],
    ],
    '<' => [
      [0b0000, 0.0, nil],
      [0b0000, 0b0000, false],
      [0b0000, 0b0110, true],
      [0b0000, 0b1111, true],
      [0b0110, 0b0000, false],
      [0b0110, 0b0011, false],
      [0b0110, 0b0110, false],
      [0b0110, 0b0111, true],
      [0b0110, 0b1001, false],
      [0b0110, 0b1100, false],
      [0b0110, 0b1110, true],
      [0b0110, 0b1111, true],
      [0b1111, 0b0000, false],
      [0b1111, 0b0110, false],
      [0b1111, 0b1111, false],
    ],
  }.each do |operator, values|
    values.each do |a, b, expected|
      a_format = a.is_a?(Float) ? "%f" : "0b%04b"
      b_format = b.is_a?(Float) ? "%f" : "0b%04b"
      label_format = "#{a_format} %s #{b_format} == %s"
      data((label_format % [a, operator, b, expected.inspect]),
           {
             :a => a,
             :operator => operator,
             :b => b,
             :expected => expected,
           })
    end
  end
  def test_flags_compare(data)
    flags = GLib::KeyFile::Flags.new(data[:a])
    assert_equal(data[:expected],
                 flags.send(data[:operator], data[:b]))
  end
end

require 'test/unit'
require 'glib2'

class TestGLibUnicode < Test::Unit::TestCase
  def test_gunicode_type
    assert_nothing_raised do
      GLib::UNICODE_CONTROL
    end
  end

  def test_gunicode_break_type
    assert_nothing_raised do
      GLib::UNICODE_BREAK_MANDATORY
    end
  end

  def test_unichar_alnum?
    assert(GLib.unichar_alnum?(?a))
    assert(GLib.unichar_alnum?(?1))
    assert(!GLib.unichar_alnum?(?!))
  end

  def test_unichar_alpha?
    assert(GLib.unichar_alpha?(?a))
    assert(GLib.unichar_alpha?(?A))
    assert(!GLib.unichar_alpha?(?1))
  end

  def test_unichar_control?
    assert(GLib.unichar_control?(?\t))
    assert(!GLib.unichar_control?(?\h))
    assert(!GLib.unichar_control?(?a))
    assert(!GLib.unichar_control?(?1))
  end

  def test_unichar_digit?
    assert(GLib.unichar_digit?(?1))
    assert(!GLib.unichar_digit?(?a))
  end

  def test_unichar_graph?
    assert(GLib.unichar_graph?(?a))
    assert(!GLib.unichar_graph?(?\ )) # space
    assert(!GLib.unichar_graph?(?\t))
  end

  def test_unichar_lower?
    assert(GLib.unichar_lower?(?a))
    assert(!GLib.unichar_lower?(?A))
    assert(!GLib.unichar_lower?(?1))
  end

  def test_unichar_print?
    assert(GLib.unichar_print?(?a))
    assert(GLib.unichar_print?(?\ )) # space
    assert(!GLib.unichar_print?(?\t))
  end

  def test_unichar_print?
    assert(GLib.unichar_print?(?a))
    assert(GLib.unichar_print?(?\ )) # space
    assert(!GLib.unichar_print?(?\t))
  end

  def test_unichar_punct?
    assert(GLib.unichar_punct?(?,))
    assert(GLib.unichar_punct?(?.))
    assert(!GLib.unichar_punct?(?a))
    assert(!GLib.unichar_punct?(?\t))
  end

  def test_unichar_space?
    assert(GLib.unichar_space?(?\ )) # space
    assert(GLib.unichar_space?(?\t))
    assert(GLib.unichar_space?(?\r))
    assert(GLib.unichar_space?(?\n))
    assert(!GLib.unichar_space?(?a))
  end

  def test_unichar_upper?
    assert(GLib.unichar_upper?(?A))
    assert(!GLib.unichar_upper?(?a))
    assert(!GLib.unichar_upper?(?1))
  end

  def test_unichar_xdigit?
    assert(GLib.unichar_xdigit?(?1))
    assert(GLib.unichar_xdigit?(?a))
    assert(GLib.unichar_xdigit?(?A))
    assert(GLib.unichar_xdigit?(?F))
    assert(!GLib.unichar_xdigit?(?X))
  end

  def test_unichar_title?
  end

  def test_unichar_defined?
  end

  def test_unichar_wide?
    require 'uconv'
    assert(GLib.unichar_wide?(Uconv.u8tou4("あ").unpack("L*")[0]))
    assert(GLib.unichar_wide?(Uconv.u8tou4("Ａ").unpack("L*")[0]))
    assert(!GLib.unichar_wide?(?a))
  end

  def test_unichar_wide_cjk?
    require 'uconv'
    assert(GLib.unichar_wide_cjk?(Uconv.u8tou4("あ").unpack("L*")[0]))
    assert(GLib.unichar_wide_cjk?(Uconv.u8tou4("한").unpack("L*")[0]))
    assert(!GLib.unichar_wide_cjk?(?a))
  end

  def test_unichar_to_upper
    assert_equal(?A, GLib.unichar_to_upper(?a))
    assert_equal(?A, GLib.unichar_to_upper(?A))
    assert_equal(?*, GLib.unichar_to_title(?*))
  end

  def test_unichar_to_lower
    assert_equal(?a, GLib.unichar_to_lower(?A))
    assert_equal(?a, GLib.unichar_to_lower(?a))
    assert_equal(?*, GLib.unichar_to_title(?*))
  end

  def test_unichar_to_title
    assert_equal(?A, GLib.unichar_to_title(?a))
    assert_equal(?A, GLib.unichar_to_title(?A))
    assert_equal(?*, GLib.unichar_to_title(?*))
  end

  def test_unichar_digit_value
    assert_equal(0, GLib.unichar_digit_value(?0))
    assert_equal(9, GLib.unichar_digit_value(?9))
    assert_equal(-1, GLib.unichar_digit_value(?a))
  end

  def test_unichar_xdigit_value
    assert_equal(0, GLib.unichar_xdigit_value(?0))
    assert_equal(9, GLib.unichar_xdigit_value(?9))
    assert_equal(10, GLib.unichar_xdigit_value(?a))
    assert_equal(15, GLib.unichar_xdigit_value(?F))
    assert_equal(-1, GLib.unichar_xdigit_value(?g))
  end

  def test_unichar_type
    assert_equal(GLib::UNICODE_DECIMAL_NUMBER, GLib.unichar_type(?0))
    assert_equal(GLib::UNICODE_LOWERCASE_LETTER, GLib.unichar_type(?a))
    assert_equal(GLib::UNICODE_UPPERCASE_LETTER, GLib.unichar_type(?A))
  end

  def test_unichar_break_type
    assert_equal(GLib::UNICODE_BREAK_HYPHEN, GLib.unichar_break_type(?-))
    assert_equal(GLib::UNICODE_BREAK_NUMERIC, GLib.unichar_break_type(?0))
  end

  def test_unicode_canonical_ordering
    require 'uconv'
    original = [?a, 0x0308, 0x0323, ?e, 0x0304, 0x0301, 0x0323].pack("U*")
    expected = [?a, 0x0323, 0x0308, ?e, 0x0323, 0x0304, 0x0301].pack("U*")
    assert_equal(Uconv.u8tou4(expected),
                 GLib.unicode_canonical_ordering(Uconv.u8tou4(original)))
  end

  def test_unicode_canonical_decomposition
    require 'uconv'
    unichar = 0x00c1 # "A" with acute
    expected = [?A, 0x0301].pack("U*")
    assert_equal(Uconv.u8tou4(expected),
                 GLib.unicode_canonical_decomposition(unichar))

    unichar = 0x304c # "が"
    expected = [0x304B, 0x3099].pack("U*")
    assert_equal(Uconv.u8tou4(expected),
                 GLib.unicode_canonical_decomposition(unichar))
  end

  def test_unichar_get_mirror_char
    assert_equal(?\(, GLib.unichar_get_mirror_char(?\)))
    assert_equal(?\), GLib.unichar_get_mirror_char(?\())
    assert_equal(?x, GLib.unichar_get_mirror_char(?x))
  end

  def test_unichar_get_script
    require 'uconv'
    return unless (GLib::VERSION <=> [2, 14, 0]) >= 1
    assert_equal(GLib::UNICODE_SCRIPT_HIRAGANA,
                 GLib.unichar_get_script(Uconv.u8tou4("あ").unpack("L*")[0]))
  end

  def test_utf8_get_char
    assert_equal(Uconv.u8tou4("あ").unpack("L*")[0],
                 GLib.utf8_get_char("あ"))
  end

  def test_utf8_get_char_validated
    assert_equal(Uconv.u8tou4("あ").unpack("L*")[0],
                 GLib.utf8_get_char_validated("あ"))
    partial_input = "あ".unpack("c*")[0..-2].pack("c*")
    assert_equal(-2, GLib.utf8_get_char_validated(partial_input))
    invalid_input = "あ".unpack("c*")[2..-1].pack("c*")
    assert_equal(-1, GLib.utf8_get_char_validated(invalid_input))
  end

  def test_utf8_size
    assert_equal(1, GLib.utf8_size("あ"))
    assert_equal(2, GLib.utf8_size("あい"))
  end

  def test_utf8_upcase
    assert_equal("ABCあいう", GLib.utf8_upcase("aBcあいう"))
  end

  def test_utf8_downcase
    assert_equal("abcあいう", GLib.utf8_downcase("aBcあいう"))
  end

  def test_utf8_casefold
    assert_equal(GLib.utf8_casefold("AbCあいう"),
                 GLib.utf8_casefold("aBcあいう"))
  end

  def test_utf8_normalize
    original = [0x00c1].pack("U*") # A with acute

    nfd = [0x0041, 0x0301].pack("U*")
    assert_equal(nfd, GLib.utf8_normalize(original, GLib::NORMALIZE_NFD))

    nfc = [0x00c1].pack("U*")
    assert_equal(nfc, GLib.utf8_normalize(original, GLib::NORMALIZE_NFC))

    nfkd = [0x0041, 0x0301].pack("U*")
    assert_equal(nfkd, GLib.utf8_normalize(original, GLib::NORMALIZE_NFKD))

    nfkc = [0x00c1].pack("U*")
    assert_equal(nfkc, GLib.utf8_normalize(original, GLib::NORMALIZE_NFKC))
  end
end

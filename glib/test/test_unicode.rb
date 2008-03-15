require 'test/unit'
require 'glib2'

class TestGLibUnicode < Test::Unit::TestCase
  def test_gunicode_type
    assert_nothing_raised do
      GLib::Unicode::CONTROL
    end
  end

  def test_gunicode_break_type
    assert_nothing_raised do
      GLib::Unicode::BREAK_MANDATORY
    end
  end

  def test_unichar_alnum?
    assert(GLib::UniChar.alnum?(?a))
    assert(GLib::UniChar.alnum?(?1))
    assert(!GLib::UniChar.alnum?(?!))
  end

  def test_unichar_alpha?
    assert(GLib::UniChar.alpha?(?a))
    assert(GLib::UniChar.alpha?(?A))
    assert(!GLib::UniChar.alpha?(?1))
  end

  def test_unichar_cntrl?
    assert(GLib::UniChar.cntrl?(?\t))
    assert(!GLib::UniChar.cntrl?(?\h))
    assert(!GLib::UniChar.cntrl?(?a))
    assert(!GLib::UniChar.cntrl?(?1))
  end

  def test_unichar_digit?
    assert(GLib::UniChar.digit?(?1))
    assert(!GLib::UniChar.digit?(?a))
  end

  def test_unichar_graph?
    assert(GLib::UniChar.graph?(?a))
    assert(!GLib::UniChar.graph?(?\ )) # space
    assert(!GLib::UniChar.graph?(?\t))
  end

  def test_unichar_lower?
    assert(GLib::UniChar.lower?(?a))
    assert(!GLib::UniChar.lower?(?A))
    assert(!GLib::UniChar.lower?(?1))
  end

  def test_unichar_print?
    assert(GLib::UniChar.print?(?a))
    assert(GLib::UniChar.print?(?\ )) # space
    assert(!GLib::UniChar.print?(?\t))
  end

  def test_unichar_print?
    assert(GLib::UniChar.print?(?a))
    assert(GLib::UniChar.print?(?\ )) # space
    assert(!GLib::UniChar.print?(?\t))
  end

  def test_unichar_punct?
    assert(GLib::UniChar.punct?(?,))
    assert(GLib::UniChar.punct?(?.))
    assert(!GLib::UniChar.punct?(?a))
    assert(!GLib::UniChar.punct?(?\t))
  end

  def test_unichar_space?
    assert(GLib::UniChar.space?(?\ )) # space
    assert(GLib::UniChar.space?(?\t))
    assert(GLib::UniChar.space?(?\r))
    assert(GLib::UniChar.space?(?\n))
    assert(!GLib::UniChar.space?(?a))
  end

  def test_unichar_upper?
    assert(GLib::UniChar.upper?(?A))
    assert(!GLib::UniChar.upper?(?a))
    assert(!GLib::UniChar.upper?(?1))
  end

  def test_unichar_xdigit?
    assert(GLib::UniChar.xdigit?(?1))
    assert(GLib::UniChar.xdigit?(?a))
    assert(GLib::UniChar.xdigit?(?A))
    assert(GLib::UniChar.xdigit?(?F))
    assert(!GLib::UniChar.xdigit?(?X))
  end

  def test_unichar_title?
  end

  def test_unichar_defined?
  end

  def test_unichar_wide?
    require 'uconv'
    assert(GLib::UniChar.wide?(Uconv.u8tou4("あ").unpack("L*")[0]))
    assert(GLib::UniChar.wide?(Uconv.u8tou4("Ａ").unpack("L*")[0]))
    assert(!GLib::UniChar.wide?(?a))
  end

  def test_unichar_wide_cjk?
    return unless (GLib::VERSION <=> [2, 12, 0]) >= 0
    require 'uconv'
    assert(GLib::UniChar.wide_cjk?(Uconv.u8tou4("あ").unpack("L*")[0]))
    assert(GLib::UniChar.wide_cjk?(Uconv.u8tou4("한").unpack("L*")[0]))
    assert(!GLib::UniChar.wide_cjk?(?a))
  end

  def test_unichar_to_upper
    assert_equal(?A, GLib::UniChar.to_upper(?a))
    assert_equal(?A, GLib::UniChar.to_upper(?A))
    assert_equal(?*, GLib::UniChar.to_title(?*))
  end

  def test_unichar_to_lower
    assert_equal(?a, GLib::UniChar.to_lower(?A))
    assert_equal(?a, GLib::UniChar.to_lower(?a))
    assert_equal(?*, GLib::UniChar.to_title(?*))
  end

  def test_unichar_to_title
    assert_equal(?A, GLib::UniChar.to_title(?a))
    assert_equal(?A, GLib::UniChar.to_title(?A))
    assert_equal(?*, GLib::UniChar.to_title(?*))
  end

  def test_unichar_digit_value
    assert_equal(0, GLib::UniChar.digit_value(?0))
    assert_equal(9, GLib::UniChar.digit_value(?9))
    assert_equal(-1, GLib::UniChar.digit_value(?a))
  end

  def test_unichar_xdigit_value
    assert_equal(0, GLib::UniChar.xdigit_value(?0))
    assert_equal(9, GLib::UniChar.xdigit_value(?9))
    assert_equal(10, GLib::UniChar.xdigit_value(?a))
    assert_equal(15, GLib::UniChar.xdigit_value(?F))
    assert_equal(-1, GLib::UniChar.xdigit_value(?g))
  end

  def test_unichar_type
    assert_equal(GLib::Unicode::DECIMAL_NUMBER, GLib::UniChar.type(?0))
    assert_equal(GLib::Unicode::LOWERCASE_LETTER, GLib::UniChar.type(?a))
    assert_equal(GLib::Unicode::UPPERCASE_LETTER, GLib::UniChar.type(?A))
  end

  def test_unichar_break_type
    assert_equal(GLib::Unicode::BREAK_HYPHEN, GLib::UniChar.break_type(?-))
    assert_equal(GLib::Unicode::BREAK_NUMERIC, GLib::UniChar.break_type(?0))
  end

  def test_unicode_canonical_ordering
    require 'uconv'
    original = [?a, 0x0308, 0x0323, ?e, 0x0304, 0x0301, 0x0323].pack("U*")
    expected = [?a, 0x0323, 0x0308, ?e, 0x0323, 0x0304, 0x0301].pack("U*")
    assert_equal(Uconv.u8tou4(expected),
                 GLib::Unicode.canonical_ordering(Uconv.u8tou4(original)))
  end

  def test_unicode_canonical_decomposition
    require 'uconv'
    unichar = 0x00c1 # "A" with acute
    expected = [?A, 0x0301].pack("U*")
    assert_equal(Uconv.u8tou4(expected),
                 GLib::Unicode.canonical_decomposition(unichar))

    unichar = 0x304c # "が"
    expected = [0x304B, 0x3099].pack("U*")
    assert_equal(Uconv.u8tou4(expected),
                 GLib::Unicode.canonical_decomposition(unichar))
  end

  def test_unichar_get_mirror_char
    return unless (GLib::VERSION <=> [2, 4, 0]) >= 0
    assert_equal(?\(, GLib::UniChar.get_mirror_char(?\)))
    assert_equal(?\), GLib::UniChar.get_mirror_char(?\())
    assert_equal(?x, GLib::UniChar.get_mirror_char(?x))
  end

  def test_unichar_get_script
    return unless (GLib::VERSION <=> [2, 14, 0]) >= 0
    require 'uconv'
    assert_equal(GLib::Unicode::SCRIPT_HIRAGANA,
                 GLib::UniChar.get_script(Uconv.u8tou4("あ").unpack("L*")[0]))
  end

  def test_utf8_get_char
    assert_equal(Uconv.u8tou4("あ").unpack("L*")[0],
                 GLib::UTF8.get_char("あ"))

    assert_equal(Uconv.u8tou4("あ").unpack("L*")[0],
                 GLib::UTF8.get_char("あ", true))
    partial_input = "あ".unpack("c*")[0..-2].pack("c*")
    assert_equal(-2, GLib::UTF8.get_char(partial_input, true))
    invalid_input = "あ".unpack("c*")[2..-1].pack("c*")
    assert_equal(-1, GLib::UTF8.get_char(invalid_input, true))
  end

  def test_utf8_size
    assert_equal(1, GLib::UTF8.size("あ"))
    assert_equal(2, GLib::UTF8.size("あい"))
  end

  def test_utf8_reverse
    assert_equal("おえういあ", GLib::UTF8.reverse("あいうえお"))
  end

  def test_utf8_validate
    assert(GLib::UTF8.validate("あ"))
    assert(!GLib::UTF8.validate("あ"[1..-1]))
  end

  def test_utf8_upcase
    assert_equal("ABCあいう", GLib::UTF8.upcase("aBcあいう"))
  end

  def test_utf8_downcase
    assert_equal("abcあいう", GLib::UTF8.downcase("aBcあいう"))
  end

  def test_utf8_casefold
    assert_equal(GLib::UTF8.casefold("AbCあいう"),
                 GLib::UTF8.casefold("aBcあいう"))
  end

  def test_utf8_normalize
    original = [0x00c1].pack("U*") # A with acute

    nfd = [0x0041, 0x0301].pack("U*")
    assert_equal(nfd,
                 GLib::UTF8.normalize(original, GLib::NormalizeMode::NFD))

    nfc = [0x00c1].pack("U*")
    assert_equal(nfc,
                 GLib::UTF8.normalize(original, GLib::NormalizeMode::NFC))

    nfkd = [0x0041, 0x0301].pack("U*")
    assert_equal(nfkd,
                 GLib::UTF8.normalize(original, GLib::NormalizeMode::NFKD))

    nfkc = [0x00c1].pack("U*")
    assert_equal(nfkc,
                 GLib::UTF8.normalize(original, GLib::NormalizeMode::NFKC))
  end

  def test_utf8_collate
    assert_operator(0, :>, GLib::UTF8.collate("あ", "い"))
    assert_operator(0, :<, GLib::UTF8.collate("い", "あ"))
    assert_equal(0, GLib::UTF8.collate("あ", "あ"))
  end

  def test_utf8_collate_key
    assert_operator(0, :>,
                    GLib::UTF8.collate_key("あ") <=>
                    GLib::UTF8.collate_key("い"))
    assert_operator(0, :<,
                    GLib::UTF8.collate_key("い") <=>
                    GLib::UTF8.collate_key("あ"))
    assert_equal(0,
                 GLib::UTF8.collate_key("あ") <=>
                 GLib::UTF8.collate_key("あ"))
  end

  def test_utf8_collate_key_for_filename
    assert_equal(["event.c", "event.h", "eventgenerator.c"],
                 ["event.c", "eventgenerator.c", "event.h"].sort_by do |f|
                   GLib::UTF8.collate_key(f, true)
                 end)

    assert_equal(["file1", "file5", "file10"],
                 ["file1", "file10", "file5"].sort_by do |f|
                   GLib::UTF8.collate_key(f, true)
                 end)
  end

  def test_utf8_to_utf16
    require 'uconv'
    assert_equal(Uconv.u8tou16("あいうえお"),
                 GLib::UTF8.to_utf16("あいうえお"))
  end

  def test_utf8_to_ucs4
    require 'uconv'
    assert_equal(Uconv.u8tou4("あいうえお"),
                 GLib::UTF8.to_ucs4("あいうえお"))

    assert_raise(GLib::ConvertError) do
      GLib::UTF8.to_ucs4("あいうえお"[1..-1])
    end
    assert_nothing_raised do
      GLib::UTF8.to_ucs4("あいうえお"[1..-1], true)
    end
  end

  def test_utf16_to_ucs4
    require 'uconv'
    assert_equal(Uconv.u8tou4("あいうえお"),
                 GLib::UTF16.to_ucs4(Uconv.u8tou16("あいうえお")))
  end

  def test_utf16_to_utf8
    require 'uconv'
    assert_equal("あいうえお",
                 GLib::UTF16.to_utf8(Uconv.u8tou16("あいうえお")))
  end


  def test_ucs4_to_utf16
    require 'uconv'
    assert_equal(Uconv.u8tou16("あいうえお"),
                 GLib::UCS4.to_utf16(Uconv.u8tou4("あいうえお")))

    assert_raise(GLib::ConvertError) do
      GLib::UCS4.to_utf16(Uconv.u8tou4("あいうえお")[1..-1])
    end
  end

  def test_utf16_to_utf8
    require 'uconv'
    assert_equal("あいうえお",
                 GLib::UCS4.to_utf8(Uconv.u8tou4("あいうえお")))
  end

  def test_unichar_to_utf8
    require 'uconv'
    assert_equal("あ",
                 GLib::UniChar.to_utf8(Uconv.u8tou4("あ").unpack("L*")[0]))
  end
end

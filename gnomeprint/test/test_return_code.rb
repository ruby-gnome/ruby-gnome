require "gnomeprint2"

class TestReturnCode < Test::Unit::TestCase

  def test_to_s
    assert_equal("OK", Gnome::PrintReturnCode::OK.to_s)
    assert_equal("unknown error", Gnome::PrintReturnCode::ERROR_UNKNOWN.to_s)
    assert_equal("bad value", Gnome::PrintReturnCode::ERROR_BADVALUE.to_s)
    assert_equal("no current point",
                 Gnome::PrintReturnCode::ERROR_NOCURRENTPOINT.to_s)
    assert_equal("no current path",
                 Gnome::PrintReturnCode::ERROR_NOCURRENTPATH.to_s)
    assert_equal("text corrupt",
                 Gnome::PrintReturnCode::ERROR_TEXTCORRUPT.to_s)
    assert_equal("bad context",
                 Gnome::PrintReturnCode::ERROR_BADCONTEXT.to_s)
    assert_equal("no page", Gnome::PrintReturnCode::ERROR_NOPAGE.to_s)
    assert_equal("no match", Gnome::PrintReturnCode::ERROR_NOMATCH.to_s)
  end
  
end

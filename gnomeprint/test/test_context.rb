require "gnomeprint2"

class TestContext < Test::Unit::TestCase

  def setup
    @job = Gnome::PrintJob.new
    @context = @job.context
  end
  
  def test_close
    # @context.close
  end

  def test_line_to
    begin
      @context.line_to(0, 10);
    rescue Gnome::PrintError
      assert_equal(Gnome::PrintReturnCode::ERROR_NOPAGE, $!.code)
    end
    @context.begin_page do
      begin
        @context.line_to(0, 10);
      rescue Gnome::PrintError
        assert_equal(Gnome::PrintReturnCode::ERROR_NOCURRENTPOINT, $!.code)
      end
    end
  end
  
end

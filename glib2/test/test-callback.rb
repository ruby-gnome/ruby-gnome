require_relative "fixture/callback-requester/callback_requester"

class TestCallback < Test::Unit::TestCase
  def test_callback_segv
    requester = CallbackRequester.new(Object.new)

    assert_not_nil(requester)
  end
end

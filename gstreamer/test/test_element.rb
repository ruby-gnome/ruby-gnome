class TestElement < Test::Unit::TestCase
  include GstTestUtils

  def test_clock
    bin = create_element("playbin")
    assert(bin.require_clock?)
    assert(bin.provide_clock?)
    assert_nil(bin.clock)
  end

  def test_each_pad
    pads = []
    create_element("filesrc").each_pad do |pad|
      pads << pad.name
    end
    assert_equal(["src"], pads)
  end

  def test_state
    pipeline = Gst::Pipeline.new
    src = create_element("videotestsrc")
    sink = create_element("fakesink")
    pipeline.add(src, sink)

    assert_equal([Gst::STATE_CHANGE_SUCCESS,
                  Gst::STATE_NULL,
                  Gst::STATE_VOID_PENDING],
                 pipeline.get_state)

    pipeline.ready
    assert_equal([Gst::STATE_CHANGE_SUCCESS,
                  Gst::STATE_READY,
                  Gst::STATE_VOID_PENDING],
                 pipeline.get_state)

    pipeline.play
    assert_equal([Gst::STATE_CHANGE_ASYNC,
                  Gst::STATE_READY,
                  Gst::STATE_PLAYING],
                 pipeline.get_state(10))

    pipeline.pause
    assert_equal([Gst::STATE_CHANGE_ASYNC,
                  Gst::STATE_READY,
                  Gst::STATE_PAUSED],
                 pipeline.get_state(10))

    pipeline.stop
    assert_equal([Gst::STATE_CHANGE_SUCCESS,
                  Gst::STATE_NULL,
                  Gst::STATE_VOID_PENDING],
                 pipeline.get_state(10))

    pipeline.set_state(:playing)
    pend("Async is correct behavior?") do
      assert_equal([Gst::STATE_CHANGE_SUCCESS,
                    Gst::STATE_PLAYING,
                    Gst::STATE_VOID_PENDING],
                   pipeline.get_state(10))
    end
  end
end

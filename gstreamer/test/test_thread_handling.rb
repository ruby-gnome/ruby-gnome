class TestThreadHandling < Test::Unit::TestCase
  def test_scheduling
    pipeline = Gst::Pipeline.new
    buffers_seen = 0

    src = Gst::ElementFactory.make("videotestsrc")
    identity = Gst::ElementFactory.make("identity")
    sink = Gst::ElementFactory.make("fakesink")
    caps = Gst::Caps.parse("video/x-raw-yuv, framerate=\(fraction\)30/1")

    src.live = true
    identity.signal_handoffs = true

    identity.signal_connect("handoff") do |element, buffer|
      buffers_seen += 1
    end

    pipeline.add(src, identity, sink)
    src.link_filtered(identity, caps)
    identity >> sink

    pipeline.play
    sleep 1
    assert_operator(15, :<, buffers_seen)
  end
end

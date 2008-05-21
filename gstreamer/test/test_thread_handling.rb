class TestThreadHandling < Test::Unit::TestCase
  def test_scheduling
    only_gst_version(0, 10, 16)
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
    pipeline.stop
    assert_operator(15, :<, buffers_seen)
  end

  def test_state_changes
    only_gst_version(0, 10, 16)
    pipeline = Gst::Pipeline.new
    buffers_seen = 0

    src = Gst::ElementFactory.make("videotestsrc")
    identity = Gst::ElementFactory.make("identity")
    sink = Gst::ElementFactory.make("fakesink")

    identity.signal_handoffs = true

    identity.signal_connect("handoff") do |element, buffer|
       buffers_seen += 1
    end

    pipeline.add(src, identity, sink)
    src >> identity >> sink
    loop = GLib::MainLoop.new(nil, false)

    10.times do
      pipeline.play
      sleep 1
      pipeline.stop
    end
  end
end

class TestPad < Test::Unit::TestCase
  include GstTestUtils

  def test_link
    file_src = create_element("filesrc")
    audio_convert = create_element("audioconvert")
    audio_resample = create_element("audioresample")
    file_sink = create_element("filesink")

    current_linked = Proc.new do
      [
       file_src["src"].linked?,
       audio_convert["sink"].linked?,
       audio_convert["src"].linked?,
       audio_resample["sink"].linked?,
       audio_resample["src"].linked?,
       file_sink["sink"].linked?,
      ]
    end

    current_peer = Proc.new do
      [
       file_src["src"].peer,
       audio_convert["sink"].peer,
       audio_convert["src"].peer,
       audio_resample["sink"].peer,
       audio_resample["src"].peer,
       file_sink["sink"].peer,
      ]
    end

    assert_equal([false, false, false, false, false, false],
                 current_linked.call)
    assert_equal([nil, nil, nil, nil, nil, nil],
                 current_peer.call)

    file_src["src"].link(audio_convert["sink"])
    assert_equal([true, true, false, false, false, false],
                 current_linked.call)
    assert_equal([audio_convert["sink"], file_src["src"], nil, nil, nil, nil],
                 current_peer.call)

    audio_convert["src"] >> audio_resample["sink"]
    assert_equal([true, true, true, true, false, false],
                 current_linked.call)
    assert_equal([audio_convert["sink"], file_src["src"],
                  audio_resample["sink"], audio_convert["src"],
                  nil, nil],
                 current_peer.call)

    audio_resample["src"] >> file_sink["sink"]
    assert_equal([true, true, true, true, true, true],
                 current_linked.call)
    assert_equal([audio_convert["sink"], file_src["src"],
                  audio_resample["sink"], audio_convert["src"],
                  file_sink["sink"], audio_resample["src"]],
                 current_peer.call)

    audio_convert["src"].unlink(audio_resample["sink"])
    assert_equal([true, true, false, false, true, true],
                 current_linked.call)
    assert_equal([audio_convert["sink"], file_src["src"],
                  nil, nil,
                  file_sink["sink"], audio_resample["src"]],
                 current_peer.call)
  end
end


# Copyright (C) 2024-2025  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "fiddle"

class TestAudio < Test::Unit::TestCase
  include GStreamerTestUtils

  def test_audio_info
    audio_info = Gst::AudioInfo.new
    audio_info.set_format(:encoded, 44100, 2)
    structure = audio_info.to_caps.structures[0]
    assert_equal("ENCODED", structure["format"].value)
    assert_equal(44100, audio_info.rate)
    assert_equal(2, audio_info.channels)

    audio_info.set_format(:f32le, 16000, 1)
    modified_structure = audio_info.to_caps.structures[0]
    assert_equal("F32LE", modified_structure["format"].value)
    assert_equal(16000, audio_info.rate)
    assert_equal(1, audio_info.channels)
  end

  def test_audio_info_from_caps
    only_gstreamer_version(1, 20)

    caps = Gst::Caps.new("audio/ogg")
    caps["rate", :int] = 44100
    caps["channels", :int] = 2

    audio_info = Gst::AudioInfo.new(caps)
    structure = audio_info.to_caps.structures[0]
    assert_equal("ENCODED", structure["format"].value)
    assert_equal(44100, audio_info.rate)
    assert_equal(2, audio_info.channels)

    audio_info.set_format(:f32le, 16000, 1)
    modified_structure = audio_info.to_caps.structures[0]
    assert_equal("F32LE", modified_structure["format"].value)
    assert_equal(16000, audio_info.rate)
    assert_equal(1, audio_info.channels)
  end

  def test_audio_make_sample
    only_gstreamer_version(1, 20)

    rate = 16000
    memory_view_producers = generate_samples(rate:)
    memory_view_producers.each do |memory_view_producer|
      sample = GstAudio.audio_make_sample(memory_view_producer, rate:)
      info = Gst::AudioInfo.new(sample.caps)

      assert_equal({
                      layout: "interleaved",
                      rate: rate,
                      channels: 1
                    },
                    {
                      layout: info.layout.nick,
                      rate: info.rate,
                      channels: info.channels
                    })
      Fiddle::MemoryView.export(memory_view_producer) do |view|
        assert_equal("e", view.format)

        actual = nil
        sample.buffer.map(:read) {|map| actual = map.data}
        assert_equal view.to_s, actual.pack("C*")
      end
    end
  end

  def test_audio_make_sample_stereo
    only_gstreamer_version(1, 20)

    rate = 16000
    memory_view_producers = generate_samples(channels: 2, rate:)
    memory_view_producers.each do |memory_view_producer|
      sample = GstAudio.audio_make_sample(memory_view_producer, rate:)
      info = Gst::AudioInfo.new(sample.caps)

      assert_equal({
                      layout: "interleaved",
                      rate: rate,
                      channels: 2
                    },
                    {
                      layout: info.layout.nick,
                      rate: info.rate,
                      channels: info.channels
                    })
      Fiddle::MemoryView.export(memory_view_producer) do |view|
        assert_equal("e", view.format)

        actual = nil
        sample.buffer.map(:read) {|map| actual = map.data}
        assert_equal view.to_s, actual.pack("C*")
      end
    end
  end

  def test_audio_make_sample_s16
    only_gstreamer_version(1, 20)

    rate = 16000
    memory_view_producers = generate_samples(format: "S16LE", rate:)
    memory_view_producers.each do |memory_view_producer|
      sample = GstAudio.audio_make_sample(memory_view_producer, rate:)
      info = Gst::AudioInfo.new(sample.caps)

      assert_equal({
                      layout: "interleaved",
                      rate: rate,
                      channels: 1
                    },
                    {
                      layout: info.layout.nick,
                      rate: info.rate,
                      channels: info.channels
                    })
      Fiddle::MemoryView.export(memory_view_producer) do |view|
        assert_equal("s<", view.format)

        actual = nil
        sample.buffer.map(:read) {|map| actual = map.data}
        assert_equal view.to_s, actual.pack("C*")
      end
    end
  end

  def test_audio_make_sample_without_rate
    only_gstreamer_version(1, 20)

    memory_view_producer = generate_samples[0]

    assert_raise(ArgumentError) do
      GstAudio.audio_make_sample(memory_view_producer)
    end
  end

  def test_audio_make_sample_non_memory_view_producer
    only_gstreamer_version(1, 20)

    assert_raise(ArgumentError) do
      GstAudio.audio_make_sample(Object.new, rate: 16000)
    end
  end

  def test_audio_make_sample_without_format
    only_gstreamer_version(1, 20)

    assert_raise ArgumentError do
      GstAudio.audio_make_sample(Fiddle::Pointer["format not defined"], rate: 16000)
    end
  end
end

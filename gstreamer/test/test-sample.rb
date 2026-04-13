# Copyright (C) 2026  Ruby-GNOME Project Team
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

class TestSample < Test::Unit::TestCase
  include GStreamerTestUtils

  AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER = 1024
  AUDIO_TEST_SRC_RAMP = 1

  def test_memory_view_mono
    only_gstreamer_version(1, 20)

    samples = generate_samples

    samples.each_with_index do |sample, i|
      Fiddle::MemoryView.export(sample) do |view|
        assert_equal({
                       byte_size: AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER * 4,
                       readonly?: true,
                       format: "e",
                       item_size: 4,
                       ndim: 2,
                       shape: [AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER, 1],
                       strides: [4, 4],
                       sub_offsets: nil,
                     },
                     {
                       byte_size: view.byte_size,
                       readonly?: view.readonly?,
                       format: view.format,
                       item_size: view.item_size,
                       ndim: view.ndim,
                       shape: view.shape,
                       strides: view.strides,
                       sub_offsets: view.sub_offsets,
                     })

        offset = AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER * i
        (view.byte_size / view.item_size).times do |j|
          assert_in_delta(Math.sin((offset + j + AUDIO_TEST_SRC_RAMP) * 2 * Math::PI * 440 / 16_000) * 0.8,
                          view[j, 0],
                          0.001,
                          "#{i}th buffer, #{j}th sample")
        end
      end
    end
  end

  def test_memory_view_stereo
    only_gstreamer_version(1, 20)

    samples = generate_samples(channels: 2)

    samples.each_with_index do |sample, i|
      Fiddle::MemoryView.export(sample) do |view|
        assert_equal({
                        byte_size: AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER * 4 * 2,
                        readonly?: true,
                        format: "e",
                        item_size: 4,
                        ndim: 2,
                        shape: [AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER, 2],
                        strides: [8, 4],
                        sub_offsets: nil,
                     },
                     {
                       byte_size: view.byte_size,
                       readonly?: view.readonly?,
                       format: view.format,
                       item_size: view.item_size,
                       ndim: view.ndim,
                       shape: view.shape,
                       strides: view.strides,
                       sub_offsets: view.sub_offsets,
                     })

        offset = AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER * i
        (view.byte_size / view.item_size).times do |j|
          index, channel = j.divmod(2)
          expected_value = Math.sin((offset + index + AUDIO_TEST_SRC_RAMP) * 2 * Math::PI * 440 / 16_000) * 0.8
          assert_in_delta(expected_value, view[index, channel], 0.001, "#{i}th buffer, #{channel}th channel, #{index}th sample")
        end
      end
    end
  end

  def test_memory_view_non_interleaved
    only_gstreamer_version(1, 20)

    samples = generate_samples(layout: "non-interleaved", channels: 2)

    assert_raise(ArgumentError) do
      Fiddle::MemoryView.export(samples[0]) {}
    end
  end

  def test_memory_view_unsupported_format
    only_gstreamer_version(1, 20)

    samples = generate_samples(format: "S24_32LE")

    assert_raise(ArgumentError) do
      Fiddle::MemoryView.export(samples[0]) {}
    end
  end

  def test_memory_view_from_buffer_list
    only_gstreamer_version(1, 20)

    samples = generate_samples
    assert_equal(15, samples.size)

    buffer_list = Gst::BufferList.new
    expected_data = +"".b

    samples.each do |sample|
      buffer = sample.buffer
      buffer.map(:read) do |map|
        expected_data << map.data.pack("C*")
      end
      buffer_list.insert(-1, buffer)
    end

    caps = samples.first.caps
    sample = Gst::Sample.new(Gst::Buffer.new, caps, Gst::Segment.new, caps.structures[0])
    sample.buffer_list = buffer_list

    Fiddle::MemoryView.export(sample) do |view|
      assert_equal(expected_data, view.to_s)
    end
  end

  private

  def generate_samples(format: "F32LE", layout: "interleaved", rate: 16_000, channels: 1)
    samples = []

    pipeline = Gst::Pipeline.new("audio-generator")
    src = Gst::ElementFactory.make("audiotestsrc", nil)
    convert = Gst::ElementFactory.make("audioconvert", nil)
    sink = Gst::ElementFactory.make("appsink", nil)

    src.set_property("num-buffers", rate / AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER)

    caps = Gst::Caps.new("audio/x-raw")
    caps["format"] = format
    caps["rate", :int] = rate
    caps["channels", :int] = channels
    caps["layout"] = layout

    sink.caps = caps

    pipeline << src << convert << sink
    src >> convert >> sink

    pipeline.play
    begin
      loop do
        sample = sink.try_pull_sample(Gst::SECOND)
        if sample
          samples << sample
        else
          break
        end
      end
    ensure
      pipeline.stop
    end

    samples
  end
end

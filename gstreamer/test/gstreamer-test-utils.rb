# Copyright (C) 2014-2021  Ruby-GNOME Project Team
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

require "gst"

module GStreamerTestUtils
  AUDIO_TEST_SRC_DEFAULT_SAMPLES_PER_BUFFER = 1024
  AUDIO_TEST_SRC_RAMP = 1

  private
  def only_ruby_version(major, minor, micro=nil)
    micro ||= 0
    unless RUBY_VERSION[0, 5] >= [major, minor, micro].join(".")
      omit("Require Ruby >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_gstreamer_version(major, minor, micro=nil)
    micro ||= 0
    unless Gst::Version.or_later?(major, minor, micro)
      omit("Require GStreamer >= #{major}.#{minor}.#{micro}")
    end
  end

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

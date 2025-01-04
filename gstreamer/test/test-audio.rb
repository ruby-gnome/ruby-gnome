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

class TestAudio < Test::Unit::TestCase
  include GStreamerTestUtils

  def test_audio_info
    audio_info = Gst::AudioInfo.new
    audio_info.set_format(:encoded, 44100, 2)
    structure = audio_info.to_caps.structures[0]
    assert_equal("ENCODED", structure.get_value("format").value)
    assert_equal(44100, audio_info.rate)
    assert_equal(2, audio_info.channels)

    audio_info.set_format(:f32le, 16000, 1)
    modified_structure = audio_info.to_caps.structures[0]
    assert_equal("F32LE", modified_structure.get_value("format").value)
    assert_equal(16000, audio_info.rate)
    assert_equal(1, audio_info.channels)
  end

  def test_audio_info_from_caps
    only_gstreamer_version(1, 20)

    caps = Gst::Caps.new("audio/ogg")
    caps.set_value("rate", GLib::Value.new(GLib::Type::INT, 44100))
    caps.set_value("channels", GLib::Value.new(GLib::Type::INT, 2))

    audio_info = Gst::AudioInfo.new(caps)
    structure = audio_info.to_caps.structures[0]
    assert_equal("ENCODED", structure.get_value("format").value)
    assert_equal(44100, audio_info.rate)
    assert_equal(2, audio_info.channels)

    audio_info.set_format(:f32le, 16000, 1)
    modified_structure = audio_info.to_caps.structures[0]
    assert_equal("F32LE", modified_structure.get_value("format").value)
    assert_equal(16000, audio_info.rate)
    assert_equal(1, audio_info.channels)
  end
end

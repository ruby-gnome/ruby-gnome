# Copyright (C) 2024  Ruby-GNOME Project Team
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
  def test_audio
    caps = Gst::Caps.new("audio/ogg")
    caps.set_value("format", "S8")
    caps.set_value("rate", 44100)
    caps.set_value("channels", 2)

    audio_info = Gst::AudioInfo.new(caps)
    structure = audio_info.to_caps.structures[0]
    assert_equal(44100, structure.get_value("rate").value)
    assert_equal(2, structure.get_value("channels").value)

    audio_info.set_format(:f32le, 16000, 1)
    modified_structure = audio_info.to_caps.structures[0]
    assert_equal("F32LE", modified_structure.get_value("format").value)
    assert_equal(16000, modified_structure.get_value("rate").value)
    assert_equal(1, modified_structure.get_value("channels").value)
  end
end

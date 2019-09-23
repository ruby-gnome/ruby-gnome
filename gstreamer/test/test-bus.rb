# Copyright (C) 2019  Ruby-GNOME Project Team
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

class TestBus < Test::Unit::TestCase
  def setup
    @pipeline = Gst::Pipeline.new("my-pipeline")
    @source = Gst::ElementFactory.make("fakesrc", "source")
    @filter = Gst::ElementFactory.make("identity", "filter")
    @sink = Gst::ElementFactory.make("fakesink", "sink")
    @pipeline.add_elements(@source, @filter, @sink)
    @source.link(@filter, @sink)
    @bus = @pipeline.bus
  end

  def test_add_watch
    message_types = []
    loop = GLib::MainLoop.new
    @bus.add_watch do |_bus, message|
      message_types << message.type
      loop.quit
      GLib::Source::REMOVE
    end
    @pipeline.play
    loop.run
    assert_equal([Gst::MessageType::STATE_CHANGED],
                 message_types)
  end
end

# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class TestBin < Test::Unit::TestCase
  class TestEach < self
    def setup
      @pipeline = Gst::Pipeline.new("my-pipeline")
      @source = Gst::ElementFactory.make("fakesrc", "source")
      @filter = Gst::ElementFactory.make("identity", "filter")
      @sink = Gst::ElementFactory.make("fakesink", "sink")
      @pipeline.add_elements(@source, @filter, @sink)
      @source.link(@filter, @sink)
    end

    def test_elements
      assert_equal(sort_elements([@source, @filter, @sink]),
                   sort_elements(@pipeline.each.to_a))
    end

    def test_sorted
      unless Gst::Version.or_later?(1, 9, 3)
        omit("Crash because gst_bin_sort_iterator_copy() doesn't copy " +
             "GstBinSortIterator::queue")
      end
      assert_equal([@sink, @filter, @source],
                   @pipeline.each(:sorted => true).to_a)
    end

    private
    def sort_elements(elements)
      elements.sort_by {|element| element.class.gtype.name}
    end
  end
end

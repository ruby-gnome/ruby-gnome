# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

class ClutterFeatureTest < Test::Unit::TestCase
  include ClutterTestUtils

  def test_feature_avaliable
    feature_flag = Clutter::FeatureFlags::SYNC_TO_VBLANK
    assert_boolean(Clutter::Feature.available?(feature_flag))
  end

  def test_feature_get_all
    assert_nothing_raised do
      Clutter::Feature.all
    end
  end

  class TestEnum
    def test_feature_flags
      assert_const_defined(Clutter::FeatureFlags, :TEXTURE_NPOT)
    end
  end
end

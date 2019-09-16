# -*- coding: utf-8 -*-
#
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

class TestGtkEntry < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @entry = Gtk::Entry.new
  end

  def test_cursor_hadjustment
    assert_nil(@entry.cursor_hadjustment)
    adjustment = Gtk::Adjustment.new(0.0, 0.0,
                                     100.0, 100.0,
                                     1.0, 1.0)
    @entry.cursor_hadjustment = adjustment
    assert_equal(adjustment, @entry.cursor_hadjustment)
  end

  def test_completion_accsessors
    entry_completion = Gtk::EntryCompletion.new
    @entry.completion = entry_completion
    assert_equal(entry_completion, @entry.completion)
  end

  class TestEnum < self
    def test_input_purpose_enum
      only_gtk_version(3, 6, 0)
      assert_const_defined(Gtk::InputPurpose, :ALPHA)
    end

    def test_input_hints_enum
      only_gtk_version(3, 6, 0)
      assert_const_defined(Gtk::InputHints, :SPELLCHECK)
    end
  end
end

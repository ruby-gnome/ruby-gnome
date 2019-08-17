# coding: ascii-8bit
#
# Copyright (C) 2015-2019  Ruby-GNOME2 Project Team
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

require 'test/unit'
require 'glib2'

class TestGLib < Test::Unit::TestCase

  def test_version
    assert_kind_of(Array, GLib::VERSION)
    assert_equal(GLib::VERSION.length, 3)
    assert(GLib::VERSION.all?{|i| i.is_a? Integer })

    assert_kind_of(Integer, GLib::MAJOR_VERSION)
    assert_kind_of(Integer, GLib::MINOR_VERSION)
    assert_kind_of(Integer, GLib::MICRO_VERSION)

    assert_kind_of(Array, GLib::BINDING_VERSION)
    assert_equal(GLib::BINDING_VERSION.length, 3)
    assert(GLib::BINDING_VERSION.all?{|i| i.is_a? Integer })
  end

  def test_priority
    assert_kind_of(Integer, GLib::PRIORITY_HIGH)
    assert_kind_of(Integer, GLib::PRIORITY_DEFAULT)
    assert_kind_of(Integer, GLib::PRIORITY_HIGH_IDLE)
    assert_kind_of(Integer, GLib::PRIORITY_DEFAULT_IDLE)
    assert_kind_of(Integer, GLib::PRIORITY_LOW)
  end

  def test_int64
  end

  def test_convert
    assert_kind_of(String, GLib.charset)

    sjis = "\202\261\202\361\202\311\202\277\202\315\220\242\212E".force_encoding("Shift_JIS")
    euc  = "\244\263\244\363\244\313\244\301\244\317\300\244\263\246".force_encoding("EUC-JP")
    utf8 = "\343\201\223\343\202\223\343\201\253\343\201\241\343\201\257\344\270\226\347\225\214".force_encoding("UTF-8")
    assert_equal(utf8, GLib.convert(sjis, "UTF-8", "SHIFT_JIS"))
    assert_equal(euc, GLib.convert(sjis, "EUC-JP", "SHIFT_JIS"))
    assert_equal(sjis, GLib.convert(sjis, "SHIFT_JIS", "SHIFT_JIS"))
    assert_equal(utf8, GLib.convert(euc, "UTF-8", "EUC-JP"))
    assert_equal(euc, GLib.convert(euc, "EUC-JP", "EUC-JP"))
    assert_equal(sjis, GLib.convert(euc, "SHIFT_JIS", "EUC-JP"))
    assert_equal(utf8, GLib.convert(utf8, "UTF-8", "UTF-8"))
    assert_equal(euc, GLib.convert(utf8, "EUC-JP", "UTF-8"))
    assert_equal(sjis, GLib.convert(utf8, "SHIFT_JIS", "UTF-8"))
  end

  def test_locale_to_utf8
    assert_equal(Encoding::UTF_8,
                 GLib.locale_to_utf8("ascii").encoding)
  end

  def test_locale_from_utf8
    assert_equal(Encoding::ASCII_8BIT,
                 GLib.locale_from_utf8("ascii").encoding)
  end

  def test_filename_to_utf8
    assert_equal(Encoding::UTF_8,
                 GLib.filename_to_utf8("ascii.txt").encoding)
  end

  def test_filename_from_utf8
    assert_equal(GLib::FILENAME_ENCODING,
                 GLib.filename_from_utf8("ascii.txt").encoding)
  end

    # rb_define_module_function(mGLib, "filename_to_uri", rbglib_m_filename_to_uri, -1);
    # rb_define_module_function(mGLib, "filename_from_uri", rbglib_m_filename_from_uri, 1);


  def test_messages
    #rb_define_module_function(mGLog, "set_handler", rbglib_m_log_set_handler, 2);
    #rb_define_module_function(mGLog, "remove_handler", rbglib_m_log_remove_handler, 2);
  end

  def test_object
    assert_raises(GLib::NoPropertyError) {
      GLib::Object.property("foo")
    }

    assert_raises(GLib::NoSignalError) {
      GLib::Object.signal("foo")
    }
  end

  def test_interface_extend
    assert_raises(TypeError){
      Object.__send__(:include, GLib::TypePlugin)
    }
  end

  def test_signal_has_handler_pending
    obj = GLib::Object.new
    signal_name = "notify"

    assert(!obj.signal_has_handler_pending?(signal_name))

    h = obj.signal_connect(signal_name){}
    assert(obj.signal_has_handler_pending?(signal_name))

    obj.signal_handler_block(h) {
      assert(obj.signal_has_handler_pending?(signal_name, true))
      assert(!obj.signal_has_handler_pending?(signal_name, false))
    }
    assert(obj.signal_has_handler_pending?(signal_name, false))

    obj.signal_handler_disconnect(h)
    assert(!obj.signal_has_handler_pending?(signal_name))

    obj = nil
  end

  def test_signal_handler_disconnect_and_gc
    obj = GLib::Object.new
    klass = Class.new
    1000.times {
      a = klass.new
      id = obj.signal_connect("notify") { p a }
      obj.signal_handler_disconnect(id)
    }
    GC.start
    ary = []
    ObjectSpace.each_object(klass) { |a| ary.push(a) }
    assert_operator(ary.size, :<, 1000)
  end

  def test_gtype
    assert_equal(GLib::Object.gtype, GLib::Type["GObject"])
    assert_equal(GLib::Interface.gtype, GLib::Type["GInterface"])

    obj = GLib::Object.new
    assert_equal(obj.gtype, GLib::Object.gtype)
  end
end

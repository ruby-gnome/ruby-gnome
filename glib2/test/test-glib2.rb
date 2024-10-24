# Copyright (C) 2015-2024  Ruby-GNOME Project Team
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

    # U+3042 HIRAGANA LETTER A
    # U+3044 HIRAGANA LETTER I
    # U+3046 HIRAGANA LETTER U
    utf8 = "\u3042\u3044\u3046"
    sjis = utf8.encode("Shift_JIS")
    euc  = utf8.encode("EUC-JP")
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
    object = GLib::Object.new
    klass = Class.new
    n_tries = 100
    n_tries.times {
      garbage_collected_object = klass.new
      id = object.signal_connect("notify") { p garbage_collected_object }
      object.signal_handler_disconnect(id)
    }
    GC.start
    n_alive_objects = ObjectSpace.each_object(klass) {}
    assert do
      n_alive_objects < n_tries
    end
  end

  def test_gtype
    assert_equal(GLib::Object.gtype, GLib::Type["GObject"])
    assert_equal(GLib::Interface.gtype, GLib::Type["GInterface"])

    obj = GLib::Object.new
    assert_equal(obj.gtype, GLib::Object.gtype)
  end

  def test_malloc
    address = GLib.malloc(29)
    GLib.free(address)
  end

  def test_malloc0
    address = GLib.malloc0(29)
    GLib.free(address)
  end
end

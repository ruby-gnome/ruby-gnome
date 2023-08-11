# Copyright (C) 2014-2023  Ruby-GNOME Project Team
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

class TestTerminalSignals < Test::Unit::TestCase
  def setup
    if Vte::Version.or_later?(0, 64)
      omit("VTE 0.64 or later doesn't work in Docker by default")
      # /usr/include/c++/11/bits/shared_ptr_base.h:976: std::__shared_ptr_access<
      #  _Tp,
      #  _Lp,
      #  <anonymous>,
      #  <anonymous>
      # >::element_type&
      # std::__shared_ptr_access<
      #  _Tp,
      #  _Lp,
      #  <anonymous>,
      #  <anonymous>
      # >::operator*() const [
      # with
      #  _Tp = vte::platform::Clipboard;
      # __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic;
      #  bool <anonymous> = false;
      # bool <anonymous> = false;
      # std::__shared_ptr_access<
      #  _Tp,
      #  _Lp,
      #  <anonymous>,
      #  <anonymous>
      # >::element_type = vte::platform::Clipboard
      #]: Assertion '_M_get() != nullptr' failed.
    end

    @terminal = Vte::Terminal.new
  end

  def test_copy_clipboard_signal
    called = false
    @terminal.signal_connect("copy-clipboard") do
      called = true
    end
    @terminal.signal_emit("copy-clipboard")
    assert_true(called)
  end

  def test_paste_clipboard_signal
    called = false
    @terminal.signal_connect("paste-clipboard") do
      called = true
    end
    @terminal.signal_emit("paste-clipboard")
    assert_true(called)
  end
end

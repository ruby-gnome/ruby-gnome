# Copyright (C) 2026  Ruby-GNOME Project Team
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

require "rbconfig"

class TestGLibTypeGC < Test::Unit::TestCase
  include GLibTestUtils

  # Regression test for a heap-use-after-free during interpreter
  # shutdown.
  #
  # Every class registered through the rbgobj type system (G_DEF_CLASS,
  # GObjectIntrospection's define_class, ...) is wrapped by an
  # RGObjClassInfo TypedData object. cinfo_free used to free the
  # object's own rb_data_type_t (xfree(cinfo->data_type)), but the GC
  # reads RTYPEDDATA_TYPE(obj)->flags *after* invoking the free callback
  # (RUBY_TYPED_EMBEDDABLE check in rb_data_free, Ruby >= 3.3), so
  # freeing the type struct from within its own dfree is a
  # use-after-free. All RGObjClassInfo wrappers now share a single
  # static rb_data_type_t (rg_class_info_type) that outlives every
  # wrapper, making this structurally impossible.
  #
  # These cinfo objects are pinned for the whole process (klass_to_cinfo),
  # so they are only freed by the shutdown finalizer
  # (rb_objspace_call_finalizer). We therefore exercise the bug in a
  # child process that simply loads glib2 and exits.
  #
  # The fault is only *detected* by a memory checker (an
  # AddressSanitizer/valgrind enabled Ruby). On a normal build the freed
  # struct is still mapped, so this test passes trivially and just acts
  # as a smoke test.
  def test_shutdown_does_not_use_freed_data_type
    code = <<~RUBY
      require "glib2"
      # Touch a handful of classes that are registered through the
      # rbgobj cinfo path; each is wrapped by an RGObjClassInfo whose
      # dfree (cinfo_free) runs in the shutdown finalizer.
      [
        GLib::Bytes,
        GLib::VariantType,
        GLib::DateTime,
        GLib::Regex,
      ]
      GC.start
    RUBY

    load_path_options = $LOAD_PATH.flat_map {|path| ["-I", path]}
    command = [RbConfig.ruby, *load_path_options, "-e", code]
    output = IO.popen(command, err: [:child, :out], &:read)
    status = $?

    assert(status.success?,
           "child process did not exit cleanly " \
           "(possible use-after-free at shutdown)\n" \
           "status: #{status.inspect}\n" \
           "output:\n#{output}")
  rescue NotImplementedError
    omit("Spawning a child process isn't supported on this platform")
  end
end

/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2004  Masao Mutoh
 *  Copyright (C) 2004  Kazuhiro NISHIYAMA
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgprivate.h"
#include "rbglib.h"

#define RG_TARGET_NAMESPACE cSpawnError

void
Init_glib_spawnerror(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_ERROR2(G_SPAWN_ERROR, "SpawnError", mGLib, rb_eIOError);

    rb_define_const(RG_TARGET_NAMESPACE, "FORK", INT2NUM(G_SPAWN_ERROR_FORK));
    rb_define_const(RG_TARGET_NAMESPACE, "READ", INT2NUM(G_SPAWN_ERROR_READ));
    rb_define_const(RG_TARGET_NAMESPACE, "CHDIR", INT2NUM(G_SPAWN_ERROR_CHDIR));
    rb_define_const(RG_TARGET_NAMESPACE, "EACCES", INT2NUM(G_SPAWN_ERROR_ACCES));
    rb_define_const(RG_TARGET_NAMESPACE, "EPERM", INT2NUM(G_SPAWN_ERROR_PERM));
    rb_define_const(RG_TARGET_NAMESPACE, "TOO_BIG", INT2NUM(G_SPAWN_ERROR_TOO_BIG));
    rb_define_const(RG_TARGET_NAMESPACE, "ENOEXEC", INT2NUM(G_SPAWN_ERROR_NOEXEC));
    rb_define_const(RG_TARGET_NAMESPACE, "ENAMETOOLONG", INT2NUM(G_SPAWN_ERROR_NAMETOOLONG));
    rb_define_const(RG_TARGET_NAMESPACE, "ENOENT", INT2NUM(G_SPAWN_ERROR_NOENT));
    rb_define_const(RG_TARGET_NAMESPACE, "ENOMEM", INT2NUM(G_SPAWN_ERROR_NOMEM));
    rb_define_const(RG_TARGET_NAMESPACE, "ENOTDIR", INT2NUM(G_SPAWN_ERROR_NOTDIR));
    rb_define_const(RG_TARGET_NAMESPACE, "ELOOP", INT2NUM(G_SPAWN_ERROR_LOOP));
    rb_define_const(RG_TARGET_NAMESPACE, "ETXTBUSY", INT2NUM(G_SPAWN_ERROR_TXTBUSY));
    rb_define_const(RG_TARGET_NAMESPACE, "EIO", INT2NUM(G_SPAWN_ERROR_IO));
    rb_define_const(RG_TARGET_NAMESPACE, "ENFILE", INT2NUM(G_SPAWN_ERROR_NFILE));
    rb_define_const(RG_TARGET_NAMESPACE, "EMFILE", INT2NUM(G_SPAWN_ERROR_MFILE));
    rb_define_const(RG_TARGET_NAMESPACE, "EINVAL", INT2NUM(G_SPAWN_ERROR_INVAL));
    rb_define_const(RG_TARGET_NAMESPACE, "EISDIR", INT2NUM(G_SPAWN_ERROR_ISDIR));
    rb_define_const(RG_TARGET_NAMESPACE, "ELIBBAD", INT2NUM(G_SPAWN_ERROR_LIBBAD));
    rb_define_const(RG_TARGET_NAMESPACE, "FAILED", INT2NUM(G_SPAWN_ERROR_FAILED));
}

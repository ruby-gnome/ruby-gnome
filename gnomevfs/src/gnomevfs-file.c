/*****************************************************************************
 *
 * gnomevfs-file.c: GnomeVFS::File class.
 *
 * Copyright (C) 2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * $Author: sakai $
 *
 * $Date: 2006/06/13 08:07:33 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"
#include "string.h"

/* Defines *******************************************************************/

#define _SELF(s)	((GnomeVFSHandle *)RVAL2BOXED(s, GNOMEVFS_TYPE_FILE))
#define RVAL2OPENMODE(x) ((GnomeVFSOpenMode)(RVAL2GFLAGS(x,GNOME_VFS_TYPE_VFS_OPEN_MODE)))
#define OPENMODE2RVAL(x) (GFLAGS2RVAL(x,GNOME_VFS_TYPE_VFS_OPEN_MODE))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

static VALUE s_default_rsep;

/* Function Implementations **************************************************/

static GnomeVFSHandle *
gnome_vfs_file_copy(gpointer boxed)
{
  return (GnomeVFSHandle *)boxed;
}

void
gnome_vfs_file_free(gpointer boxed)
{
  /*
   * do nothing for now...
   * we can probably say that #close() is enough
   */
}

GType
gnome_vfs_file_get_type(void)
{
  static GType our_type = 0;

  if (our_type == 0) {
    our_type = g_boxed_type_register_static("GnomeVFSFile",
                                            (GBoxedCopyFunc)gnome_vfs_file_copy,
                                            (GBoxedFreeFunc)gnome_vfs_file_free);
  }

  return our_type;
}

static VALUE
file_unlink(self, uri)
     VALUE self, uri;
{
  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
    return GVFSRESULT2RVAL(
                           gnome_vfs_unlink_from_uri(RVAL2GVFSURI(uri)));
  } else {
    return GVFSRESULT2RVAL(gnome_vfs_unlink(RVAL2CSTR(uri)));
  }
}

static VALUE
file_move(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE source, target, r_force;
  gboolean force;

  if (rb_scan_args(argc, argv, "21", &source, &target, &r_force) == 3) {
    force = RTEST(r_force);
  } else {
    force = TRUE;
  }

  /* XXX: this needs to be fixed */
  if (RTEST(rb_obj_is_kind_of(source, g_gvfs_uri))) {
    return GVFSRESULT2RVAL(
                           gnome_vfs_move_uri(RVAL2GVFSURI(source),
                                              RVAL2GVFSURI(target),
                                              force));
  } else {
    return GVFSRESULT2RVAL(gnome_vfs_move(RVAL2CSTR(source),
                                          RVAL2CSTR(target),
                                          force));
  }
}

static VALUE
create_symbolic_link(self, uri, reference)
     VALUE self, uri, reference;
{
  GnomeVFSURI *tmp;
  VALUE result;

  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
    tmp = RVAL2GVFSURI(uri);
    gnome_vfs_uri_ref(tmp);
  } else {
    tmp = gnome_vfs_uri_new(RVAL2CSTR(uri));
  }

  if (RTEST(rb_obj_is_kind_of(reference, g_gvfs_uri))) {
    gchar *str;

    str = gnome_vfs_uri_to_string(RVAL2GVFSURI(reference),
                                  GNOME_VFS_URI_HIDE_NONE);
    result = GVFSRESULT2RVAL(
                             gnome_vfs_create_symbolic_link(tmp, str));
    g_free(str);
  } else {
    result = GVFSRESULT2RVAL(
                             gnome_vfs_create_symbolic_link(tmp,
                                                            RVAL2CSTR(reference)));
  }

  gnome_vfs_uri_unref(tmp);
  return result;
}

static VALUE
apply_set_info(paths, info, mask)
     VALUE paths;
     GnomeVFSFileInfo *info;
     GnomeVFSSetFileInfoMask mask;
{
  VALUE path;
  int i, n;
  GnomeVFSResult result;

  Check_Type(paths, T_ARRAY);
  for (i = 0, n = RARRAY(paths)->len; i < n; i++) {
    path = rb_ary_entry(paths, i);
    if (RTEST(rb_obj_is_kind_of(path, g_gvfs_uri))) {
      result = GVFSRESULT2RVAL(
                               gnome_vfs_set_file_info_uri(RVAL2GVFSURI(path),
                                                           info, mask));
    } else {
      result = GVFSRESULT2RVAL(
                               gnome_vfs_set_file_info(RVAL2CSTR(path), info,
                                                       mask));
    }
    if (result != GNOME_VFS_OK) {
      return GVFSRESULT2RVAL(result);
    }
  }

  return Qtrue;
}

static VALUE
file_m_chmod(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE r_mode, paths;
  GnomeVFSFileInfo *info;
  VALUE value;

  rb_secure(2);
  info = gnome_vfs_file_info_new();
  rb_scan_args(argc, argv, "1*", &r_mode, &paths);
  info->permissions = RVAL2GFLAGS(r_mode, GNOME_VFS_TYPE_VFS_FILE_PERMISSIONS);
  value = apply_set_info(paths, info,
                         GNOME_VFS_SET_FILE_INFO_PERMISSIONS);
  gnome_vfs_file_info_unref(info);
  return value;
}

static VALUE
file_m_chown(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE r_owner, r_group, paths;
  GnomeVFSFileInfo *info;
  VALUE value;

  rb_secure(2);
  info = gnome_vfs_file_info_new();
  rb_scan_args(argc, argv, "2*", &r_owner, &r_group, &paths);
  /* XXX: hm...how about -1? works for UNIX, but what else? */
  if (NIL_P(r_owner)) {
    info->uid = (guint)-1;
  } else {
    info->uid = NUM2INT(r_owner);
  }
  if (NIL_P(r_group)) {
    info->gid = (guint)-1;
  } else {
    info->gid = NUM2INT(r_group);
  }

  value = apply_set_info(paths, info, GNOME_VFS_SET_FILE_INFO_OWNER);
  gnome_vfs_file_info_unref(info);
  return value;
}

static VALUE
file_m_lstat(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE uri, r_options;
  GnomeVFSFileInfo *info;
  GnomeVFSFileInfoOptions options;
  GnomeVFSResult result;

  if (rb_scan_args(argc, argv, "11", &uri, &r_options) == 1) {
    options = RVAL2GFLAGS(r_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
  } else {
    options = GNOME_VFS_FILE_INFO_DEFAULT;
  }

  info = gnome_vfs_file_info_new();

  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
    result = gnome_vfs_get_file_info_uri(RVAL2GVFSURI(uri),
                                         info,
                                         options);
  } else {
    result = gnome_vfs_get_file_info(RVAL2CSTR(uri),
                                     info,
                                     options);
  }

  return CHECK_RESULT(result, GVFSFILEINFO2RVAL(info));
}

static VALUE
file_m_stat(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE uri, r_options;
  GnomeVFSFileInfo *info;
  GnomeVFSFileInfoOptions options;
  GnomeVFSResult result;

  if (rb_scan_args(argc, argv, "11", &uri, &r_options) == 1) {
    options = RVAL2GFLAGS(r_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
  } else {
    options = GNOME_VFS_FILE_INFO_DEFAULT;
  }
  options |= GNOME_VFS_FILE_INFO_FOLLOW_LINKS;

  info = gnome_vfs_file_info_new();

  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
    result = gnome_vfs_get_file_info_uri(RVAL2GVFSURI(uri),
                                         info,
                                         options);
  } else {
    result = gnome_vfs_get_file_info(RVAL2CSTR(uri),
                                     info,
                                     options);
  }

  return CHECK_RESULT(result, GVFSFILEINFO2RVAL(info));
}

static VALUE
file_m_set_stat(self, uri, info, mask)
     VALUE self, uri, info, mask;
{
  GnomeVFSResult result;

  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
    result = gnome_vfs_set_file_info_uri(RVAL2GVFSURI(uri),
                                         RVAL2GVFSFILEINFO(info),
                                         RVAL2GFLAGS(mask, GNOME_VFS_TYPE_VFS_SET_FILE_INFO_MASK));
  } else {
    result = gnome_vfs_set_file_info(RVAL2CSTR(uri),
                                     RVAL2GVFSFILEINFO(info),
                                     RVAL2GFLAGS(mask, GNOME_VFS_TYPE_VFS_SET_FILE_INFO_MASK));
  }

  return GVFSRESULT2RVAL(result);
}

static VALUE
file_m_truncate(self, uri, length)
     VALUE self, uri, length;
{
  GnomeVFSResult result;

  rb_secure(2);
  /* XXX: how to do this?
   * SafeStringValue(uri);
   */
  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
    result = gnome_vfs_truncate_uri(RVAL2GVFSURI(uri),
                                    NUM2ULONG(length));
  } else {
    result = gnome_vfs_truncate(RVAL2CSTR(uri),
                                NUM2ULONG(length));
  }

  return GVFSRESULT2RVAL(result);
}

static VALUE
handle_gets(handle, sep, len)
     GnomeVFSHandle *handle;
     const char *sep;
     int len;
{
  guint8 buf[8192];
  guint8 *bp, *bpend;
  guint8 c;
  gboolean append;
  VALUE str;
  char last;
  GnomeVFSFileSize bytes_read;
  GnomeVFSResult result;

  bp = buf;
  bpend = buf + sizeof(buf) - 3;
  append = FALSE;
  str = Qnil;
  last = sep[len - 1];

  for ( ; ; ) {
    result = gnome_vfs_read(handle, &c, 1, &bytes_read);
    if (result == GNOME_VFS_OK) {
      *bp++ = c;

      if (c == last || bp == bpend) {
        if (append) {
          rb_str_cat(str, (const char*)buf, bp - buf);
        } else {
          str = rb_str_new((const char*)buf, bp - buf);
          append = TRUE;
        }

        if (len == 1 && c == *sep) {
          break;
        } else if (memcmp(RSTRING(str)->ptr +
                          RSTRING(str)->len - len,
                          sep, len) == 0) {
          break;
        } else {
          bp = buf;
        }
      }
    } else if (result == GNOME_VFS_ERROR_EOF) {
      if (bp - buf > 0) {
        if (append) {
          rb_str_cat(str, (const char*)buf, bp - buf);
        } else {
          str = rb_str_new((const char*)buf, bp - buf);
        }
      }

      break;
    } else {
      return GVFSRESULT2RVAL(result);
    }
  }

  if (!NIL_P(str)) {
    OBJ_TAINT(str);
  }
  return str;
}

static void
get_gets_separator(argc, argv, sep, len)
     int argc;
     VALUE *argv;
     char **sep;
     int *len;
{
  VALUE r_separator;

  if (rb_scan_args(argc, argv, "01", &r_separator) == 1) {
    Check_Type(r_separator, T_STRING);
    *sep = RSTRING(r_separator)->ptr;
    *len = RSTRING(r_separator)->len;
  } else {
    *sep = RSTRING(s_default_rsep)->ptr;
    *len = RSTRING(s_default_rsep)->len;
  }
}

static VALUE
file_initialize(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE uri, open_mode, exclusive, perm;
  GnomeVFSResult result;
  GnomeVFSHandle *handle;
  mode_t mask;

  argc = rb_scan_args(argc, argv, "13", &uri, &open_mode, &exclusive,
                      &perm);
  if (argc < 4) {
    mask = umask(0);
    umask(mask);
    perm = INT2FIX(0666 & ~mask);
  }
  if (argc < 3) {
    exclusive = Qfalse;
  }
  if (argc < 2) {
    open_mode = OPENMODE2RVAL(GNOME_VFS_OPEN_READ);
  }

  printf ("open_mode = %d\n", RVAL2OPENMODE(open_mode));
  if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
      result = gnome_vfs_open_uri(&handle,
                                  RVAL2GVFSURI(uri),
                                  RVAL2OPENMODE(open_mode));
      if (result == GNOME_VFS_ERROR_NOT_FOUND && 
          (RVAL2OPENMODE(open_mode) & GNOME_VFS_OPEN_WRITE)){
        result = gnome_vfs_create_uri(&handle,
                                      RVAL2GVFSURI(uri),
                                      RVAL2OPENMODE(open_mode),
                                      RTEST(exclusive),
                                      NUM2UINT(perm));
      }
  } else {
      result = gnome_vfs_open(&handle,
                              RVAL2CSTR(uri),
                              RVAL2OPENMODE(open_mode));
      if (result == GNOME_VFS_ERROR_NOT_FOUND && 
          (RVAL2OPENMODE(open_mode) & GNOME_VFS_OPEN_WRITE)){
        result = gnome_vfs_create(&handle,
                                  RVAL2CSTR(uri),
                                  RVAL2OPENMODE(open_mode),
                                  RTEST(exclusive),
                                  NUM2UINT(perm));
      }
  }

  RAISE_IF_ERROR(result);
  G_INITIALIZE(self, handle);
  return Qnil;
}

static VALUE
file_close(self)
     VALUE self;
{
  return GVFSRESULT2RVAL(gnome_vfs_close(_SELF(self)));
}

static VALUE
file_create_or_open(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE file;

  file = rb_funcall2(self, rb_intern("new"), argc, argv);
  if (rb_block_given_p()) {
    return rb_ensure(rb_yield, file, file_close, file);
  }
  return file;
}

static VALUE
file_each(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  char *sep;
  int len;
  VALUE str;
  GnomeVFSHandle *handle;

  get_gets_separator(argc, argv, &sep, &len);
  handle = _SELF(self);
  while (!NIL_P(str = handle_gets(handle, sep, len))) {
    rb_yield(str);
  }

  return self;
}

static VALUE
file_each_byte(self)
     VALUE self;
{
  guint8 c;
  GnomeVFSFileSize bytes_read;
  GnomeVFSHandle *handle;
  GnomeVFSResult result;

  handle = _SELF(self);
  for ( ; ; ) {
    result = gnome_vfs_read(handle, &c, 1, &bytes_read);
    if (result == GNOME_VFS_OK) {
      rb_yield(INT2FIX(c & 0xff));
    } else if (result == GNOME_VFS_ERROR_EOF) {
      break;
    } else {
      return GVFSRESULT2RVAL(result);
    }
  }

  return self;
}

static VALUE
file_getc(self)
     VALUE self;
{
  guint8 c;
  GnomeVFSFileSize bytes_read;
  GnomeVFSResult result;

  result = gnome_vfs_read(_SELF(self), &c, 1, &bytes_read);
  if (result == GNOME_VFS_OK) {
    return INT2FIX(c & 0xff);
  } else if (result == GNOME_VFS_ERROR_EOF) {
    return Qnil;
  } else {
    return GVFSRESULT2RVAL(result);
  }
}

static VALUE
file_gets(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  char *sep;
  int len;

  get_gets_separator(argc, argv, &sep, &len);
  return handle_gets(_SELF(self), sep, len);
}

static VALUE
file_tell(self)
     VALUE self;
{
  GnomeVFSResult result;
  GnomeVFSFileSize offset;

  result = gnome_vfs_tell(_SELF(self), &offset);
  return CHECK_RESULT(result, ULL2NUM(offset));
}

static VALUE
file_pos(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE offset;

  if (rb_scan_args(argc, argv, "01", &offset) == 1) {
    return GVFSRESULT2RVAL(gnome_vfs_seek(_SELF(self),
                                          GNOME_VFS_SEEK_START,
                                          OFFT2NUM(offset)));
  } else {
    return file_tell(self);
  }
}

static VALUE
file_putc(self, r_c)
     VALUE self, r_c;
{
  guint8 c;
  GnomeVFSFileSize bytes_written;

  c = NUM2UINT(r_c) & 0xff;
  return GVFSRESULT2RVAL(gnome_vfs_write(_SELF(self), &c, 1ULL, 
                                         &bytes_written));
}

static VALUE
file_write(self, str)
     VALUE self, str;
{
  GnomeVFSFileSize bytes_written;
  GnomeVFSResult result;

  if (TYPE(str) != T_STRING) {
    str = rb_obj_as_string(str);
  }

  if (RSTRING(str)->len == 0) {
    return INT2FIX(0);
  }

  result = gnome_vfs_write(_SELF(self), RSTRING(str)->ptr,
                           RSTRING(str)->len, &bytes_written);
  return CHECK_RESULT(result, ULL2NUM(bytes_written));
}

static GnomeVFSFileSize
remain_size(handle)
     GnomeVFSHandle* handle;
{
  GnomeVFSFileInfo *info;
  GnomeVFSFileSize offset;
  GnomeVFSResult result;
  GnomeVFSFileSize value = BUFSIZ;

  info = gnome_vfs_file_info_new();
  result = gnome_vfs_get_file_info_from_handle(handle, info,
                                               GNOME_VFS_FILE_INFO_DEFAULT);
  if (result == GNOME_VFS_OK) {
    result = gnome_vfs_tell(handle, &offset);
    if (result == GNOME_VFS_OK) {
      /* XXX: is the + 1 right? */
      value = info->size - offset + 1;
    }
  }

  gnome_vfs_file_info_unref(info);

  if (value > LONG_MAX)
      rb_raise(rb_eIOError, "file too big for single read");
  return value;
}

static VALUE
read_all(handle, siz, str)
     GnomeVFSHandle *handle;
     GnomeVFSFileSize siz;
     VALUE str;
{
  GnomeVFSFileSize bytes = 0;
  
  if (siz == 0) siz = BUFSIZ;
  if (NIL_P(str)) {
    str = rb_tainted_str_new(0, siz);
  } else {
    StringValue(str);
    rb_str_modify(str);
    rb_str_resize(str, siz);
  }
  
  for (;;) {
    GnomeVFSFileSize bytes_read;
    GnomeVFSResult result;
  
    result = gnome_vfs_read(handle,
                            RSTRING(str)->ptr + bytes,
                            siz - bytes,
                            &bytes_read);
    
    if (result == GNOME_VFS_OK) {
    	bytes += bytes_read;
        if (siz - bytes < BUFSIZ) {
          siz += BUFSIZ;
          rb_str_resize(str, siz);
    	}
    } else if (result == GNOME_VFS_ERROR_EOF) {
      rb_str_resize(str, bytes);
      break;
    } else {
      GVFSRESULT2RVAL(result);
    }
  }
  
  return str;
}

static VALUE
file_read(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE len, str;
  GnomeVFSHandle* handle = _SELF(self);
  GnomeVFSFileSize bytes;
  GnomeVFSFileSize bytes_read;
  GnomeVFSResult result;

  rb_scan_args(argc, argv, "02", &len, &str);
  if (NIL_P(len))
    return read_all(handle, remain_size(handle), str);

  bytes = NUM2INT(len);
  if (bytes < 0) {
    /* FIXME */
    rb_raise(rb_eArgError, "negative length %ld given", (long)bytes);
  }

  if (NIL_P(str)) {
    str = rb_tainted_str_new(0, bytes);
  } else {
    StringValue(str);
    rb_str_modify(str);
    rb_str_resize(str, bytes);
  }
  if (bytes == 0) {
    return str;
  }

  result = gnome_vfs_read(handle, RSTRING(str)->ptr, bytes,
                          &bytes_read);
  if (result == GNOME_VFS_OK) {
    RSTRING(str)->len = bytes_read;
    RSTRING(str)->ptr[bytes_read] = '\0';
    return str;
  } else if (result == GNOME_VFS_ERROR_EOF) {
    return Qnil;
  } else {
    return GVFSRESULT2RVAL(result);
  }
}

static VALUE
file_readchar(self)
     VALUE self;
{
  guint8 c;
  GnomeVFSResult result;
  GnomeVFSFileSize bytes_read;

  result = gnome_vfs_read(_SELF(self), &c, 1, &bytes_read);
  return CHECK_RESULT(result, INT2FIX(c));
}

static VALUE
file_readline(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  char *sep;
  int len;
  VALUE line;

  get_gets_separator(argc, argv, &sep, &len);
  line = handle_gets(_SELF(self), sep, len);
  return NIL_P(line) ? GVFSRESULT2RVAL(GNOME_VFS_ERROR_EOF) : line;
}

static VALUE
file_readlines(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  char *sep;
  int len;
  VALUE line, ary;
  GnomeVFSHandle *handle;

  ary = rb_ary_new();
  handle = _SELF(self);
  get_gets_separator(argc, argv, &sep, &len);
  while (!NIL_P(line = handle_gets(handle, sep, len))) {
    rb_ary_push(ary, line);
  }
  return ary;
}

static VALUE
file_rewind(self)
     VALUE self;
{
  return GVFSRESULT2RVAL(gnome_vfs_seek(_SELF(self),
                                        GNOME_VFS_SEEK_START, 0ULL));
}

static VALUE
file_seek(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE offset, r_whence;
  GnomeVFSSeekPosition whence;

  if (rb_scan_args(argc, argv, "11", &offset, &r_whence) == 2) {
    whence = RVAL2GENUM(r_whence, GNOME_VFS_TYPE_VFS_SEEK_POSITION);
  } else {
    whence = GNOME_VFS_SEEK_START;
  }

  return GVFSRESULT2RVAL(gnome_vfs_seek(_SELF(self), whence,
                                        OFFT2NUM(offset)));
}

static VALUE
file_stat(argc, argv, self)
     int argc;
     VALUE *argv;
     VALUE self;
{
  VALUE r_options;
  GnomeVFSFileInfo *info;
  GnomeVFSFileInfoOptions options;
  GnomeVFSResult result;

  if (rb_scan_args(argc, argv, "01", &r_options) == 1) {
    options = RVAL2GFLAGS(r_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
  } else {
    options = GNOME_VFS_FILE_INFO_DEFAULT;
  }

  info = gnome_vfs_file_info_new();
  result = gnome_vfs_get_file_info_from_handle(_SELF(self), info,
                                               options);
  return CHECK_RESULT(result, GVFSFILEINFO2RVAL(info));
}

static VALUE
file_truncate(self, length)
     VALUE self, length;
{
  rb_secure(2);
  return GVFSRESULT2RVAL(gnome_vfs_truncate_handle(_SELF(self),
                                                   NUM2ULL(length)));
}

void
Init_gnomevfs_file(m_gvfs)
     VALUE m_gvfs;
{
  s_default_rsep = rb_str_new2("\n");

  g_gvfs_file = G_DEF_CLASS(GNOMEVFS_TYPE_FILE, "File", m_gvfs);
  rb_include_module(g_gvfs_file, rb_mEnumerable);

  rb_define_singleton_method(g_gvfs_file, "create", file_create_or_open,
                             -1);
  rb_define_singleton_method(g_gvfs_file, "open", file_create_or_open,
                             -1);
  rb_define_singleton_method(g_gvfs_file, "unlink", file_unlink, 1);
  rb_define_singleton_method(g_gvfs_file, "delete", file_unlink, 1);
  rb_define_singleton_method(g_gvfs_file, "move", file_move, 2);
  rb_define_singleton_method(g_gvfs_file, "rename", file_move, 2);
  rb_define_singleton_method(g_gvfs_file, "chmod", file_m_chmod, -1);
  rb_define_singleton_method(g_gvfs_file, "chown", file_m_chown, -1);
  rb_define_singleton_method(g_gvfs_file, "create_symbolic_link",
                             create_symbolic_link, 2);
  rb_define_singleton_method(g_gvfs_file, "symlink",
                             create_symbolic_link, 2);
  rb_define_singleton_method(g_gvfs_file, "stat", file_m_stat, -1);
  rb_define_singleton_method(g_gvfs_file, "lstat", file_m_lstat, -1);
  rb_define_singleton_method(g_gvfs_file, "file_info", file_m_lstat, -1);
  rb_define_singleton_method(g_gvfs_file, "set_file_info",
                             file_m_set_stat, 3);
  rb_define_singleton_method(g_gvfs_file, "truncate", file_m_truncate,
                             2);

  rb_define_method(g_gvfs_file, "initialize", file_initialize, -1);
  rb_define_method(g_gvfs_file, "close", file_close, 0);
  rb_define_method(g_gvfs_file, "each", file_each, -1);
  rb_define_method(g_gvfs_file, "each_line", file_each, -1);
  rb_define_method(g_gvfs_file, "each_byte", file_each_byte, 0);
  rb_define_method(g_gvfs_file, "getc", file_getc, 0);
  rb_define_method(g_gvfs_file, "gets", file_gets, -1);
  rb_define_method(g_gvfs_file, "pos", file_pos, -1);
  rb_define_method(g_gvfs_file, "tell", file_tell, 0);
  rb_define_method(g_gvfs_file, "putc", file_putc, 1);
  rb_define_method(g_gvfs_file, "read", file_read, -1);
  rb_define_method(g_gvfs_file, "readchar", file_readchar, 0);
  rb_define_method(g_gvfs_file, "readline", file_readline, -1);
  rb_define_method(g_gvfs_file, "readlines", file_readlines, -1);
  rb_define_method(g_gvfs_file, "rewind", file_rewind, 0);
  rb_define_method(g_gvfs_file, "seek", file_seek, -1);
  rb_define_method(g_gvfs_file, "stat", file_stat, -1);
  rb_define_method(g_gvfs_file, "file_info", file_stat, -1);
  rb_define_method(g_gvfs_file, "truncate", file_truncate, 1);
  rb_define_method(g_gvfs_file, "write", file_write, 1);
  rb_define_method(g_gvfs_file, "printf", rb_io_printf, -1);
  rb_define_method(g_gvfs_file, "puts", rb_io_puts, -1);
  rb_define_method(g_gvfs_file, "print", rb_io_print, -1);
  rb_define_method(g_gvfs_file, "<<", rb_io_addstr, 1);

  G_DEF_CLASS(GNOME_VFS_TYPE_VFS_OPEN_MODE , "OpenMode", g_gvfs_file);
  G_DEF_CONSTANTS(g_gvfs_file, GNOME_VFS_TYPE_VFS_OPEN_MODE, "GNOME_VFS_");

  /* FIXME: GNOME_VFS_TYPE_VFS_SEEK_POSITION */
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/

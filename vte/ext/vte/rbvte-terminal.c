/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2011  Ruby-GNOME2 Project Team
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

#include "rbvte.h"

#include <stdarg.h>
#include <pwd.h>

#define RG_TARGET_NAMESPACE cTerminal
#define _SELF(s) (VTE_TERMINAL(RVAL2GOBJ(s)))

static ID id_new, id_call;

static char **
rval2cstrary(VALUE ary)
{
    long i, len;
    char **result;

    if (NIL_P(ary))
        return NULL;

    len = RARRAY_LEN(ary);
    result = ALLOC_N(char *, len + 1);
    for (i = 0; i < len; i++) {
        result[i] = g_strdup(RVAL2CSTR(RARRAY_PTR(ary)[i]));
    }
    result[i] = NULL;

    return result;
}

static void
free_cstrary(char **ary)
{
    long i;

    if (!ary) return;

    for (i = 0; ary[i] != NULL; i++) {
        g_free(ary[i]);
    }
    g_free(ary);
}

static VALUE
attrary2rval(GArray *attrs)
{
    long i, len;
    VALUE rb_attrs, rb_class;

    len = attrs->len;
    rb_attrs = rb_ary_new2(len);
    rb_class = rb_const_get(rb_const_get(rb_cObject, rb_intern("Vte")), rb_intern("CharAttributes"));

    for (i = 0; i < len; i++) {
        VteCharAttributes *attr;
        attr = &g_array_index(attrs, VteCharAttributes, i);
        rb_ary_push(rb_attrs, rb_funcall(rb_class, id_new, 6,
                                         LONG2NUM(attr->row),
                                         LONG2NUM(attr->column),
                                         COLOR2RVAL(&(attr->fore)),
                                         COLOR2RVAL(&(attr->back)),
                                         CBOOL2RVAL(attr->underline),
                                         CBOOL2RVAL(attr->strikethrough)));
    }

    return rb_attrs;
}

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, vte_terminal_new());
    return Qnil;
}

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
fork_command_default_argv(void)
{
    struct passwd *pwd;
    const char *shell = NULL;

    pwd = getpwuid(getuid());
    if (pwd != NULL)
        shell = pwd->pw_shell;
    if (shell == NULL)
        shell = g_getenv("SHELL") ? g_getenv("SHELL") : "/bin/sh";

    return rb_ary_new3(1, CSTR2RVAL(shell));
}

static VALUE
fork_command_full(int argc, VALUE *argv, VALUE self)
{
    VALUE options, rb_pty_flags, rb_working_directory, rb_command_argv, rb_envv, rb_spawn_flags;
    int pty_flags, spawn_flags;
    char *working_directory;
    char **command_argv;
    char **envv;
    GPid child_pid;
    gboolean result;
    GError *error = NULL;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "pty_flags", &rb_pty_flags,
                     "working_directory", &rb_working_directory,
                     "argv", &rb_command_argv,
                     "envv", &rb_envv,
                     "spawn_flags", &rb_spawn_flags,
                     NULL);
    pty_flags = NIL_P(rb_pty_flags) ?
                VTE_PTY_DEFAULT :
                RVAL2GFLAGS(rb_pty_flags, VTE_TYPE_PTY_FLAGS);
    working_directory = NIL_P(rb_working_directory) ? NULL : RVAL2CSTR(rb_working_directory);
    command_argv = rval2cstrary(NIL_P(rb_command_argv) ? fork_command_default_argv() : rb_command_argv);
    envv = rval2cstrary(rb_envv);
    spawn_flags = NIL_P(rb_spawn_flags) ?
                G_SPAWN_CHILD_INHERITS_STDIN | G_SPAWN_SEARCH_PATH :
                NUM2INT(rb_spawn_flags);

    result = vte_terminal_fork_command_full(_SELF(self),
                                            pty_flags,
                                            working_directory,
                                            command_argv,
                                            envv,
                                            spawn_flags,
                                            NULL,
                                            NULL,
                                            &child_pid,
                                            &error);
    free_cstrary(command_argv);
    free_cstrary(envv);
    if (error)
        RAISE_GERROR(error);

    return INT2NUM(child_pid);
}
#endif

static VALUE
rg_fork_command(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_command, rb_command_argv, rb_envv, rb_directory;
    VALUE lastlog, utmp, wtmp;
    char *command;
    char **command_argv;
    char **envv;
    char *directory;
    pid_t pid;

    rb_scan_args(argc, argv, "07", &rb_command, &rb_command_argv,
                 &rb_envv, &rb_directory, &lastlog, &utmp, &wtmp);

#if VTE_CHECK_VERSION(0, 26, 0)
    if (argc == 0 || TYPE(rb_command) == T_HASH)
        return fork_command_full(1, &rb_command, self);

    rb_warn("'fork_commad(command, argv, envv, directory, lastlog, utmp, wtmp)' style"
            " has been deprecated since version 0.26."
            " Use 'fork_commad(options = {})' style.");
#endif

    command = NIL_P(rb_command) ? NULL : RVAL2CSTR(rb_command);
    command_argv = rval2cstrary(rb_command_argv);
    envv = rval2cstrary(rb_envv);
    directory = NIL_P(rb_directory) ? NULL : RVAL2CSTR(rb_directory);
    pid = vte_terminal_fork_command(RVAL2TERM(self), command,
                                    command_argv, envv, directory,
                                    NIL_P(lastlog) ? TRUE : RVAL2CBOOL(lastlog),
                                    NIL_P(utmp) ? TRUE : RVAL2CBOOL(utmp),
                                    NIL_P(wtmp) ? TRUE : RVAL2CBOOL(wtmp));
    free_cstrary(command_argv);
    free_cstrary(envv);

    return INT2NUM(pid);
}

static VALUE
rg_fork_pty(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_envv, rb_directory, lastlog, utmp, wtmp;
    char **envv;
    char *directory;
    pid_t pid;

    rb_scan_args(argc, argv, "05", &rb_envv, &rb_directory,
                 &lastlog, &utmp, &wtmp);

    envv = rval2cstrary(rb_envv);
    directory = NIL_P(rb_directory) ? NULL : RVAL2CSTR(rb_directory);
    pid = vte_terminal_forkpty(RVAL2TERM(self), envv, directory,
                               NIL_P(lastlog) ? TRUE : RVAL2CBOOL(lastlog),
                               NIL_P(utmp) ? TRUE : RVAL2CBOOL(utmp),
                               NIL_P(wtmp) ? TRUE : RVAL2CBOOL(wtmp));
    free_cstrary(envv);

    return INT2NUM(pid);
}

static VALUE
rg_feed(VALUE self, VALUE data)
{
    glong length;

    length = RSTRING_LEN(data);

    if (length > 0) {
        vte_terminal_feed(RVAL2TERM(self), RSTRING_PTR(data), length);
    }

    return self;
}

static VALUE
rg_feed_child(VALUE self, VALUE data)
{
    glong length;

    length = RSTRING_LEN(data);

    if (length > 0) {
        vte_terminal_feed_child(RVAL2TERM(self), RSTRING_PTR(data), length);
    }

    return self;
}

static VALUE
rg_feed_child_binary(VALUE self, VALUE data)
{
    glong length;

    length = RSTRING_LEN(data);

    if (length > 0) {
        vte_terminal_feed_child_binary(RVAL2TERM(self),
                                       RSTRING_PTR(data), length);
    }

    return self;
}

static VALUE
rg_copy_clipboard(VALUE self)
{
    vte_terminal_copy_clipboard(RVAL2TERM(self));
    return self;
}

static VALUE
rg_paste_clipboard(VALUE self)
{
    vte_terminal_paste_clipboard(RVAL2TERM(self));
    return self;
}

static VALUE
rg_copy_primary(VALUE self)
{
    vte_terminal_copy_primary(RVAL2TERM(self));
    return self;
}

static VALUE
rg_paste_primary(VALUE self)
{
    vte_terminal_paste_primary(RVAL2TERM(self));
    return self;
}

static VALUE
rg_set_size(VALUE self, VALUE columns, VALUE rows)
{
    vte_terminal_set_size(RVAL2TERM(self), NUM2LONG(columns), NUM2LONG(rows));
    return self;
}

static VALUE
rg_set_audible_bell(VALUE self, VALUE is_audible)
{
    vte_terminal_set_audible_bell(RVAL2TERM(self), RVAL2CBOOL(is_audible));
    return self;
}

static VALUE
rg_audible_bell_p(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_audible_bell(RVAL2TERM(self)));
}

static VALUE
rg_set_visible_bell(VALUE self, VALUE is_visible)
{
    vte_terminal_set_visible_bell(RVAL2TERM(self), RVAL2CBOOL(is_visible));
    return self;
}

static VALUE
rg_visible_bell_p(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_visible_bell(RVAL2TERM(self)));
}

static VALUE
rg_set_scroll_background(VALUE self, VALUE scroll)
{
    vte_terminal_set_scroll_background(RVAL2TERM(self), RVAL2CBOOL(scroll));
    return self;
}

static VALUE
rg_set_scroll_on_output(VALUE self, VALUE scroll)
{
    vte_terminal_set_scroll_on_output(RVAL2TERM(self), RVAL2CBOOL(scroll));
    return self;
}

static VALUE
rg_set_scroll_on_keystroke(VALUE self, VALUE scroll)
{
    vte_terminal_set_scroll_on_keystroke(RVAL2TERM(self), RVAL2CBOOL(scroll));
    return self;
}

static VALUE
rg_set_color_dim(VALUE self, VALUE dim)
{
    vte_terminal_set_color_dim(RVAL2TERM(self), RVAL2COLOR(dim));
    return self;
}

static VALUE
rg_set_color_bold(VALUE self, VALUE bold)
{
    vte_terminal_set_color_bold(RVAL2TERM(self), RVAL2COLOR(bold));
    return self;
}

static VALUE
rg_set_color_foreground(VALUE self, VALUE foreground)
{
    vte_terminal_set_color_foreground(RVAL2TERM(self), RVAL2COLOR(foreground));
    return self;
}

static VALUE
rg_set_color_background(VALUE self, VALUE background)
{
    vte_terminal_set_color_background(RVAL2TERM(self), RVAL2COLOR(background));
    return self;
}

static VALUE
rg_set_color_cursor(VALUE self, VALUE cursor)
{
    vte_terminal_set_color_cursor(RVAL2TERM(self),
                                  NIL_P(cursor) ? NULL : RVAL2COLOR(cursor));
    return self;
}

static VALUE
rg_set_color_highlight(VALUE self, VALUE highlight)
{
    vte_terminal_set_color_highlight(RVAL2TERM(self),
                                     NIL_P(highlight) ?
                                       NULL : RVAL2COLOR(highlight));
    return self;
}

static VALUE
rg_set_colors(VALUE self, VALUE foreground, VALUE background,
                VALUE rb_palette)
{
    glong i, len;
    GdkColor *palette;

    len = RARRAY_LEN(rb_palette);

    if (!(len == 0 || len == 8 || len == 16 || len == 24)) {
        char *inspect;
        inspect = RVAL2CSTR(rb_palette);
        rb_raise(rb_eArgError, "palette size must be 0, 8, 16 or 24: %s",
                 inspect);
    }

    palette = ALLOCA_N(GdkColor, len);
    for (i = 0; i < len; i++) {
        GdkColor *color;
        color = RVAL2COLOR(RARRAY_PTR(rb_palette)[i]);
        palette[i] = *color;
    }

    vte_terminal_set_colors(RVAL2TERM(self), RVAL2COLOR(foreground),
                            RVAL2COLOR(background), palette, len);
    return self;
}

static VALUE
rg_set_default_colors(VALUE self)
{
    vte_terminal_set_default_colors(RVAL2TERM(self));
    return self;
}

static VALUE
rg_set_background_image(VALUE self, VALUE image_or_path)
{
    if (RVAL2CBOOL(rb_obj_is_kind_of(image_or_path, rb_cString))) {
        vte_terminal_set_background_image_file(RVAL2TERM(self),
                                               RVAL2CSTR(image_or_path));
    } else {
        vte_terminal_set_background_image(RVAL2TERM(self),
                                          RVAL2GOBJ(image_or_path));
    }

    return self;
}

static VALUE
rg_set_background_tint_color(VALUE self, VALUE color)
{
    vte_terminal_set_background_tint_color(RVAL2TERM(self), RVAL2COLOR(color));
    return self;
}

static VALUE
rg_set_background_saturation(VALUE self, VALUE saturation)
{
    vte_terminal_set_background_saturation(RVAL2TERM(self),
                                           NUM2DBL(saturation));
    return self;
}

static VALUE
rg_set_background_transparent(VALUE self, VALUE transparent)
{
    vte_terminal_set_background_transparent(RVAL2TERM(self),
                                            RVAL2CBOOL(transparent));
    return self;
}

static VALUE
rg_set_cursor_blinks(VALUE self, VALUE blink)
{
#if VTE_CHECK_VERSION(0, 18, 0)
    VteTerminalCursorBlinkMode mode;

    mode = RVAL2CBOOL(blink) ? VTE_CURSOR_BLINK_ON : VTE_CURSOR_BLINK_OFF;
    vte_terminal_set_cursor_blink_mode(RVAL2TERM(self), mode);
#else
    vte_terminal_set_cursor_blinks(RVAL2TERM(self), RVAL2CBOOL(blink));
#endif
    return self;
}

#if VTE_CHECK_VERSION(0, 18, 0)
static VALUE
rg_set_cursor_blink_mode(VALUE self, VALUE rb_mode)
{
    VteTerminalCursorBlinkMode mode;

    mode = RVAL2GENUM(rb_mode, VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE);
    vte_terminal_set_cursor_blink_mode(RVAL2TERM(self), mode);
    return self;
}

static VALUE
rg_cursor_blink_mode(VALUE self)
{
    VteTerminalCursorBlinkMode mode;

    mode = vte_terminal_get_cursor_blink_mode(RVAL2TERM(self));
    return GENUM2RVAL(mode, VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE);
}
#endif

#if VTE_CHECK_VERSION(0, 19, 1)
static VALUE
rg_set_cursor_shape(VALUE self, VALUE rb_shape)
{
    VteTerminalCursorShape shape;

    shape = RVAL2GENUM(rb_shape, VTE_TYPE_TERMINAL_CURSOR_SHAPE);
    vte_terminal_set_cursor_shape(RVAL2TERM(self), shape);
    return self;
}

static VALUE
rg_cursor_shape(VALUE self)
{
    VteTerminalCursorShape shape;

    shape = vte_terminal_get_cursor_shape(RVAL2TERM(self));
    return GENUM2RVAL(shape, VTE_TYPE_TERMINAL_CURSOR_SHAPE);
}

#if !VTE_CHECK_VERSION(0, 20, 0)
static VALUE
rg_pty(VALUE self)
{
    return INT2NUM(vte_terminal_get_pty(RVAL2TERM(self)));
}
#endif

static VALUE
rg_child_exit_status(VALUE self)
{
    return INT2NUM(vte_terminal_get_child_exit_status(RVAL2TERM(self)));
}
#endif

static VALUE
rg_set_scrollback_lines(VALUE self, VALUE lines)
{
    vte_terminal_set_scrollback_lines(RVAL2TERM(self), NUM2LONG(lines));
    return self;
}

static VALUE
rg_im_append_menuitems(VALUE self, VALUE menushell)
{
    vte_terminal_im_append_menuitems(RVAL2TERM(self), RVAL2GOBJ(menushell));
    return self;
}

static VALUE
rg_set_font(int argc, VALUE *argv, VALUE self)
{
    VALUE font_desc_or_name, rb_antialias;
    VteTerminalAntiAlias antialias = VTE_ANTI_ALIAS_USE_DEFAULT;
    VteTerminal *term;

    rb_scan_args(argc, argv, "11", &font_desc_or_name, &rb_antialias);

    term = RVAL2TERM(self);
    if (!NIL_P(rb_antialias))
        antialias = RVAL2AA(rb_antialias);

    if (rb_obj_is_kind_of(font_desc_or_name, rb_cString)) {
        char *name;
        name = RVAL2CSTR(font_desc_or_name);
        if (NIL_P(rb_antialias))
            vte_terminal_set_font_from_string(term, name);
        else
            vte_terminal_set_font_from_string_full(term, name, antialias);
    } else {
        PangoFontDescription *font_desc;
        font_desc = RVAL2PFD(font_desc_or_name);
        if (NIL_P(rb_antialias))
            vte_terminal_set_font(term, font_desc);
        else
            vte_terminal_set_font_full(term, font_desc, antialias);
    }

    return self;
}

static VALUE
rg_font(VALUE self)
{
    PangoFontDescription *font_desc;
    font_desc = (PangoFontDescription *)vte_terminal_get_font(RVAL2TERM(self));
    return PFD2RVAL(font_desc);
}

static VALUE
rg_using_xft_p(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_using_xft(RVAL2TERM(self)));
}

static VALUE
rg_set_allow_bold(VALUE self, VALUE allow_bold)
{
    vte_terminal_set_allow_bold(RVAL2TERM(self), RVAL2CBOOL(allow_bold));
    return self;
}

static VALUE
rg_allow_bold_p(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_allow_bold(RVAL2TERM(self)));
}

static VALUE
rg_has_selection_p(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_has_selection(RVAL2TERM(self)));
}

static VALUE
rg_set_word_chars(VALUE self, VALUE word_chars)
{
    vte_terminal_set_word_chars(RVAL2TERM(self),
                                NIL_P(word_chars) ?
                                  NULL : RVAL2CSTR(word_chars));
    return self;
}

static VALUE
rg_word_char_p(VALUE self, VALUE c)
{
    return CBOOL2RVAL(vte_terminal_is_word_char(RVAL2TERM(self), NUM2UINT(c)));
}

static VALUE
rg_set_backspace_binding(VALUE self, VALUE binding)
{
    vte_terminal_set_backspace_binding(RVAL2TERM(self), RVAL2EB(binding));
    return self;
}

static VALUE
rg_set_delete_binding(VALUE self, VALUE binding)
{
    vte_terminal_set_delete_binding(RVAL2TERM(self), RVAL2EB(binding));
    return self;
}

static VALUE
rg_mouse_autohide_p(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_mouse_autohide(RVAL2TERM(self)));
}

static VALUE
rg_set_mouse_autohide(VALUE self, VALUE setting)
{
    vte_terminal_set_mouse_autohide(RVAL2TERM(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_reset(VALUE self, VALUE full, VALUE clear_history)
{
    vte_terminal_reset(RVAL2TERM(self), RVAL2CBOOL(full),
                       RVAL2CBOOL(clear_history));
    return self;
}

static gboolean
term_is_selected_cb(VteTerminal *terminal, glong column, glong row,
                    gpointer data)
{
    gboolean result = TRUE;
    VALUE callback = (VALUE)data;

    if (!NIL_P(callback)) {
        VALUE rb_result;
        rb_result = rb_funcall(callback, id_call, 3, GOBJ2RVAL(terminal),
                               LONG2NUM(column), LONG2NUM(row));
        result = RVAL2CBOOL(rb_result);
    }

    return result;
}

static VALUE
rg_get_text(int argc, VALUE *argv, VALUE self)
{
    VALUE get_attrs, include_trailing_spaces, proc, rb_text;
    GArray *attrs = NULL;
    char *text;

    rb_scan_args(argc, argv, "02&", &get_attrs,
                 &include_trailing_spaces, &proc);

    if (get_attrs != Qfalse)
        attrs = g_array_new(FALSE, TRUE, sizeof(VteCharAttributes));

    if (RVAL2CBOOL(include_trailing_spaces)) {
        text = vte_terminal_get_text_include_trailing_spaces(
            RVAL2TERM(self), term_is_selected_cb, (gpointer)proc, attrs);
    } else {
        text = vte_terminal_get_text(RVAL2TERM(self), term_is_selected_cb,
                                     (gpointer)proc, attrs);
    }
    rb_text = CSTR2RVAL(text);
    free(text);

    if (attrs) {
        VALUE rb_attrs;
        rb_attrs = attrary2rval(attrs);
        g_array_free(attrs, TRUE);
        return rb_ary_new3(2, rb_text, rb_attrs);
    } else {
        return rb_text;
    }
}

static VALUE
rg_get_text_range(int argc, VALUE *argv, VALUE self)
{
    VALUE start_row, start_col, end_row, end_col, get_attrs, proc, rb_text;
    GArray *attrs = NULL;
    char *text;

    rb_scan_args(argc, argv, "41&", &start_row, &start_col,
                 &end_row, &end_col, &get_attrs, &proc);

    if (get_attrs != Qfalse)
        attrs = g_array_new(FALSE, TRUE, sizeof(VteCharAttributes));

    text = vte_terminal_get_text_range(RVAL2TERM(self),
                                       NUM2LONG(start_row),
                                       NUM2LONG(start_col),
                                       NUM2LONG(end_row),
                                       NUM2LONG(end_col),
                                       term_is_selected_cb,
                                       (gpointer)proc,
                                       attrs);
    rb_text = CSTR2RVAL(text);
    free(text);

    if (attrs) {
        VALUE rb_attrs;
        rb_attrs = attrary2rval(attrs);
        g_array_free(attrs, TRUE);
        return rb_ary_new3(2, rb_text, rb_attrs);
    } else {
        return rb_text;
    }
}

static VALUE
rg_cursor_position(VALUE self)
{
    glong column, row;
    vte_terminal_get_cursor_position(RVAL2TERM(self), &column, &row);
    return rb_ary_new3(2, LONG2NUM(column), LONG2NUM(row));
}

static VALUE
rg_match_clear_all(VALUE self)
{
    vte_terminal_match_clear_all(RVAL2TERM(self));
    return self;
}

static VALUE
rg_match_add(VALUE self, VALUE match)
{
    return INT2NUM(vte_terminal_match_add(RVAL2TERM(self), RVAL2CSTR(match)));
}

static VALUE
rg_match_set_cursor(VALUE self, VALUE tag, VALUE cursor)
{
    if (NIL_P(cursor) || RVAL2GTYPE(cursor) == GDK_TYPE_CURSOR) {
        vte_terminal_match_set_cursor(RVAL2TERM(self), NUM2INT(tag), RVAL2GOBJ(cursor));
    } else if (RVAL2GTYPE(cursor) == GDK_TYPE_CURSOR_TYPE) {
        vte_terminal_match_set_cursor_type(RVAL2TERM(self), NUM2INT(tag), RVAL2CT(cursor));
#if VTE_CHECK_VERSION(0, 17, 1)
    } else {
        vte_terminal_match_set_cursor_name(_SELF(self), NUM2INT(tag), RVAL2CSTR(cursor));
#endif
    }

    return self;
}

static VALUE
rg_match_remove(VALUE self, VALUE tag)
{
    vte_terminal_match_remove(RVAL2TERM(self), NUM2INT(tag));
    return self;
}

static VALUE
rg_match_check(VALUE self, VALUE column, VALUE row)
{
    char *string;
    int tag;

    string = vte_terminal_match_check(RVAL2TERM(self), NUM2LONG(column),
                                      NUM2LONG(row), &tag);
    if (string) {
        VALUE rb_string;
        rb_string = CSTR2RVAL(string);
        free(string);
        return rb_ary_new3(2, rb_string, INT2NUM(tag));
    } else {
        return Qnil;
    }
}

#if !VTE_CHECK_VERSION(0, 20, 0)
static VALUE
rg_set_emulation(VALUE self, VALUE emulation)
{
    vte_terminal_set_emulation(RVAL2TERM(self), RVAL2CSTR(emulation));
    return self;
}

static VALUE
rg_emulation(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_emulation(RVAL2TERM(self)));
}
#endif

static VALUE
rg_default_emulation(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_default_emulation(RVAL2TERM(self)));
}

#if !VTE_CHECK_VERSION(0, 20, 0)
static VALUE
rg_set_encoding(VALUE self, VALUE encoding)
{
    vte_terminal_set_encoding(RVAL2TERM(self), RVAL2CSTR(encoding));
    return self;
}

static VALUE
rg_encoding(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_encoding(RVAL2TERM(self)));
}
#endif

static VALUE
rg_status_line(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_status_line(RVAL2TERM(self)));
}

static VALUE
rg_padding(VALUE self)
{
    int xpad, ypad;
    vte_terminal_get_padding(RVAL2TERM(self), &xpad, &ypad);
    return rb_ary_new3(2, INT2NUM(xpad), INT2NUM(ypad));
}

#if !VTE_CHECK_VERSION(0, 20, 0)
static VALUE
rg_set_pty(VALUE self, VALUE pty_master)
{
    vte_terminal_set_pty(RVAL2TERM(self), NUM2INT(pty_master));
    return self;
}
#endif

static VALUE
rg_adjustment(VALUE self)
{
    return GOBJ2RVAL(vte_terminal_get_adjustment(RVAL2TERM(self)));
}

static VALUE
rg_char_width(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_width(RVAL2TERM(self)));
}

static VALUE
rg_char_height(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_height(RVAL2TERM(self)));
}

static VALUE
rg_char_descent(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_descent(RVAL2TERM(self)));
}

static VALUE
rg_char_ascent(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_ascent(RVAL2TERM(self)));
}

static VALUE
rg_row_count(VALUE self)
{
    return LONG2NUM(vte_terminal_get_row_count(RVAL2TERM(self)));
}

static VALUE
rg_column_count(VALUE self)
{
    return LONG2NUM(vte_terminal_get_column_count(RVAL2TERM(self)));
}

static VALUE
rg_window_title(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_window_title(RVAL2TERM(self)));
}

static VALUE
rg_icon_title(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_icon_title(RVAL2TERM(self)));
}

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
rg_pty_new(VALUE self, VALUE flags)
{
    VtePty *result;
    GError *error = NULL;

    result = vte_terminal_pty_new(_SELF(self), RVAL2GFLAGS(flags, VTE_TYPE_PTY_FLAGS), &error);
    if (error)
        RAISE_GERROR(error);

    return GOBJ2RVAL(result);
}
#endif

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
rg_search_find_next(VALUE self)
{
    gboolean result;

    result = vte_terminal_search_find_next(_SELF(self));

    return CBOOL2RVAL(result);
}
#endif

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
rg_search_find_previous(VALUE self)
{
    gboolean result;

    result = vte_terminal_search_find_previous(_SELF(self));

    return CBOOL2RVAL(result);
}
#endif

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
rg_search_get_wrap_around_p(VALUE self)
{
    gboolean result;

    result = vte_terminal_search_get_wrap_around(_SELF(self));

    return CBOOL2RVAL(result);
}
#endif

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
rg_search_set_wrap_around(VALUE self, VALUE wrap_around)
{
    vte_terminal_search_set_wrap_around(_SELF(self), RVAL2CBOOL(wrap_around));

    return self;
}
#endif

#if VTE_CHECK_VERSION(0, 16, 0)
static VALUE
rg_select_all(VALUE self)
{
    vte_terminal_select_all(_SELF(self));

    return self;
}
#endif

#if VTE_CHECK_VERSION(0, 16, 0)
static VALUE
rg_select_none(VALUE self)
{
    vte_terminal_select_none(_SELF(self));

    return self;
}
#endif

static VALUE
rg_set_opacity(VALUE self, VALUE opacity)
{
    vte_terminal_set_opacity(_SELF(self), NUM2UINT(opacity));

    return self;
}

#if VTE_CHECK_VERSION(0, 26, 0)
static VALUE
rg_watch_child(VALUE self, VALUE child_pid)
{
    vte_terminal_watch_child(_SELF(self), NUM2INT(child_pid));

    return self;
}
#endif

#if VTE_CHECK_VERSION(0, 24, 0)
static VALUE
rg_write_contents(int argc, VALUE *argv, VALUE self)
{
    VALUE stream, flags, rb_cancellable;
    GCancellable *cancellable;
    gboolean result;
    GError *error = NULL;

    rb_scan_args(argc, argv, "21", &stream, &flags, &rb_cancellable);
    cancellable = NIL_P(rb_cancellable) ? NULL : RVAL2GOBJ(rb_cancellable);

    result = vte_terminal_write_contents(_SELF(self),
                                        RVAL2GOBJ(stream),
                                        RVAL2GENUM(flags, VTE_TYPE_TERMINAL_WRITE_FLAGS),
                                        cancellable,
                                        &error);
    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}
#endif

void
Init_vte_terminal(VALUE mVte)
{
    VALUE RG_TARGET_NAMESPACE;

    id_new = rb_intern("new");
    id_call = rb_intern("call");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(VTE_TYPE_TERMINAL, "Terminal", mVte);

    G_DEF_CLASS(VTE_TYPE_TERMINAL_ERASE_BINDING, "EraseBinding", RG_TARGET_NAMESPACE);
#if VTE_CHECK_VERSION(0, 18, 0)
    G_DEF_CLASS(VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE, "CursorBlinkMode", RG_TARGET_NAMESPACE);
#endif
#if VTE_CHECK_VERSION(0, 19, 1)
    G_DEF_CLASS(VTE_TYPE_TERMINAL_CURSOR_SHAPE, "CursorShape", RG_TARGET_NAMESPACE);
#endif
#if VTE_CHECK_VERSION(0, 24, 0)
    G_DEF_CLASS(VTE_TYPE_TERMINAL_WRITE_FLAGS, "WriteFlags", RG_TARGET_NAMESPACE);
#endif

    RG_DEF_METHOD(initialize, 0);

    RG_DEF_METHOD(fork_command, -1);
    RG_DEF_METHOD(fork_pty, -1);

    RG_DEF_METHOD(feed, 1);
    RG_DEF_METHOD(feed_child, 1);
    RG_DEF_METHOD(feed_child_binary, 1);

    RG_DEF_METHOD(copy_clipboard, 0);
    RG_DEF_METHOD(paste_clipboard, 0);
    RG_DEF_METHOD(copy_primary, 0);
    RG_DEF_METHOD(paste_primary, 0);

    RG_DEF_METHOD(set_size, 2);

    RG_DEF_METHOD(set_audible_bell, 1);
    RG_DEF_METHOD_P(audible_bell, 0);
    RG_DEF_METHOD(set_visible_bell, 1);
    RG_DEF_METHOD_P(visible_bell, 0);

    RG_DEF_METHOD(set_scroll_background, 1);
    RG_DEF_METHOD(set_scroll_on_output, 1);
    RG_DEF_METHOD(set_scroll_on_keystroke, 1);

    RG_DEF_METHOD(set_color_dim, 1);
    RG_DEF_METHOD(set_color_bold, 1);
    RG_DEF_METHOD(set_color_foreground, 1);
    RG_DEF_METHOD(set_color_background, 1);
    RG_DEF_METHOD(set_color_cursor, 1);
    RG_DEF_METHOD(set_color_highlight, 1);
    RG_DEF_METHOD(set_colors, 3);
    RG_DEF_METHOD(set_default_colors, 0);
    RG_DEF_METHOD(set_background_image, 1);
    RG_DEF_METHOD(set_background_tint_color, 1);
    RG_DEF_METHOD(set_background_saturation, 1);
    RG_DEF_METHOD(set_background_transparent, 1);
    RG_DEF_METHOD(set_cursor_blinks, 1);
#if VTE_CHECK_VERSION(0, 18, 0)
    RG_DEF_METHOD(set_cursor_blink_mode, 1);
    RG_DEF_METHOD(cursor_blink_mode, 0);
#endif
#if VTE_CHECK_VERSION(0, 19, 1)
    RG_DEF_METHOD(set_cursor_shape, 1);
    RG_DEF_METHOD(cursor_shape, 0);
#if !VTE_CHECK_VERSION(0, 20, 0)
    RG_DEF_METHOD(pty, 0);
#endif
    RG_DEF_METHOD(child_exit_status, 0);
#endif
    RG_DEF_METHOD(set_scrollback_lines, 1);

    RG_DEF_METHOD(im_append_menuitems, 1);

    RG_DEF_METHOD(set_font, -1);
    RG_DEF_METHOD(font, 0);
    RG_DEF_METHOD_P(using_xft, 0);
    RG_DEF_METHOD(set_allow_bold, 1);
    RG_DEF_METHOD_P(allow_bold, 0);
    RG_DEF_METHOD_P(has_selection, 0);
    RG_DEF_ALIAS("have_selection?", "has_selection?");
    RG_DEF_METHOD(set_word_chars, 1);
    RG_DEF_METHOD_P(word_char, 1);
    RG_DEF_METHOD(set_backspace_binding, 1);
    RG_DEF_METHOD(set_delete_binding, 1);
    RG_DEF_METHOD_P(mouse_autohide, 0);
    RG_DEF_METHOD(set_mouse_autohide, 1);

    RG_DEF_METHOD(reset, 2);

    RG_DEF_METHOD(get_text, -1);
    RG_DEF_METHOD(get_text_range, -1);

    RG_DEF_METHOD(cursor_position, 0);

    RG_DEF_METHOD(match_clear_all, 0);
    RG_DEF_METHOD(match_add, 1);
    RG_DEF_METHOD(match_set_cursor, 2);
    RG_DEF_METHOD(match_remove, 1);
    RG_DEF_METHOD(match_check, 2);

#if !VTE_CHECK_VERSION(0, 20, 0)
    RG_DEF_METHOD(set_emulation, 1);
    RG_DEF_METHOD(emulation, 0);
#endif
    RG_DEF_METHOD(default_emulation, 0);

#if !VTE_CHECK_VERSION(0, 20, 0)
    RG_DEF_METHOD(set_encoding, 1);
    RG_DEF_METHOD(encoding, 0);
#endif

    RG_DEF_METHOD(status_line, 0);
    RG_DEF_METHOD(padding, 0);

#if !VTE_CHECK_VERSION(0, 20, 0)
    RG_DEF_METHOD(set_pty, 1);
#endif

    RG_DEF_METHOD(adjustment, 0);
    RG_DEF_METHOD(char_width, 0);
    RG_DEF_METHOD(char_height, 0);
    RG_DEF_METHOD(char_descent, 0);
    RG_DEF_METHOD(char_ascent, 0);
    RG_DEF_METHOD(row_count, 0);
    RG_DEF_METHOD(column_count, 0);
    RG_DEF_METHOD(window_title, 0);
    RG_DEF_METHOD(icon_title, 0);

#if VTE_CHECK_VERSION(0, 26, 0)
    RG_DEF_METHOD(pty_new, 1);
    RG_DEF_METHOD(search_find_next, 0);
    RG_DEF_METHOD(search_find_previous, 0);
    RG_DEF_METHOD_P(search_get_wrap_around, 0);
    RG_DEF_METHOD(search_set_wrap_around, 1);
    RG_DEF_ALIAS("search_wrap_around=", "search_set_wrap_around");
#endif
#if VTE_CHECK_VERSION(0, 16, 0)
    RG_DEF_METHOD(select_all, 0);
    RG_DEF_METHOD(select_none, 0);
#endif
    RG_DEF_METHOD(set_opacity, 1);
#if VTE_CHECK_VERSION(0, 26, 0)
    RG_DEF_METHOD(watch_child, 1);
#endif
#if VTE_CHECK_VERSION(0, 24, 0)
    RG_DEF_METHOD(write_contents, -1);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}

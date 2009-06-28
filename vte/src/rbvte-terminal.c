/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbvte-terminal.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:34 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbvte.h"

static VALUE cCharAttributes;
static ID id_new, id_call;
static ID id_row, id_column, id_fore, id_back, id_underline, id_strikethrough;


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
    VALUE rb_attrs;

    len = attrs->len;
    rb_attrs = rb_ary_new2(len);

    for (i = 0; i < len; i++) {
        VteCharAttributes *attr;
        attr = &g_array_index(attrs, VteCharAttributes, i);
        rb_ary_push(rb_attrs, rb_funcall(cCharAttributes, id_new, 6,
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
ca_initialize(VALUE self, VALUE row, VALUE column, VALUE fore, VALUE back,
              VALUE underline, VALUE strikethrough)
{
    rb_ivar_set(self, id_row, row);
    rb_ivar_set(self, id_column, column);
    rb_ivar_set(self, id_fore, fore);
    rb_ivar_set(self, id_back, back);
    rb_ivar_set(self, id_underline, underline);
    rb_ivar_set(self, id_strikethrough, strikethrough);
    return Qnil;
}

static VALUE
ca_get_underline(VALUE self)
{
    return rb_ivar_get(self, id_underline);
}

static VALUE
ca_get_strikethrough(VALUE self)
{
    return rb_ivar_get(self, id_strikethrough);
}



static VALUE
term_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, vte_terminal_new());
    return Qnil;
}

static VALUE
term_fork_command(int argc, VALUE *argv, VALUE self)
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
term_fork_pty(int argc, VALUE *argv, VALUE self)
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
term_feed(VALUE self, VALUE data)
{
    glong length;

    length = RSTRING_LEN(data);

    if (length > 0) {
        vte_terminal_feed(RVAL2TERM(self), RSTRING_PTR(data), length);
    }

    return Qnil;
}

static VALUE
term_feed_child(VALUE self, VALUE data)
{
    glong length;

    length = RSTRING_LEN(data);

    if (length > 0) {
        vte_terminal_feed_child(RVAL2TERM(self), RSTRING_PTR(data), length);
    }

    return Qnil;
}

static VALUE
term_feed_child_binary(VALUE self, VALUE data)
{
    glong length;

    length = RSTRING_LEN(data);

    if (length > 0) {
        vte_terminal_feed_child_binary(RVAL2TERM(self),
                                       RSTRING_PTR(data), length);
    }

    return Qnil;
}

static VALUE
term_copy_clipboard(VALUE self)
{
    vte_terminal_copy_clipboard(RVAL2TERM(self));
    return Qnil;
}

static VALUE
term_paste_clipboard(VALUE self)
{
    vte_terminal_paste_clipboard(RVAL2TERM(self));
    return Qnil;
}

static VALUE
term_copy_primary(VALUE self)
{
    vte_terminal_copy_primary(RVAL2TERM(self));
    return Qnil;
}

static VALUE
term_paste_primary(VALUE self)
{
    vte_terminal_paste_primary(RVAL2TERM(self));
    return Qnil;
}

static VALUE
term_set_size(VALUE self, VALUE columns, VALUE rows)
{
    vte_terminal_set_size(RVAL2TERM(self), NUM2LONG(columns), NUM2LONG(rows));
    return Qnil;
}

static VALUE
term_set_audible_bell(VALUE self, VALUE is_audible)
{
    vte_terminal_set_audible_bell(RVAL2TERM(self), RVAL2CBOOL(is_audible));
    return Qnil;
}

static VALUE
term_get_audible_bell(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_audible_bell(RVAL2TERM(self)));
}

static VALUE
term_set_visible_bell(VALUE self, VALUE is_visible)
{
    vte_terminal_set_visible_bell(RVAL2TERM(self), RVAL2CBOOL(is_visible));
    return Qnil;
}

static VALUE
term_get_visible_bell(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_visible_bell(RVAL2TERM(self)));
}

static VALUE
term_set_scroll_background(VALUE self, VALUE scroll)
{
    vte_terminal_set_scroll_background(RVAL2TERM(self), RVAL2CBOOL(scroll));
    return Qnil;
}

static VALUE
term_set_scroll_on_output(VALUE self, VALUE scroll)
{
    vte_terminal_set_scroll_on_output(RVAL2TERM(self), RVAL2CBOOL(scroll));
    return Qnil;
}

static VALUE
term_set_scroll_on_keystroke(VALUE self, VALUE scroll)
{
    vte_terminal_set_scroll_on_keystroke(RVAL2TERM(self), RVAL2CBOOL(scroll));
    return Qnil;
}

static VALUE
term_set_color_dim(VALUE self, VALUE dim)
{
    vte_terminal_set_color_dim(RVAL2TERM(self), RVAL2COLOR(dim));
    return Qnil;
}

static VALUE
term_set_color_bold(VALUE self, VALUE bold)
{
    vte_terminal_set_color_bold(RVAL2TERM(self), RVAL2COLOR(bold));
    return Qnil;
}

static VALUE
term_set_color_foreground(VALUE self, VALUE foreground)
{
    vte_terminal_set_color_foreground(RVAL2TERM(self), RVAL2COLOR(foreground));
    return Qnil;
}

static VALUE
term_set_color_background(VALUE self, VALUE background)
{
    vte_terminal_set_color_background(RVAL2TERM(self), RVAL2COLOR(background));
    return Qnil;
}

static VALUE
term_set_color_cursor(VALUE self, VALUE cursor)
{
    vte_terminal_set_color_cursor(RVAL2TERM(self),
                                  NIL_P(cursor) ? NULL : RVAL2COLOR(cursor));
    return Qnil;
}

static VALUE
term_set_color_highlight(VALUE self, VALUE highlight)
{
    vte_terminal_set_color_highlight(RVAL2TERM(self),
                                     NIL_P(highlight) ?
                                       NULL : RVAL2COLOR(highlight));
    return Qnil;
}

static VALUE
term_set_colors(VALUE self, VALUE foreground, VALUE background,
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
    return Qnil;
}

static VALUE
term_set_default_colors(VALUE self)
{
    vte_terminal_set_default_colors(RVAL2TERM(self));
    return Qnil;
}

static VALUE
term_set_background_image(VALUE self, VALUE image_or_path)
{
    if (RVAL2CBOOL(rb_obj_is_kind_of(image_or_path, rb_cString))) {
        vte_terminal_set_background_image_file(RVAL2TERM(self),
                                               RVAL2CSTR(image_or_path));
    } else {
        vte_terminal_set_background_image(RVAL2TERM(self),
                                          RVAL2GOBJ(image_or_path));
    }

    return Qnil;
}

static VALUE
term_set_background_tint_color(VALUE self, VALUE color)
{
    vte_terminal_set_background_tint_color(RVAL2TERM(self), RVAL2COLOR(color));
    return Qnil;
}

static VALUE
term_set_background_saturation(VALUE self, VALUE saturation)
{
    vte_terminal_set_background_saturation(RVAL2TERM(self),
                                           NUM2DBL(saturation));
    return Qnil;
}

static VALUE
term_set_background_transparent(VALUE self, VALUE transparent)
{
    vte_terminal_set_background_transparent(RVAL2TERM(self),
                                            RVAL2CBOOL(transparent));
    return Qnil;
}

static VALUE
term_set_cursor_blinks(VALUE self, VALUE blink)
{
#if VTE_CHECK_VERSION(0, 18, 0)
    VteTerminalCursorBlinkMode mode;

    mode = RVAL2CBOOL(blink) ? VTE_CURSOR_BLINK_ON : VTE_CURSOR_BLINK_OFF;
    vte_terminal_set_cursor_blink_mode(RVAL2TERM(self), mode);
#else
    vte_terminal_set_cursor_blinks(RVAL2TERM(self), RVAL2CBOOL(blink));
#endif
    return Qnil;
}

#if VTE_CHECK_VERSION(0, 18, 0)
static VALUE
term_set_cursor_blink_mode(VALUE self, VALUE rb_mode)
{
    VteTerminalCursorBlinkMode mode;

    mode = RVAL2GENUM(rb_mode, VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE);
    vte_terminal_set_cursor_blink_mode(RVAL2TERM(self), mode);
    return Qnil;
}

static VALUE
term_get_cursor_blink_mode(VALUE self)
{
    VteTerminalCursorBlinkMode mode;

    mode = vte_terminal_get_cursor_blink_mode(RVAL2TERM(self));
    return GENUM2RVAL(mode, VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE);
}
#endif

#if VTE_CHECK_VERSION(0, 19, 1)
static VALUE
term_set_cursor_shape(VALUE self, VALUE rb_shape)
{
    VteTerminalCursorShape shape;

    shape = RVAL2GENUM(rb_shape, VTE_TYPE_TERMINAL_CURSOR_SHAPE);
    vte_terminal_set_cursor_shape(RVAL2TERM(self), shape);
    return Qnil;
}

static VALUE
term_get_cursor_shape(VALUE self)
{
    VteTerminalCursorShape shape;

    shape = vte_terminal_get_cursor_shape(RVAL2TERM(self));
    return GENUM2RVAL(shape, VTE_TYPE_TERMINAL_CURSOR_SHAPE);
}
#endif

static VALUE
term_set_scrollback_lines(VALUE self, VALUE lines)
{
    vte_terminal_set_scrollback_lines(RVAL2TERM(self), NUM2LONG(lines));
    return Qnil;
}

static VALUE
term_im_append_menuitems(VALUE self, VALUE menushell)
{
    vte_terminal_im_append_menuitems(RVAL2TERM(self), RVAL2GOBJ(menushell));
    return Qnil;
}

static VALUE
term_set_font(int argc, VALUE *argv, VALUE self)
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

    return Qnil;
}

static VALUE
term_get_font(VALUE self)
{
    PangoFontDescription *font_desc;
    font_desc = (PangoFontDescription *)vte_terminal_get_font(RVAL2TERM(self));
    return PFD2RVAL(font_desc);
}

static VALUE
term_get_using_xft(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_using_xft(RVAL2TERM(self)));
}

static VALUE
term_set_allow_bold(VALUE self, VALUE allow_bold)
{
    vte_terminal_set_allow_bold(RVAL2TERM(self), RVAL2CBOOL(allow_bold));
    return Qnil;
}

static VALUE
term_get_allow_bold(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_allow_bold(RVAL2TERM(self)));
}

static VALUE
term_get_has_selection(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_has_selection(RVAL2TERM(self)));
}

static VALUE
term_set_word_chars(VALUE self, VALUE word_chars)
{
    vte_terminal_set_word_chars(RVAL2TERM(self),
                                NIL_P(word_chars) ?
                                  NULL : RVAL2CSTR(word_chars));
    return Qnil;
}

static VALUE
term_is_word_char(VALUE self, VALUE c)
{
    return CBOOL2RVAL(vte_terminal_is_word_char(RVAL2TERM(self), NUM2UINT(c)));
}

static VALUE
term_set_backspace_binding(VALUE self, VALUE binding)
{
    vte_terminal_set_backspace_binding(RVAL2TERM(self), RVAL2EB(binding));
    return Qnil;
}

static VALUE
term_set_delete_binding(VALUE self, VALUE binding)
{
    vte_terminal_set_delete_binding(RVAL2TERM(self), RVAL2EB(binding));
    return Qnil;
}

static VALUE
term_get_mouse_autohide(VALUE self)
{
    return CBOOL2RVAL(vte_terminal_get_mouse_autohide(RVAL2TERM(self)));
}

static VALUE
term_set_mouse_autohide(VALUE self, VALUE setting)
{
    vte_terminal_set_mouse_autohide(RVAL2TERM(self), RVAL2CBOOL(setting));
    return Qnil;
}

static VALUE
term_reset(VALUE self, VALUE full, VALUE clear_history)
{
    vte_terminal_reset(RVAL2TERM(self), RVAL2CBOOL(full),
                       RVAL2CBOOL(clear_history));
    return Qnil;
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
term_get_text(int argc, VALUE *argv, VALUE self)
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
term_get_text_range(int argc, VALUE *argv, VALUE self)
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
term_get_cursor_position(VALUE self)
{
    glong column, row;
    vte_terminal_get_cursor_position(RVAL2TERM(self), &column, &row);
    return rb_ary_new3(2, LONG2NUM(column), LONG2NUM(row));
}

static VALUE
term_match_clear_all(VALUE self)
{
    vte_terminal_match_clear_all(RVAL2TERM(self));
    return Qnil;
}

static VALUE
term_match_add(VALUE self, VALUE match)
{
    return INT2NUM(vte_terminal_match_add(RVAL2TERM(self), RVAL2CSTR(match)));
}

static VALUE
term_match_set_cursor(VALUE self, VALUE tag, VALUE cursor)
{
    vte_terminal_match_set_cursor(RVAL2TERM(self), NUM2INT(tag),
                                  RVAL2GOBJ(cursor));
    return Qnil;
}

static VALUE
term_match_set_cursor_type(VALUE self, VALUE tag, VALUE cursor_type)
{
    vte_terminal_match_set_cursor_type(RVAL2TERM(self), NUM2INT(tag),
                                       RVAL2CT(cursor_type));
    return Qnil;
}

static VALUE
term_match_remove(VALUE self, VALUE tag)
{
    vte_terminal_match_remove(RVAL2TERM(self), NUM2INT(tag));
    return Qnil;
}

static VALUE
term_match_check(VALUE self, VALUE column, VALUE row)
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

static VALUE
term_set_emulation(VALUE self, VALUE emulation)
{
    vte_terminal_set_emulation(RVAL2TERM(self), RVAL2CSTR(emulation));
    return Qnil;
}

static VALUE
term_get_emulation(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_emulation(RVAL2TERM(self)));
}

static VALUE
term_get_default_emulation(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_default_emulation(RVAL2TERM(self)));
}

static VALUE
term_set_encoding(VALUE self, VALUE encoding)
{
    vte_terminal_set_encoding(RVAL2TERM(self), RVAL2CSTR(encoding));
    return Qnil;
}

static VALUE
term_get_encoding(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_encoding(RVAL2TERM(self)));
}

static VALUE
term_get_status_line(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_status_line(RVAL2TERM(self)));
}

static VALUE
term_get_padding(VALUE self)
{
    int xpad, ypad;
    vte_terminal_get_padding(RVAL2TERM(self), &xpad, &ypad);
    return rb_ary_new3(2, INT2NUM(xpad), INT2NUM(ypad));
}

static VALUE
term_set_pty(VALUE self, VALUE pty_master)
{
    vte_terminal_set_pty(RVAL2TERM(self), NUM2INT(pty_master));
    return Qnil;
}

static VALUE
term_get_adjustment(VALUE self)
{
    return GOBJ2RVAL(vte_terminal_get_adjustment(RVAL2TERM(self)));
}

static VALUE
term_get_char_width(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_width(RVAL2TERM(self)));
}

static VALUE
term_get_char_height(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_height(RVAL2TERM(self)));
}

static VALUE
term_get_char_descent(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_descent(RVAL2TERM(self)));
}

static VALUE
term_get_char_ascent(VALUE self)
{
    return LONG2NUM(vte_terminal_get_char_ascent(RVAL2TERM(self)));
}

static VALUE
term_get_row_count(VALUE self)
{
    return LONG2NUM(vte_terminal_get_row_count(RVAL2TERM(self)));
}

static VALUE
term_get_column_count(VALUE self)
{
    return LONG2NUM(vte_terminal_get_column_count(RVAL2TERM(self)));
}

static VALUE
term_get_window_title(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_window_title(RVAL2TERM(self)));
}

static VALUE
term_get_icon_title(VALUE self)
{
    return CSTR2RVAL(vte_terminal_get_icon_title(RVAL2TERM(self)));
}


void
Init_vte_terminal(VALUE mVte)
{
    VALUE cTerminal, cTerminalEraseBinding, cTerminalAntiAlias;
#if VTE_CHECK_VERSION(0, 18, 0)
    VALUE cTerminalCursorBlinkMode;
#endif
#if VTE_CHECK_VERSION(0, 19, 1)
    VALUE cTerminalCursorShape;
#endif

    id_new = rb_intern("new");
    id_call = rb_intern("call");

    id_row = rb_intern("@row");
    id_column = rb_intern("@column");
    id_fore = rb_intern("@fore");
    id_back = rb_intern("@back");
    id_underline = rb_intern("@underline");
    id_strikethrough = rb_intern("@strikethrough");

    cTerminal = G_DEF_CLASS(VTE_TYPE_TERMINAL, "Terminal", mVte);
    cTerminalEraseBinding = G_DEF_CLASS(VTE_TYPE_TERMINAL_ERASE_BINDING,
                                        "TerminalEraseBinding", mVte);
#if VTE_CHECK_VERSION(0, 18, 0)
    cTerminalCursorBlinkMode = G_DEF_CLASS(VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE,
					   "TerminalCursorBlinkMode", mVte);
#endif
#if VTE_CHECK_VERSION(0, 19, 1)
    cTerminalCursorShape = G_DEF_CLASS(VTE_TYPE_TERMINAL_CURSOR_SHAPE,
				       "TerminalCursorShape", mVte);
#endif
    cTerminalAntiAlias = G_DEF_CLASS(VTE_TYPE_TERMINAL_ANTI_ALIAS,
                                     "TerminalAntiAlias", mVte);

    cCharAttributes = rb_define_class_under(mVte, "CharAttributes", rb_cObject);

    rb_define_method(cCharAttributes, "initialize", ca_initialize, 6);
    rb_attr(cCharAttributes, rb_intern("row"), TRUE, FALSE, TRUE);
    rb_attr(cCharAttributes, rb_intern("column"), TRUE, FALSE, TRUE);
    rb_attr(cCharAttributes, rb_intern("fore"), TRUE, FALSE, TRUE);
    rb_attr(cCharAttributes, rb_intern("back"), TRUE, FALSE, TRUE);
    rb_define_alias(cCharAttributes, "foreground", "fore");
    rb_define_alias(cCharAttributes, "background", "back");
    rb_define_method(cCharAttributes, "underline?", ca_get_underline, 0);
    rb_define_method(cCharAttributes, "strikethrough?",
                     ca_get_strikethrough, 0);


    rb_define_method(cTerminal, "initialize", term_initialize, 0);

    rb_define_method(cTerminal, "fork_command", term_fork_command, -1);
    rb_define_method(cTerminal, "fork_pty", term_fork_pty, -1);

    rb_define_method(cTerminal, "feed", term_feed, 1);
    rb_define_method(cTerminal, "feed_child", term_feed_child, 1);
    rb_define_method(cTerminal, "feed_child_binary", term_feed_child_binary, 1);

    rb_define_method(cTerminal, "copy_clipboard", term_copy_clipboard, 0);
    rb_define_method(cTerminal, "paste_clipboard", term_paste_clipboard, 0);
    rb_define_method(cTerminal, "copy_primary", term_copy_primary, 0);
    rb_define_method(cTerminal, "paste_primary", term_paste_primary, 0);

    rb_define_method(cTerminal, "set_size", term_set_size, 2);

    rb_define_method(cTerminal, "set_audible_bell", term_set_audible_bell, 1);
    rb_define_method(cTerminal, "audible_bell?", term_get_audible_bell, 0);
    rb_define_method(cTerminal, "set_visible_bell", term_set_visible_bell, 1);
    rb_define_method(cTerminal, "visible_bell?", term_get_visible_bell, 0);

    rb_define_method(cTerminal, "set_scroll_background",
                     term_set_scroll_background, 1);
    rb_define_method(cTerminal, "set_scroll_on_output",
                     term_set_scroll_on_output, 1);
    rb_define_method(cTerminal, "set_scroll_on_keystroke",
                     term_set_scroll_on_keystroke, 1);

    rb_define_method(cTerminal, "set_color_dim", term_set_color_dim, 1);
    rb_define_method(cTerminal, "set_color_bold", term_set_color_bold, 1);
    rb_define_method(cTerminal, "set_color_foreground",
                     term_set_color_foreground, 1);
    rb_define_method(cTerminal, "set_color_background",
                     term_set_color_background, 1);
    rb_define_method(cTerminal, "set_color_cursor", term_set_color_cursor, 1);
    rb_define_method(cTerminal, "set_color_highlight",
                     term_set_color_highlight, 1);
    rb_define_method(cTerminal, "set_colors", term_set_colors, 3);
    rb_define_method(cTerminal, "set_default_colors",
                     term_set_default_colors, 0);
    rb_define_method(cTerminal, "set_background_image",
                     term_set_background_image, 1);
    rb_define_method(cTerminal, "set_background_tint_color",
                     term_set_background_tint_color, 1);
    rb_define_method(cTerminal, "set_background_saturation",
                     term_set_background_saturation, 1);
    rb_define_method(cTerminal, "set_background_transparent",
                     term_set_background_transparent, 1);
    rb_define_method(cTerminal, "set_cursor_blinks", term_set_cursor_blinks, 1);
#if VTE_CHECK_VERSION(0, 18, 0)
    rb_define_method(cTerminal, "set_cursor_blink_mode",
		     term_set_cursor_blink_mode, 1);
    rb_define_method(cTerminal, "cursor_blink_mode",
		     term_get_cursor_blink_mode, 0);
#endif
#if VTE_CHECK_VERSION(0, 19, 1)
    rb_define_method(cTerminal, "set_cursor_shape", term_set_cursor_shape, 1);
    rb_define_method(cTerminal, "cursor_shape", term_get_cursor_shape, 0);
#endif
    rb_define_method(cTerminal, "set_scrollback_lines",
                     term_set_scrollback_lines, 1);

    rb_define_method(cTerminal, "im_append_menuitems",
                     term_im_append_menuitems, 1);

    rb_define_method(cTerminal, "set_font", term_set_font, -1);
    rb_define_method(cTerminal, "font", term_get_font, 0);
    rb_define_method(cTerminal, "using_xft?", term_get_using_xft, 0);
    rb_define_method(cTerminal, "set_allow_bold", term_set_allow_bold, 1);
    rb_define_method(cTerminal, "allow_bold?", term_get_allow_bold, 0);
    rb_define_method(cTerminal, "has_selection?", term_get_has_selection, 0);
    rb_define_alias(cTerminal, "have_selection?", "has_selection?");
    rb_define_method(cTerminal, "set_word_chars", term_set_word_chars, 1);
    rb_define_method(cTerminal, "word_char?", term_is_word_char, 1);
    rb_define_method(cTerminal, "set_backspace_binding",
                     term_set_backspace_binding, 1);
    rb_define_method(cTerminal, "set_delete_binding",
                     term_set_delete_binding, 1);
    rb_define_method(cTerminal, "mouse_autohide?", term_get_mouse_autohide, 0);
    rb_define_method(cTerminal, "set_mouse_autohide",
                     term_set_mouse_autohide, 1);

    rb_define_method(cTerminal, "reset", term_reset, 2);

    rb_define_method(cTerminal, "get_text", term_get_text, -1);
    rb_define_method(cTerminal, "get_text_range", term_get_text_range, -1);

    rb_define_method(cTerminal, "cursor_position", term_get_cursor_position, 0);

    rb_define_method(cTerminal, "match_clear_all", term_match_clear_all, 0);
    rb_define_method(cTerminal, "match_add", term_match_add, 1);
    rb_define_method(cTerminal, "match_set_cursor",
                     term_match_set_cursor, 2);
    rb_define_method(cTerminal, "match_set_cursor_type",
                     term_match_set_cursor_type, 2);
    rb_define_method(cTerminal, "match_remove", term_match_remove, 1);
    rb_define_method(cTerminal, "match_check", term_match_check, 2);

    rb_define_method(cTerminal, "set_emulation", term_set_emulation, 1);
    rb_define_method(cTerminal, "emulation", term_get_emulation, 0);
    rb_define_method(cTerminal, "default_emulation",
                     term_get_default_emulation, 0);

    rb_define_method(cTerminal, "set_encoding", term_set_encoding, 1);
    rb_define_method(cTerminal, "encoding", term_get_encoding, 0);

    rb_define_method(cTerminal, "status_line", term_get_status_line, 0);
    rb_define_method(cTerminal, "padding", term_get_padding, 0);

    rb_define_method(cTerminal, "set_pty", term_set_pty, 1);

    rb_define_method(cTerminal, "adjustment", term_get_adjustment, 0);
    rb_define_method(cTerminal, "char_width", term_get_char_width, 0);
    rb_define_method(cTerminal, "char_height", term_get_char_height, 0);
    rb_define_method(cTerminal, "char_descent", term_get_char_descent, 0);
    rb_define_method(cTerminal, "char_ascent", term_get_char_ascent, 0);
    rb_define_method(cTerminal, "row_count", term_get_row_count, 0);
    rb_define_method(cTerminal, "column_count", term_get_column_count, 0);
    rb_define_method(cTerminal, "window_title", term_get_window_title, 0);
    rb_define_method(cTerminal, "icon_title", term_get_icon_title, 0);

    G_DEF_SETTERS(cTerminal);
}

/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rbvlcprivate.h"

static libvlc_media_list_t *
vlc_media_list_copy(const libvlc_media_list_t *instance)
{
    libvlc_media_list_retain((libvlc_media_list_t *)instance);
    return (libvlc_media_list_t *)instance;
}

static void
vlc_media_list_free(libvlc_media_list_t *instance)
{
    libvlc_media_list_release(instance);
}

GType
vlc_media_list_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCMediaList",
                                                (GBoxedCopyFunc)vlc_media_list_copy,
                                                (GBoxedFreeFunc)vlc_media_list_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cMediaList
#define _SELF(self) (RVAL2VLCMEDIALIST(self))

static ID id_core;

/*
 * Create an empty media list.
 *
 * @param [VLC::Core] core the core instance
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE core;
    libvlc_media_list_t *instance;

    rb_scan_args(argc, argv, "01", &core);
    if (NIL_P(core))
        core = rb_funcall(GTYPE2CLASS(VLC_TYPE_CORE), rb_intern("new"), 0);

    instance = libvlc_media_list_new(RVAL2VLCCORE(core));
    if (!instance)
        rb_raise(rb_eArgError, "Invalid arguments.");
    rb_ivar_set(self, id_core, core);

    G_INITIALIZE(self, instance);
    vlc_media_list_free(instance);
    return Qnil;
}

static VALUE
rg_core(VALUE self)
{
    return rb_ivar_get(self, id_core);
}

static VALUE
get_media(VALUE self, VALUE media)
{
    if (TYPE(media) == T_HASH) {
        if (NIL_P(rb_hash_lookup(media, ID2SYM(id_core))))
            rb_hash_aset(media, ID2SYM(id_core), rg_core(self));
        return rb_funcall(GTYPE2CLASS(VLC_TYPE_MEDIA), rb_intern("new"), 1, media);
    } else {
        return media;
    }
}

/*
 * Associate media instance with this media list instance.
 * If another media instance was present it will be released.
 * The libvlc_media_list_lock should NOT be held upon entering this function.
 *
 * @param [VLC::Media, Hash] media the media instance or specify media Hash (see VLC::Media#initialize)
 * @return [VLC::Media] the media instance
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_set_media(VALUE self, VALUE media)
{
    media = get_media(self, media);
    libvlc_media_list_set_media(_SELF(self), RVAL2VLCMEDIA(media));
    return media;
}

/*
 * Get media instance from this media list instance. This action will increase
 * the refcount on the media instance.
 * The libvlc_media_list_lock should NOT be held upon entering this function.
 *
 * @return [VLC::Media] media instance
 * @todo fixme
 */
static VALUE
rg_media(VALUE self)
{
    return VLCMEDIA2RVAL(libvlc_media_list_media(_SELF(self)));
}

/*
 * List media instance in media list at a position
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @param [Integer] pos position in array
 * @return [VLC::Media] media instance at position or nil
 * @todo fixme
 */
static VALUE
rg_get_media(VALUE self, VALUE pos)
{
    /* TODO: need libvlc_media_release? */
    return VLCMEDIA2RVAL(libvlc_media_list_item_at_index(_SELF(self), NUM2INT(pos)));
}

/*
 * Add media instance to media list
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @param [VLC::Media, Hash] media the media instance or specify media Hash (see VLC::Media#initialize)
 * @return [VLC::Media] the media instance
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_add_media(VALUE self, VALUE media)
{
    media = get_media(self, media);
    if (libvlc_media_list_add_media(_SELF(self), RVAL2VLCMEDIA(media)))
        rb_raise(rb_eArgError, "Invalid arguments.");
    G_CHILD_ADD(self, media);
    return media;
}

/*
 * Insert media instance in media list on a position
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @param [VLC::Media, Hash] media the media instance or specify media Hash (see VLC::Media#initialize)
 * @param [Integer] pos position in array
 * @return [VLC::Media] the media instance
 * @raise [ArgumentError] Invalid or unsupported arguments
 * @todo fixme
 */
static VALUE
rg_insert_media(VALUE self, VALUE media, VALUE pos)
{
    media = get_media(self, media);
    if (libvlc_media_list_insert_media(_SELF(self), RVAL2VLCMEDIA(media), NUM2INT(pos)))
        rb_raise(rb_eArgError, "Invalid arguments.");
    G_CHILD_ADD(self, media);
    return media;
}

/*
 * Remove media instance from media list on a position
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @param [VLC::Media, Integer] media the media instance or position in array
 * @return [Boolean] true on success, false if the list is read-only or the item was not found
 * @todo fixme
 */
static VALUE
rg_remove_media(VALUE self, VALUE media)
{
    int pos;

    if (TYPE(media) == T_FIXNUM)
        pos = NUM2INT(media);
    else
        pos = libvlc_media_list_index_of_item(_SELF(self), RVAL2VLCMEDIA(media));

    return ZEROBOOL2RVAL(libvlc_media_list_remove_index(_SELF(self), pos));
}

/*
 * Get count on media list items
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @return [Integer] number of items in media list
 * @todo fixme
 */
static VALUE
rg_count(VALUE self)
{
    return INT2NUM(libvlc_media_list_count(_SELF(self)));
}

/*
 * Find index position of List media instance in media list.
 * Warning: the function will return the first matched position.
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @param [VLC::Media] media media instance
 * @return [Integer] position of media instance or nil if media not found
 * @todo fixme
 */
static VALUE
rg_index(VALUE self, VALUE media)
{
    int index = libvlc_media_list_index_of_item(_SELF(self), RVAL2VLCMEDIA(media));
    return (index < 0) ? Qnil : INT2NUM(index);
}

/*
 * This indicates if this media list is read-only from a user point of view
 *
 * @return [Boolean] true on readonly, false on readwrite
 * @todo fixme
 */
static VALUE
rg_readonly_p(VALUE self)
{
    return CBOOL2RVAL(libvlc_media_list_is_readonly(_SELF(self)));
}

/*
 * Release lock on media list items
 * The libvlc_media_list_lock should be held upon entering this function.
 *
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_unlock(VALUE self)
{
    libvlc_media_list_unlock(_SELF(self));
    return self;
}

/*
 * Get lock on media list items
 *
 * @yield
 * @return [self]
 * @todo fixme
 */
static VALUE
rg_lock(VALUE self)
{
    libvlc_media_list_lock(_SELF(self));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, rg_unlock, self);
    return self;
}

static VALUE
rg_attach_event(VALUE self, VALUE event_type)
{
    return em_attach_event(libvlc_media_list_event_manager(_SELF(self)), self, event_type);
}

static VALUE
rg_detach_event(VALUE self, VALUE event_type)
{
    return em_detach_event(libvlc_media_list_event_manager(_SELF(self)), self, event_type);
}

void
Init_vlc_medialist(VALUE mVLC)
{
    id_core = rb_intern("core");

    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_MEDIA_LIST, "MediaList", mVLC);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(core, 0);
    RG_DEF_METHOD(set_media, 1);
    RG_DEF_METHOD(media, 0);
    RG_DEF_METHOD(get_media, 1);
    RG_DEF_METHOD(add_media, 1);
    RG_DEF_METHOD(insert_media, 2);
    RG_DEF_METHOD(remove_media, 1);
    RG_DEF_METHOD(count, 0);
    RG_DEF_METHOD(index, 1);
    RG_DEF_METHOD_P(readonly, 0);
    RG_DEF_METHOD(unlock, 0);
    RG_DEF_METHOD(lock, 0);
    RG_DEF_METHOD(attach_event, 1);
    RG_DEF_METHOD(detach_event, 1);
}

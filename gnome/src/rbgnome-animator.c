/* $Id: rbgnome-animator.c,v 1.1 2002/05/19 12:39:24 mutoh Exp $ */

/* Gnome::Animator widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE gnoAnimator;

/* Create a new animator of the specified size.  */
static VALUE
animator_initialize(self, width, height)
    VALUE self, width, height;
{
    GtkWidget *animator;
    animator = gnome_animator_new_with_size((guint)NUM2INT(width),
					    (guint)NUM2INT(height));
    set_widget(self, animator);
    return Qnil;
}

/* Set/get loop type (See `GnomeAnimatorLoopType').  */
static VALUE
animator_set_loop_type(self, loop_type)
    VALUE self, loop_type;
{
    gnome_animator_set_loop_type(GNOME_ANIMATOR(get_widget(self)),
				 NUM2INT(loop_type));
    return self;
}

static VALUE
animator_get_loop_type(self)
    VALUE self;
{
    GnomeAnimatorLoopType loop_type;
    loop_type = gnome_animator_get_loop_type(GNOME_ANIMATOR(get_widget(self)));
    return INT2NUM(loop_type);
}

/* Set/get playback direction.  A positive value means "forwards"; a
   negative one means "backwards".  */
static VALUE
animator_set_playback_direction(self, playback_direction)
    VALUE self, playback_direction;
{
    gnome_animator_set_playback_direction(GNOME_ANIMATOR(get_widget(self)),
					  NUM2INT(playback_direction));
    return self;
}

static VALUE
animator_get_playback_direction(self)
    VALUE self;
{
    gint playback_direction;
    playback_direction = gnome_animator_get_playback_direction(
				GNOME_ANIMATOR(get_widget(self)));
    return INT2NUM(playback_direction);
}

/* Append a frame from `image' */
static VALUE
animator_append_frame_from_imlib(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE image, x_offset, y_offset, interval, width, height;
    gboolean result;
    GdkImlibImage* im;

    rb_scan_args(argc, argv, "42", &image, &x_offset, &y_offset,
		 &interval, &width, &height);
    Data_Get_Struct(image, GdkImlibImage, im);
    switch (argc) {
    case 4:
	/* rendering at its natural size.  */
	result = gnome_animator_append_frame_from_imlib(
			GNOME_ANIMATOR(get_widget(self)),
                        im,
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval));
	break;
    case 6:
	/* rendering at the specified size.  */
	result = gnome_animator_append_frame_from_imlib_at_size(
			GNOME_ANIMATOR(get_widget(self)),
                        im,
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval),
			(guint)NUM2INT(width),
			(guint)NUM2INT(height));
	break;
    default:	
	rb_raise(rb_eArgError, "wrong # of arguments");
    }
    return result?Qtrue:Qfalse;
}

/* Append a frame from file `name' */
static VALUE
animator_append_frame_from_file(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE name, x_offset, y_offset, interval, width, height;
    gboolean result;

    rb_scan_args(argc, argv, "42", &name, &x_offset, &y_offset,
		 &interval, &width, &height);
    switch (argc) {
    case 4:
	/* rendering at its natural size.  */
	result = gnome_animator_append_frame_from_file(
			GNOME_ANIMATOR(get_widget(self)),
                        STR2CSTR(name),
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval));
	break;
    case 6:
	/* rendering at the specified size.  */
	result = gnome_animator_append_frame_from_file_at_size(
			GNOME_ANIMATOR(get_widget(self)),
                        STR2CSTR(name),
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval),
			(guint)NUM2INT(width),
			(guint)NUM2INT(height));
	break;
    default:	
	rb_raise(rb_eArgError, "wrong # of arguments");
    }
    return result?Qtrue:Qfalse;
}

/* Crop `image' into frames `x_unit' pixels wide, and append them as
   frames to the animator with the specified `interval' and offsets.  */
static VALUE
animator_append_frames_from_imlib(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE image, x_offset, y_offset, interval, x_unit, width, height;
    gboolean result;
    GdkImlibImage* im;

    rb_scan_args(argc, argv, "52", &image, &x_offset, &y_offset,
		 &interval, &x_unit, &width, &height);
    Data_Get_Struct(image, GdkImlibImage, im);
    switch (argc) {
    case 5:
	/* Each frame is rendered at its natural size.  */
	result = gnome_animator_append_frames_from_imlib(
			GNOME_ANIMATOR(get_widget(self)),
                        im,
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval),
			NUM2INT(x_unit));
	break;
    case 7:
	/* Each frame is rendered at the specified size.  */
	result = gnome_animator_append_frames_from_imlib_at_size(
			GNOME_ANIMATOR(get_widget(self)),
                        im,
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval),
			NUM2INT(x_unit),
			(guint)NUM2INT(width),
			(guint)NUM2INT(height));
	break;
    default:	
	rb_raise(rb_eArgError, "wrong # of arguments");
    }
    return result?Qtrue:Qfalse;
}

/* Load an image from `name', crop it into frames `x_unit' pixels
   wide, and append them as frames to the animator.  */
static VALUE
animator_append_frames_from_file(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE name, x_offset, y_offset, interval, x_unit, width, height;
    gboolean result;

    rb_scan_args(argc, argv, "52", &name, &x_offset, &y_offset,
		 &interval, &x_unit, &width, &height);
    switch (argc) {
    case 5:
	/* Each frame is rendered at its natural size.  */
	result = gnome_animator_append_frames_from_file(
			GNOME_ANIMATOR(get_widget(self)),
                        STR2CSTR(name),
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval),
			NUM2INT(x_unit));
	break;
    case 7:
	/* Each frame is rendered at the specified size.  */
	result = gnome_animator_append_frames_from_file_at_size(
			GNOME_ANIMATOR(get_widget(self)),
                        STR2CSTR(name),
			NUM2INT(x_offset),
			NUM2INT(y_offset),
			(guint32)NUM2INT(interval),
			NUM2INT(x_unit),
			(guint)NUM2INT(width),
			(guint)NUM2INT(height));
	break;
    default:	
	rb_raise(rb_eArgError, "wrong # of arguments");
    }
    return result?Qtrue:Qfalse;
}

/* Append `pixmap' to animator's list of frames.  */
static VALUE
animator_append_frame_from_gnome_pixmap(
    self, pixmap, x_offset, y_offset, interval)
    VALUE self, pixmap, x_offset, y_offset, interval;
{
    gboolean result;
    result = gnome_animator_append_frame_from_gnome_pixmap (
		GNOME_ANIMATOR(get_widget(self)),
		GNOME_PIXMAP(get_widget(pixmap)),
		NUM2INT(x_offset),
		NUM2INT(y_offset),
		(guint32)NUM2INT(interval));
    return result?Qtrue:Qfalse;
}

/* Start playing the animation in the GTK loop.  */
static VALUE
animator_start(self)
    VALUE self;
{
    gnome_animator_start(GNOME_ANIMATOR(get_widget(self)));
    return self;
}

/* Stop playing the animation in the GTK loop.  */
static VALUE
animator_stop(self)
    VALUE self;
{
    gnome_animator_stop(GNOME_ANIMATOR(get_widget(self)));
    return self;
}

/* Advance the animation by `num' frames.  A positive value uses the
   specified playback direction; a negative one goes in the opposite
   direction.  If the loop type is `GNOME_ANIMATOR_LOOP_NONE' and the
   value causes the frame counter to overflow, `FALSE' is returned and
   the animator is stopped; otherwise, `TRUE' is returned.  */
static VALUE
animator_advance(self, num)
    VALUE self, num;
{
    gboolean result;
    result = gnome_animator_advance(GNOME_ANIMATOR(get_widget(self)),
				    NUM2INT(num));
    return result?Qtrue:Qfalse;
}

/* Set the number of current frame.  The result is immediately
   visible.  */
static VALUE
animator_goto_frame(self, frame_number)
    VALUE self, frame_number;
{
    gnome_animator_goto_frame(GNOME_ANIMATOR(get_widget(self)),
			      (guint)NUM2INT(frame_number));
    return self;
}

/* Get the number of current frame.  */
static VALUE
animator_get_current_frame_number(self)
    VALUE self;
{
    guint frame_number;
    frame_number = gnome_animator_get_current_frame_number(
			GNOME_ANIMATOR(get_widget(self)));
    return self;
}

/* Get the animator status.  */
static VALUE
animator_get_status(self)
    VALUE self;
{
    GnomeAnimatorStatus status;
    status = gnome_animator_get_status(GNOME_ANIMATOR(get_widget(self)));
    return INT2NUM(status);
}

/* Set/get speed factor (the higher, the faster: the `interval' value
   is divided by this factor before being used).  Default is 1.0.  */
static VALUE
animator_set_playback_speed(self, speed)
    VALUE self, speed;
{
    gnome_animator_set_playback_speed(GNOME_ANIMATOR(get_widget(self)),
				      NUM2DBL(speed));
    return self;
}

static VALUE
animator_get_playback_speed(self)
    VALUE self;
{
    gdouble speed;
    speed = gnome_animator_get_playback_speed(
	      GNOME_ANIMATOR(get_widget(self)));
    return rb_float_new(speed);
}

void
Init_gnome_animator()
{
    gnoAnimator = rb_define_class_under(mGnome, "Animator", gWidget);

    /* GnomeAnimatorStatus */
    rb_define_const(gnoAnimator, "STATUS_STOPPED",
		    INT2FIX(GNOME_ANIMATOR_STATUS_STOPPED));
    rb_define_const(gnoAnimator, "STATUS_RUNNING",
		    INT2FIX(GNOME_ANIMATOR_STATUS_RUNNING));

    /* GnomeAnimatorLoopType */
    rb_define_const(gnoAnimator, "LOOP_NONE",
		    INT2FIX(GNOME_ANIMATOR_LOOP_NONE));
    rb_define_const(gnoAnimator, "LOOP_RESTART",
		    INT2FIX(GNOME_ANIMATOR_LOOP_RESTART));
    rb_define_const(gnoAnimator, "LOOP_PING_PONG",
		    INT2FIX(GNOME_ANIMATOR_LOOP_PING_PONG));

    rb_define_method(gnoAnimator, "initialize", animator_initialize, 2);
    rb_define_method(gnoAnimator, "set_loop_type", animator_set_loop_type, 1);
    rb_define_method(gnoAnimator, "get_loop_type", animator_get_loop_type, 0);
    rb_define_method(gnoAnimator, "set_playback_direction",
		     animator_set_playback_direction, 1);
    rb_define_method(gnoAnimator, "get_playback_direction",
		     animator_get_playback_direction, 0);
    rb_define_method(gnoAnimator, "append_frame_from_imlib",
		     animator_append_frame_from_imlib, -1);
    rb_define_method(gnoAnimator, "append_frame_from_file",
		     animator_append_frame_from_file, -1);
    rb_define_method(gnoAnimator, "append_frames_from_imlib",
		     animator_append_frames_from_imlib, -1);
    rb_define_method(gnoAnimator, "append_frames_from_file",
		     animator_append_frames_from_file, -1);
    rb_define_method(gnoAnimator, "append_frame_from_gnome_pixmap",
		     animator_append_frame_from_gnome_pixmap, 4);
    rb_define_method(gnoAnimator, "start", animator_start, 0);
    rb_define_method(gnoAnimator, "stop", animator_stop, 0);
    rb_define_method(gnoAnimator, "advance", animator_advance, 1);
    rb_define_method(gnoAnimator, "goto_frame", animator_goto_frame, 1);
    rb_define_method(gnoAnimator, "get_current_frame_number",
		     animator_get_current_frame_number, 0);
    rb_define_method(gnoAnimator, "get_status", animator_get_status, 0);
    rb_define_method(gnoAnimator, "set_playback_speed",
		     animator_set_playback_speed, 1);
    rb_define_method(gnoAnimator, "get_playback_speed",
		     animator_get_playback_speed, 0);
}

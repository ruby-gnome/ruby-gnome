/*
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

/* Class: Gst::Message
 * Lightweight objects to signal the application of pipeline events.
 */

#define SELF(obj) (RVAL2GST_MSG(obj))
#define RVAL2GST_MSG_TYPE(type) RVAL2GFLAGS(type, GST_TYPE_MESSAGE_TYPE)
#define GST_MSG_TYPE2RVAL(type) GFLAGS2RVAL(type, GST_TYPE_MESSAGE_TYPE)

static RGFundamental fundamental;
static VALUE rb_cGstMessage;
static VALUE rb_cGstMessageUnknown;
static VALUE rb_cGstMessageEos;
static VALUE rb_cGstMessageError;
static VALUE rb_cGstMessageWarning;
static VALUE rb_cGstMessageInfo;
static VALUE rb_cGstMessageTag;
static VALUE rb_cGstMessageBuffering;
static VALUE rb_cGstMessageStateChanged;
static VALUE rb_cGstMessageStateDirty;
static VALUE rb_cGstMessageStepDone;
static VALUE rb_cGstMessageClockProvide;
static VALUE rb_cGstMessageClockLost;
static VALUE rb_cGstMessageNewClock;
static VALUE rb_cGstMessageStructureChange;
static VALUE rb_cGstMessageStreamStatus;
static VALUE rb_cGstMessageApplication;
static VALUE rb_cGstMessageElement;
static VALUE rb_cGstMessageSegmentStart;
static VALUE rb_cGstMessageSegmentDone;
static VALUE rb_cGstMessageDuration;
static VALUE rb_cGstMessageLatency;
static VALUE rb_cGstMessageAsyncStart;
static VALUE rb_cGstMessageAsyncDone;
static VALUE rb_cGstMessageAny;

static VALUE
get_superclass(void)
{
    return rb_cGstMiniObject;
}

static VALUE
instance2robj(gpointer instance)
{
    VALUE klass;
    GstMessage *message;

    message = instance;
    switch (GST_MESSAGE_TYPE(message)) {
      case GST_MESSAGE_UNKNOWN:
        klass = rb_cGstMessageUnknown;
        break;
      case GST_MESSAGE_EOS:
        klass = rb_cGstMessageEos;
        break;
      case GST_MESSAGE_ERROR:
        klass = rb_cGstMessageError;
        break;
      case GST_MESSAGE_WARNING:
        klass = rb_cGstMessageWarning;
        break;
      case GST_MESSAGE_INFO:
        klass = rb_cGstMessageInfo;
        break;
      case GST_MESSAGE_TAG:
        klass = rb_cGstMessageTag;
        break;
      case GST_MESSAGE_BUFFERING:
        klass = rb_cGstMessageBuffering;
        break;
      case GST_MESSAGE_STATE_CHANGED:
        klass = rb_cGstMessageStateChanged;
        break;
      case GST_MESSAGE_STATE_DIRTY:
        klass = rb_cGstMessageStateDirty;
        break;
      case GST_MESSAGE_STEP_DONE:
        klass = rb_cGstMessageStepDone;
        break;
      case GST_MESSAGE_CLOCK_PROVIDE:
        klass = rb_cGstMessageClockProvide;
        break;
      case GST_MESSAGE_CLOCK_LOST:
        klass = rb_cGstMessageClockLost;
        break;
      case GST_MESSAGE_NEW_CLOCK:
        klass = rb_cGstMessageNewClock;
        break;
      case GST_MESSAGE_STRUCTURE_CHANGE:
        klass = rb_cGstMessageStructureChange;
        break;
      case GST_MESSAGE_STREAM_STATUS:
        klass = rb_cGstMessageStreamStatus;
        break;
      case GST_MESSAGE_APPLICATION:
        klass = rb_cGstMessageApplication;
        break;
      case GST_MESSAGE_ELEMENT:
        klass = rb_cGstMessageElement;
        break;
      case GST_MESSAGE_SEGMENT_START:
        klass = rb_cGstMessageSegmentStart;
        break;
      case GST_MESSAGE_SEGMENT_DONE:
        klass = rb_cGstMessageSegmentDone;
        break;
      case GST_MESSAGE_DURATION:
        klass = rb_cGstMessageDuration;
        break;
      case GST_MESSAGE_LATENCY:
        klass = rb_cGstMessageLatency;
        break;
      case GST_MESSAGE_ASYNC_START:
        klass = rb_cGstMessageAsyncStart;
        break;
      case GST_MESSAGE_ASYNC_DONE:
        klass = rb_cGstMessageAsyncDone;
        break;
      case GST_MESSAGE_ANY:
        klass = rb_cGstMessageAny;
        break;
      default:
        klass = rb_cGstMessage;
        break;
    }
    gst_mini_object_ref(instance);
    return Data_Wrap_Struct(klass, NULL, rbgst_mini_object_free, instance);
}


static VALUE
initialize(VALUE self, VALUE type, VALUE src, VALUE structure)
{
    GstMessage *message;
    message = gst_message_new_custom(RVAL2GST_MSG_TYPE(type),
                                     RVAL2GST_OBJ(src),
                                     RVAL2GST_STRUCT(structure));
    G_INITIALIZE(self, message);
    return Qnil;
}

static VALUE
get_type(VALUE self)
{
    return GST_MSG_TYPE2RVAL(GST_MESSAGE_TYPE(SELF(self)));
}

static VALUE
set_type(VALUE self, VALUE type)
{
    GST_MESSAGE_TYPE(SELF(self)) = RVAL2GST_MSG_TYPE(type);
    return Qnil;
}

static VALUE
have_mutex_p(VALUE self)
{
    return GST_MESSAGE_GET_LOCK(SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
unlock(VALUE self)
{
    GST_MESSAGE_UNLOCK(SELF(self));
    return Qnil;
}

static VALUE
lock(VALUE self)
{
    GST_MESSAGE_LOCK(SELF(self));
    if (rb_block_given_p()) {
        rb_ensure(rb_yield, Qnil, unlock, self);
    }

    return Qnil;
}

static VALUE
have_cond_p(VALUE self)
{
    return GST_MESSAGE_COND(SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
cond_wait(VALUE self)
{
    GST_MESSAGE_WAIT(SELF(self));
    return Qnil;
}

static VALUE
cond_signal(VALUE self)
{
    GST_MESSAGE_SIGNAL(SELF(self));
    return Qnil;
}

static VALUE
get_timestamp(VALUE self)
{
    return ULL2NUM(GST_MESSAGE_TIMESTAMP(SELF(self)));
}

static VALUE
set_timestamp(VALUE self, VALUE timestamp)
{
    GST_MESSAGE_TIMESTAMP(SELF(self)) = NUM2ULL(timestamp);
    return Qnil;
}

static VALUE
valid_timestamp_p(VALUE self)
{
    gboolean valid;
    valid = GST_CLOCK_TIME_IS_VALID(GST_MESSAGE_TIMESTAMP(SELF(self)));
    return CBOOL2RVAL(valid);
}

static VALUE
get_source(VALUE self)
{
    return GOBJ2RVAL(GST_MESSAGE_SRC(SELF(self)));
}

static VALUE
set_source(VALUE self, VALUE source)
{
    GST_MESSAGE_SRC(SELF(self)) = RVAL2GST_OBJ(source);
    return Qnil;
}

static VALUE
get_structure(VALUE self)
{
    return GST_STRUCT2RVAL((gpointer)gst_message_get_structure(SELF(self)));
}


static VALUE
eos_initialize(VALUE self, VALUE src)
{
    G_INITIALIZE(self, gst_message_new_eos(RVAL2GST_OBJ(src)));
    return Qnil;
}

#if 0
static VALUE
error_initialize(VALUE self, VALUE src, VALUE error, VALUE debug)
{
    G_INITIALIZE(self, gst_message_new_error(RVAL2GST_OBJ(src),
                                             RVAL2GERROR(error), /* FIXME */
                                             RVAL2CSTR(debug)));
    return Qnil;
}
#endif

static VALUE
error_parse(VALUE self)
{
    VALUE result;
    GError *error;
    gchar *debug;

    gst_message_parse_error(SELF(self), &error, &debug);
    result = rb_ary_new3(2, rbgerr_gerror2exception(error), CSTR2RVAL2(debug));
    g_error_free(error);
    return result;
}

#if 0
static VALUE
warning_initialize(VALUE self, VALUE src, VALUE error, VALUE debug)
{
    G_INITIALIZE(self, gst_message_new_warning(RVAL2GST_OBJ(src),
                                               RVAL2GERROR(error), /* FIXME */
                                               RVAL2CSTR(debug)));
    return Qnil;
}
#endif

static VALUE
warning_parse(VALUE self)
{
    VALUE result;
    GError *error;
    gchar *debug;

    gst_message_parse_warning(SELF(self), &error, &debug);
    result = rb_ary_new3(2, rbgerr_gerror2exception(error), CSTR2RVAL2(debug));
    g_error_free(error);
    return result;
}

#if 0
static VALUE
info_initialize(VALUE self, VALUE src, VALUE error, VALUE debug)
{
    G_INITIALIZE(self, gst_message_new_info(RVAL2GST_OBJ(src),
                                            RVAL2GERROR(error), /* FIXME */
                                            RVAL2CSTR(debug)));
    return Qnil;
}
#endif

static VALUE
info_parse(VALUE self)
{
    VALUE result;
    GError *error;
    gchar *debug;

    gst_message_parse_info(SELF(self), &error, &debug);
    result = rb_ary_new3(2, rbgerr_gerror2exception(error), CSTR2RVAL2(debug));
    g_error_free(error);
    return result;
}

#if 0
static VALUE
tag_initialize(VALUE self, VALUE src, VALUE tag_list)
{
    G_INITIALIZE(self, gst_message_new_tag(RVAL2GST_OBJ(src),
                                           RVAL2GST_TAG_LIST(tag_list))); /* FIXME */
    return Qnil;
}
#endif

static VALUE
buffering_initialize(VALUE self, VALUE src, VALUE percent)
{
    G_INITIALIZE(self, gst_message_new_buffering(RVAL2GST_OBJ(src),
                                                 NUM2INT(percent)));
    return Qnil;
}

static VALUE
buffering_parse(VALUE self)
{
    gint percent;

    gst_message_parse_buffering(SELF(self), &percent);
    return INT2NUM(percent);
}

static VALUE
state_changed_initialize(VALUE self, VALUE src, VALUE old_state,
                         VALUE new_state, VALUE pending)
{
    G_INITIALIZE(self, gst_message_new_state_changed(RVAL2GST_OBJ(src),
                                                     RVAL2GST_STATE(old_state),
                                                     RVAL2GST_STATE(new_state),
                                                     RVAL2GST_STATE(pending)));
    return Qnil;
}

static VALUE
state_changed_parse(VALUE self)
{
    GstState old_state, new_state, pending_state;

    gst_message_parse_state_changed(SELF(self),
                                    &old_state, &new_state, &pending_state);
    return rb_ary_new3(3,
                       GST_STATE2RVAL(old_state),
                       GST_STATE2RVAL(new_state),
                       GST_STATE2RVAL(pending_state));
}

static VALUE
state_dirty_initialize(VALUE self, VALUE src)
{
    G_INITIALIZE(self, gst_message_new_state_dirty(RVAL2GST_OBJ(src)));
    return Qnil;
}

static VALUE
clock_provide_initialize(VALUE self, VALUE src, VALUE clock, VALUE ready)
{
    G_INITIALIZE(self, gst_message_new_clock_provide(RVAL2GST_OBJ(src),
                                                     RVAL2GST_CLOCK(clock),
                                                     RVAL2CBOOL(ready)));
    return Qnil;
}

static VALUE
clock_provide_parse(VALUE self)
{
    GstClock *clock;
    gboolean ready;

    gst_message_parse_clock_provide(SELF(self), &clock, &ready);
    return rb_ary_new3(2, GOBJ2RVAL(clock), CBOOL2RVAL(ready));
}

static VALUE
clock_lost_initialize(VALUE self, VALUE src, VALUE clock)
{
    G_INITIALIZE(self, gst_message_new_clock_lost(RVAL2GST_OBJ(src),
                                                  RVAL2GST_CLOCK(clock)));
    return Qnil;
}

static VALUE
clock_lost_parse(VALUE self)
{
    GstClock *clock;

    gst_message_parse_clock_lost(SELF(self), &clock);
    return GOBJ2RVAL(clock);
}

static VALUE
new_clock_initialize(VALUE self, VALUE src, VALUE clock)
{
    G_INITIALIZE(self, gst_message_new_new_clock(RVAL2GST_OBJ(src),
                                                 RVAL2GST_CLOCK(clock)));
    return Qnil;
}

static VALUE
new_clock_parse(VALUE self)
{
    GstClock *clock;

    gst_message_parse_new_clock(SELF(self), &clock);
    return GOBJ2RVAL(clock);
}

static VALUE
application_initialize(VALUE self, VALUE src, VALUE structure)
{
    G_INITIALIZE(self, gst_message_new_application(RVAL2GST_OBJ(src),
                                                   RVAL2GST_STRUCT(structure)));
    return Qnil;
}

static VALUE
element_initialize(VALUE self, VALUE src, VALUE structure)
{
    G_INITIALIZE(self, gst_message_new_element(RVAL2GST_OBJ(src),
                                               RVAL2GST_STRUCT(structure)));
    return Qnil;
}

static VALUE
segment_start_initialize(VALUE self, VALUE src, VALUE format, VALUE position)
{
    G_INITIALIZE(self, gst_message_new_segment_start(RVAL2GST_OBJ(src),
                                                     RVAL2GST_FORMAT(format),
                                                     NUM2LL(position)));
    return Qnil;
}

static VALUE
segment_start_parse(VALUE self)
{
    GstFormat format;
    gint64 position;

    gst_message_parse_segment_start(SELF(self), &format, &position);
    return rb_ary_new3(2, GST_FORMAT2RVAL(format), LL2NUM(position));
}

static VALUE
segment_done_initialize(VALUE self, VALUE src, VALUE format, VALUE position)
{
    G_INITIALIZE(self, gst_message_new_segment_done(RVAL2GST_OBJ(src),
                                                    RVAL2GST_FORMAT(format),
                                                    NUM2LL(position)));
    return Qnil;
}

static VALUE
segment_done_parse(VALUE self)
{
    GstFormat format;
    gint64 position;

    gst_message_parse_segment_done(SELF(self), &format, &position);
    return rb_ary_new3(2, GST_FORMAT2RVAL(format), LL2NUM(position));
}

static VALUE
duration_initialize(VALUE self, VALUE src, VALUE format, VALUE duration)
{
    G_INITIALIZE(self, gst_message_new_duration(RVAL2GST_OBJ(src),
                                                RVAL2GST_FORMAT(format),
                                                NUM2LL(duration)));
    return Qnil;
}

static VALUE
duration_parse(VALUE self)
{
    GstFormat format;
    gint64 duration;

    gst_message_parse_duration(SELF(self), &format, &duration);
    return rb_ary_new3(2, GST_FORMAT2RVAL(format), LL2NUM(duration));
}

static VALUE
async_start_initialize(VALUE self, VALUE src, VALUE new_base_time)
{
    G_INITIALIZE(self, gst_message_new_async_start(RVAL2GST_OBJ(src),
                                                   RVAL2CBOOL(new_base_time)));
    return Qnil;
}

static VALUE
async_start_parse(VALUE self)
{
    gboolean new_base_time;

    gst_message_parse_async_start(SELF(self), &new_base_time);
    return CBOOL2RVAL(new_base_time);
}

static VALUE
async_done_initialize(VALUE self, VALUE src)
{
    G_INITIALIZE(self, gst_message_new_async_done(RVAL2GST_OBJ(src)));
    return Qnil;
}

static VALUE
latency_initialize(VALUE self, VALUE src)
{
    G_INITIALIZE(self, gst_message_new_latency(RVAL2GST_OBJ(src)));
    return Qnil;
}


static VALUE
type_name(VALUE self)
{
    return CSTR2RVAL(gst_message_type_get_name(RVAL2GST_MSG_TYPE(self)));
}


void
Init_gst_message(void)
{
    VALUE rb_cGstMessageType;

    fundamental.type = GST_TYPE_MESSAGE;
    fundamental.get_superclass = get_superclass;
    fundamental.type_init_hook = rbgst_mini_object_type_init_hook;
    fundamental.rvalue2gvalue = NULL;
    fundamental.gvalue2rvalue = NULL;
    fundamental.initialize = rbgst_mini_object_initialize;
    fundamental.robj2instance = rbgst_mini_object_robj2instance;
    fundamental.instance2robj = instance2robj;
    fundamental.unref = rbgst_mini_object_unref;

    /* G_DEF_FUNDAMENTAL(&fundamental); */

    rb_cGstMessage = G_DEF_CLASS(GST_TYPE_MESSAGE, "Message", mGst);

#define DEFINE_MESSAGE(type)                                            \
    rb_cGstMessage ## type =                                            \
        rb_define_class_under(mGst,"Message" #type, rb_cGstMessage)

    DEFINE_MESSAGE(Unknown);
    DEFINE_MESSAGE(Eos);
    DEFINE_MESSAGE(Error);
    DEFINE_MESSAGE(Warning);
    DEFINE_MESSAGE(Info);
    DEFINE_MESSAGE(Tag);
    DEFINE_MESSAGE(Buffering);
    DEFINE_MESSAGE(StateChanged);
    DEFINE_MESSAGE(StateDirty);
    DEFINE_MESSAGE(StepDone);
    DEFINE_MESSAGE(ClockProvide);
    DEFINE_MESSAGE(ClockLost);
    DEFINE_MESSAGE(NewClock);
    DEFINE_MESSAGE(StructureChange);
    DEFINE_MESSAGE(StreamStatus);
    DEFINE_MESSAGE(Application);
    DEFINE_MESSAGE(Element);
    DEFINE_MESSAGE(SegmentStart);
    DEFINE_MESSAGE(SegmentDone);
    DEFINE_MESSAGE(Duration);
    DEFINE_MESSAGE(Latency);
    DEFINE_MESSAGE(AsyncStart);
    DEFINE_MESSAGE(AsyncDone);
    DEFINE_MESSAGE(Any);
#undef DEFINE_MESSAGE

    rb_cGstMessageType = G_DEF_CLASS(GST_TYPE_MESSAGE_TYPE,
                                     "Type", rb_cGstMessage);
    G_DEF_CONSTANTS(rb_cGstMessage, GST_TYPE_MESSAGE_TYPE, "GST_MESSAGE_");

    rb_define_method(rb_cGstMessage, "initialize", initialize, 3);

    rb_define_method(rb_cGstMessage, "type", get_type, 0);
    rb_define_method(rb_cGstMessage, "set_type", set_type, 1);

    rb_define_method(rb_cGstMessage, "have_mutex?", have_mutex_p, 0);
    rb_define_method(rb_cGstMessage, "lock", lock, 0);
    rb_define_method(rb_cGstMessage, "unlock", unlock, 0);
    rb_define_method(rb_cGstMessage, "have_cond?", have_cond_p, 0);
    rb_define_method(rb_cGstMessage, "wait", cond_wait, 0);
    rb_define_method(rb_cGstMessage, "signal", cond_signal, 0);

    rb_define_method(rb_cGstMessage, "timestamp", get_timestamp, 0);
    rb_define_method(rb_cGstMessage, "set_timestamp", set_timestamp, 1);
    rb_define_method(rb_cGstMessage, "valid_timestamp?", valid_timestamp_p, 0);

    rb_define_method(rb_cGstMessage, "source", get_source, 0);
    rb_define_method(rb_cGstMessage, "set_source", set_source, 1);
    rb_define_alias(rb_cGstMessage, "src", "source");
    rb_define_alias(rb_cGstMessage, "set_src", "set_source");

    rb_define_method(rb_cGstMessage, "structure", get_structure, 0);


    rb_define_method(rb_cGstMessageEos, "initialize", eos_initialize, 1);

#if 0
    rb_define_method(rb_cGstMessageError, "initialize", error_initialize, 3);
#endif
    rb_define_method(rb_cGstMessageError, "parse", error_parse, 0);

#if 0
    rb_define_method(rb_cGstMessageWarning, "initialize", warning_initialize, 3);
#endif
    rb_define_method(rb_cGstMessageWarning, "parse", warning_parse, 0);

#if 0
    rb_define_method(rb_cGstMessageInfo, "initialize", info_initialize, 3);
#endif
    rb_define_method(rb_cGstMessageInfo, "parse", info_parse, 0);

#if 0
    rb_define_method(rb_cGstMessageTag, "initialize", tag_initialize, 2);
    rb_define_method(rb_cGstMessageTag, "parse", tag_parse, 0);
#endif

    rb_define_method(rb_cGstMessageBuffering, "initialize",
                     buffering_initialize, 2);
    rb_define_method(rb_cGstMessageBuffering, "parse", buffering_parse, 0);

    rb_define_method(rb_cGstMessageStateChanged, "initialize",
                     state_changed_initialize, 4);
    rb_define_method(rb_cGstMessageStateChanged, "parse",
                     state_changed_parse, 0);

    rb_define_method(rb_cGstMessageStateDirty, "initialize",
                     state_dirty_initialize, 1);

    rb_define_method(rb_cGstMessageClockProvide, "initialize",
                     clock_provide_initialize, 3);
    rb_define_method(rb_cGstMessageClockProvide, "parse",
                     clock_provide_parse, 0);

    rb_define_method(rb_cGstMessageClockLost, "initialize",
                     clock_lost_initialize, 2);
    rb_define_method(rb_cGstMessageClockLost, "parse", clock_lost_parse, 0);

    rb_define_method(rb_cGstMessageNewClock, "initialize",
                     new_clock_initialize, 2);
    rb_define_method(rb_cGstMessageNewClock, "parse", new_clock_parse, 0);

    rb_define_method(rb_cGstMessageApplication, "initialize",
                     application_initialize, 2);

    rb_define_method(rb_cGstMessageElement, "initialize",
                     element_initialize, 2);

    rb_define_method(rb_cGstMessageSegmentStart, "initialize",
                     segment_start_initialize, 3);
    rb_define_method(rb_cGstMessageSegmentStart, "parse",
                     segment_start_parse, 0);

    rb_define_method(rb_cGstMessageSegmentDone, "initialize",
                     segment_done_initialize, 3);
    rb_define_method(rb_cGstMessageSegmentDone, "parse", segment_done_parse, 0);

    rb_define_method(rb_cGstMessageDuration, "initialize",
                     duration_initialize, 3);
    rb_define_method(rb_cGstMessageDuration, "parse", duration_parse, 0);

    rb_define_method(rb_cGstMessageAsyncStart, "initialize",
                     async_start_initialize, 2);
    rb_define_method(rb_cGstMessageAsyncStart, "parse", async_start_parse, 0);

    rb_define_method(rb_cGstMessageAsyncDone, "initialize",
                     async_done_initialize, 1);

    rb_define_method(rb_cGstMessageLatency, "initialize", latency_initialize, 1);


    rb_define_method(rb_cGstMessageType, "name", type_name, 0);


    G_DEF_SETTERS(rb_cGstMessage);
}

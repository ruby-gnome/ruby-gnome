#include "rbgutil.h"

typedef struct {
    VALUE obj;
} callback_requester;

static VALUE
touch_object(VALUE obj)
{
    rb_funcall(rb_cObject, rb_intern("p"), 1, obj);

    return Qnil;
}

static void
callback_requester_free(void *data)
{
    callback_requester *requester = (callback_requester *)data;

    if (!requester) {
        return;
    }

    rbgutil_invoke_callback_async(touch_object, requester->obj);
    xfree(requester);
}

static const rb_data_type_t callback_requester_type = {
    "CallbackRequester",
    {
        NULL, // Doesn't mark `obj` member for test purpose
        callback_requester_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
callback_requester_alloc(VALUE klass)
{
    callback_requester *requester;
    VALUE self = TypedData_Make_Struct(klass,
                                       callback_requester,
                                       &callback_requester_type,
                                       requester);
    requester->obj = Qnil;

    return self;
}

static VALUE
callback_requester_initialize(VALUE self, VALUE obj)
{
    callback_requester *requester;

    TypedData_Get_Struct(self,
                         callback_requester,
                         &callback_requester_type,
                         requester);
    requester->obj = obj;

    return Qnil;
}

void
Init_callback_requester(void)
{
    VALUE klass = rb_define_class("CallbackRequester", rb_cObject);

    rb_define_alloc_func(klass, callback_requester_alloc);
    rb_define_method(klass, "initialize", callback_requester_initialize, 1);

    rbgutil_start_callback_dispatch_thread();
}

/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  utils_int64.c -

  $Author: sakai $
  $Date: 2002/08/09 12:44:01 $

  Copyright (C) 2002  Masahiro Sakai
                      Kenichi Komiya

**********************************************************************/

#include <glib/gtypes.h>
#include "ruby.h"

#if 0
static ID id_and;
static ID id_rshift;
static ID id_lshift;
static ID id_lt;
static ID id_plus;
static ID id_uminus;
static ID id_abs;
#endif

typedef guint64 PRUint64;
typedef gint64 PRInt64;

#define max_PRUint32 G_MAXUINT

#define LL_ZERO 0
#define LL_UI2L(lhs,rhs) ((lhs)=(rhs))
#define LL_L2UI(lhs,rhs) ((lhs)=(rhs))
#define LL_SHL(lhs,v1,v2) ((lhs)=(v1)<<(v2))
#define LL_SHR(lhs,v1,v2) ((lhs)=(v1)>>(v2))
#define LL_ADD(lhs,v1,v2) ((lhs)=(v1)+(v2))
#define LL_NEG(lhs,rhs) ((lhs)=-(rhs))
#define LL_CMP(v1,op,v2) ((v1) op (v2))

//////////////////////////////////////////////////////////////////////////////
// following is ripped from rbXPCOM
// http://www.ruby-lang.org/en/raa-list.rhtml?name=rbXPCOM
// Copyright (C) 2001 Kenichi Komiya

static PRUint64
RubyTo64BitInt(VALUE aRuby)
{
    VALUE bitMask = max_PRUint32;
    VALUE lo = rb_funcall(aRuby, rb_intern("&"), 1, bitMask);
    VALUE hi = rb_funcall(aRuby, rb_intern(">>"), 1, INT2FIX(32));
    PRUint64 result, hi64, lo64;
    LL_UI2L(hi64, NUM2UINT(hi));
    LL_UI2L(lo64, NUM2UINT(lo));
    LL_SHL(result, hi64, 32);
    LL_ADD(result, result, lo64);
    return result;
}


static inline PRUint64
RubyToPRUint64(VALUE aRuby) 
{
    return RubyTo64BitInt(aRuby);
}


static PRInt64
RubyToPRInt64(VALUE aRuby) 
{
    if(RTEST(rb_funcall(aRuby, rb_intern("<"), 1, INT2FIX(0))))
    {
        VALUE absRuby = rb_funcall(aRuby, rb_intern("abs"), 0);
        PRInt64 result;
        LL_NEG(result, RubyTo64BitInt(absRuby));
        return result;
    } else
        return (PRInt64)RubyTo64BitInt(aRuby);

}

static VALUE
RubyFrom64BitInt(PRUint64 aNative)
{
    PRUint64 lo64, hi64;
    LL_L2UI(lo64, aNative);
    LL_SHR(hi64, aNative, 32);
    {
    VALUE lo = UINT2NUM(lo64);
    VALUE hi = UINT2NUM(hi64);
    VALUE hiRuby = rb_funcall(hi, rb_intern("<<"), 1, INT2FIX(32));
    return rb_funcall(hiRuby, rb_intern("+"), 1, lo);
    }
}

static inline VALUE
PRUint64ToRuby(PRUint64 aNative) 
{
    return RubyFrom64BitInt(aNative);
}

static VALUE
PRInt64ToRuby(PRInt64 aNative) 
{
    if(LL_CMP(aNative, <, LL_ZERO))
    {        
        PRUint64 abs64;
        LL_NEG(abs64, aNative);
        return rb_funcall(RubyFrom64BitInt(abs64), rb_intern("-@"), 0);
    }
    else
        return RubyFrom64BitInt((PRUint64)aNative);
}

// end of ripping
//////////////////////////////////////////////////////////////////////////////

VALUE
rbgobj_int64_to_num(guint64 val)
{
    return PRInt64ToRuby(val);
}

VALUE
rbgobj_uint64_to_num(guint64 val)
{
    return PRUint64ToRuby(val);
}

gint64
rbgobj_num_to_int64(VALUE val)
{
    return RubyToPRInt64(val);
}

guint64
rbgobj_num_to_uint64(VALUE val)
{
    return RubyToPRUint64(val);
}

//////////////////////////////////////////////////////////////////////////////

void
Init_utils_int64()
{
#if 0
    id_and    = rb_intern("&");
    id_rshift = rb_intern(">>");
    id_lshift = rb_intern(">>");
    id_lt     = rb_intern("<");
    id_plus   = rb_intern("+");
    id_uminus = rb_intern("-@");
    id_abs    = rb_intern("abs");
#endif
}

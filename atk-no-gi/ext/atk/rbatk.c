/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Masao Mutoh
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

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE mAtk
VALUE mAtk;

extern void Init_atk(void);

void
Init_atk(void)
{
    RG_TARGET_NAMESPACE = rb_define_module("Atk");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(ATK_MAJOR_VERSION),
                                INT2FIX(ATK_MINOR_VERSION),
                                INT2FIX(ATK_MICRO_VERSION)));

    Init_atk_action(RG_TARGET_NAMESPACE);
    Init_atk_component(RG_TARGET_NAMESPACE);
    Init_atk_document(RG_TARGET_NAMESPACE);
    Init_atk_editabletext(RG_TARGET_NAMESPACE);
    Init_atk_gobjectaccessible(RG_TARGET_NAMESPACE);
    Init_atk_hyperlink(RG_TARGET_NAMESPACE);
    Init_atk_hypertext(RG_TARGET_NAMESPACE);
    Init_atk_image(RG_TARGET_NAMESPACE);
    Init_atk_implementor(RG_TARGET_NAMESPACE);
    Init_atk_noopobject(RG_TARGET_NAMESPACE);
    Init_atk_noopobjectfactory(RG_TARGET_NAMESPACE);
    Init_atk_object(RG_TARGET_NAMESPACE);
    Init_atk_objectfactory(RG_TARGET_NAMESPACE);
    Init_atk_registry(RG_TARGET_NAMESPACE);
    Init_atk_relation(RG_TARGET_NAMESPACE);
    Init_atk_relation_set(RG_TARGET_NAMESPACE);
    Init_atk_selection(RG_TARGET_NAMESPACE);
    Init_atk_state(RG_TARGET_NAMESPACE);
    Init_atk_state_set(RG_TARGET_NAMESPACE);
    Init_atk_streamable_content(RG_TARGET_NAMESPACE);
    Init_atk_table(RG_TARGET_NAMESPACE);
    Init_atk_text(RG_TARGET_NAMESPACE);
    Init_atk_text_range(RG_TARGET_NAMESPACE);
    Init_atk_text_rectangle(RG_TARGET_NAMESPACE);
    Init_atk_util(RG_TARGET_NAMESPACE);
    Init_atk_value(RG_TARGET_NAMESPACE);
}

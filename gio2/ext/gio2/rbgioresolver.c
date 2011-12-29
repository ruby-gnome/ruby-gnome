/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cResolver
#define _SELF(value) RVAL2GRESOLVER(value)

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL_UNREF(g_resolver_get_default());
}

static VALUE
rg_s_set_default(G_GNUC_UNUSED VALUE self, VALUE resolver)
{
        g_resolver_set_default(_SELF(resolver));

        return self;
}

static VALUE
rg_lookup_by_name(VALUE self, VALUE hostname, VALUE cancellable)
{
        GError *error = NULL;
        GList *addresses;

        addresses = g_resolver_lookup_by_name(_SELF(self),
                                              RVAL2CSTR(hostname),
                                              RVAL2GCANCELLABLE(cancellable),
                                              &error);
        if (addresses == NULL)
                rbgio_raise_error(error);

        return GLIST2ARY_UNREF_FREE(addresses);
}

static VALUE
rg_lookup_by_name_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbhostname, rbcancellable, block;
        const gchar *hostname;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "11&", &hostname, &rbcancellable, &block);
        hostname = RVAL2CSTR(rbhostname);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_resolver_lookup_by_name_async(_SELF(self),
                                        hostname,
                                        cancellable,
                                        rbgio_async_ready_callback,
                                        (gpointer)block);

        return self;
}

static VALUE
rg_lookup_by_name_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GList *addresses;

        addresses = g_resolver_lookup_by_name_finish(_SELF(self),
                                                     RVAL2GASYNCRESULT(result),
                                                     &error);
        if (addresses == NULL)
                rbgio_raise_error(error);

        return GLIST2ARY_UNREF_FREE(addresses);
}

static VALUE
rg_lookup_by_address(int argc, VALUE *argv, VALUE self)
{
        VALUE address, cancellable;
        GError *error = NULL;
        gchar *hostname;

        rb_scan_args(argc, argv, "11", &address, &cancellable);
        hostname = g_resolver_lookup_by_address(_SELF(self),
                                                RVAL2GINETADDRESS(address),
                                                RVAL2GCANCELLABLE(cancellable),
                                                &error);
        if (hostname == NULL)
                rbgio_raise_error(error);

        return CSTR2RVAL_FREE(hostname);
}

static VALUE
rg_lookup_by_address_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbaddress, rbcancellable, block;
        GInetAddress *address;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "11&", &rbaddress, &rbcancellable, &block);
        address = RVAL2GINETADDRESS(rbaddress);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_resolver_lookup_by_address_async(_SELF(self),
                                           address,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

static VALUE
rg_lookup_by_address_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gchar *hostname;

        hostname = g_resolver_lookup_by_address_finish(_SELF(self),
                                                       RVAL2GASYNCRESULT(result),
                                                       &error);
        if (hostname == NULL)
                rbgio_raise_error(error);

        return CSTR2RVAL_FREE(hostname);
}

static VALUE
rg_lookup_service(int argc, VALUE *argv, VALUE self)
{
        VALUE service, protocol, domain, cancellable;
        GError *error = NULL;
        GList *targets;

        rb_scan_args(argc, argv, "31", &service, &protocol, &domain, &cancellable);
        targets = g_resolver_lookup_service(_SELF(self),
                                            RVAL2CSTR(service),
                                            RVAL2CSTR(protocol),
                                            RVAL2CSTR(domain),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error);
        if (targets == NULL)
                rbgio_raise_error(error);

        return GLIST2ARY_UNREF_FREE(targets);
}

static VALUE
rg_lookup_service_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbservice, rbprotocol, rbdomain, rbcancellable, block;
        const gchar *service;
        const gchar *protocol;
        const gchar *domain;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "31&", &rbservice, &rbprotocol, &rbdomain, &rbcancellable, &block);
        service = RVAL2CSTR(rbservice);
        protocol = RVAL2CSTR(rbprotocol);
        domain = RVAL2CSTR(rbdomain);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_resolver_lookup_service_async(_SELF(self),
                                        service,
                                        protocol,
                                        domain,
                                        cancellable,
                                        rbgio_async_ready_callback,
                                        (gpointer)block);

        return self;
}

static VALUE
rg_lookup_service_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GList *targets;

        targets = g_resolver_lookup_service_finish(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (targets == NULL)
                rbgio_raise_error(error);

        return GLIST2ARY_UNREF_FREE(targets);
}

void
Init_gresolver(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE, error;

        RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_RESOLVER, "Resolver", mGio);

        RG_DEF_SMETHOD(default, 0);
        RG_DEF_SMETHOD(set_default, 1);

        /* TODO: Taint result of these methods? */
        RG_DEF_METHOD(lookup_by_name, 2);
        RG_DEF_METHOD(lookup_by_name_async, -1);
        RG_DEF_METHOD(lookup_by_name_finish, 1);
        RG_DEF_METHOD(lookup_by_address, -1);
        RG_DEF_METHOD(lookup_by_address_async, -1);
        RG_DEF_METHOD(lookup_by_address_finish, 1);
        RG_DEF_METHOD(lookup_service, -1);
        RG_DEF_METHOD(lookup_service_async, -1);
        RG_DEF_METHOD(lookup_service_finish, 1);

        error = rbgio_define_domain_error(RG_TARGET_NAMESPACE, "Error", G_RESOLVER_ERROR, "GResolverErrorEnum", rb_eIOError);

        rbgio_define_error(RG_TARGET_NAMESPACE, "NotFoundError", G_RESOLVER_ERROR_NOT_FOUND, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "TemporaryFailureError", G_RESOLVER_ERROR_TEMPORARY_FAILURE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "InternalError", G_RESOLVER_ERROR_INTERNAL, error);
}

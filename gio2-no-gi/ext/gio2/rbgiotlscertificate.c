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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cTlsCertificate
#define _SELF(value) RVAL2GOBJ(value)

static VALUE
rg_initialize(VALUE self, VALUE data)
{
    GTlsCertificate *certificate;
    GError *error = NULL;

    certificate = g_tls_certificate_new_from_pem(StringValuePtr(data),
						 RSTRING_LEN(data),
						 &error);
    if (error)
	rbgio_raise_error(error);

    G_INITIALIZE(self, certificate);

    return Qnil;
}

static VALUE
rg_s_load(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE cert_file, key_file;
    GError *error = NULL;
    GTlsCertificate *certificate;

    rb_scan_args(argc, argv, "11", &cert_file, &key_file);
    if (NIL_P(key_file)) {
	certificate = g_tls_certificate_new_from_file(StringValuePtr(cert_file),
						      &error);
    } else {
	certificate = g_tls_certificate_new_from_files(StringValuePtr(cert_file),
						       StringValuePtr(key_file),
						       &error);
    }

    if (error)
	rbgio_raise_error(error);

    return GOBJ2RVAL_UNREF(certificate);
}

static VALUE
rg_verify(VALUE self, VALUE identity, VALUE trusted_ca)
{
    GTlsCertificateFlags flags;

    flags = g_tls_certificate_verify(_SELF(self),
				     RVAL2GOBJ(identity),
				     RVAL2GOBJ(trusted_ca));
    return GFLAGS2RVAL(flags, G_TYPE_TLS_CERTIFICATE_FLAGS);
}

void
Init_gtlscertificate(VALUE mGio)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_TLS_CERTIFICATE,
					    "TlsCertificate", mGio);

    G_DEF_CLASS(G_TYPE_TLS_CERTIFICATE_FLAGS, "TlsCertificateFlags", mGio);

    RG_DEF_SMETHOD(load, -1);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(verify, 2);
}

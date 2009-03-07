/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmozembed-init.c -

    Ruby-GNOME2 Gtk::MozEmbed - Ruby bindings for GtkMozEmbed

    Copyright (C) 2009 Ruby-GNOME2 Project Team.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
************************************************/

#include "rbgtkmozembed.h"

#ifdef XPCOM_GLUE
#  include "gtkmozembed_glue.cpp"
#endif

G_BEGIN_DECLS

#ifdef XPCOM_GLUE
static void
setup_xpcom_glue(void)
{
    static const GREVersionRange greVersion = {
	"1.9b", PR_TRUE,
	"2", PR_TRUE
    };
    char xpcomPath[PATH_MAX];

    nsresult rv = GRE_GetGREPathWithProperties(&greVersion, 1, nsnull, 0,
					       xpcomPath, sizeof(xpcomPath));
    if (NS_FAILED(rv))
        return;

    rv = XPCOMGlueStartup(xpcomPath);
    if (NS_FAILED(rv))
        return;

    rv = GTKEmbedGlueStartup();
    if (NS_FAILED(rv))
        return;

    rv = GTKEmbedGlueStartupInternal();
    if (NS_FAILED(rv))
        return;

    char *lastSlash = strrchr(xpcomPath, '/');
    if (lastSlash)
	*lastSlash = '\0';

    gtk_moz_embed_set_path(xpcomPath);

    gtk_moz_embed_push_startup();
}
#endif

void
Init_gtkmozembed(void)
{
#ifdef XPCOM_GLUE
    setup_xpcom_glue();
#else
#  ifdef DEFAULT_MOZILLA_FIVE_HOME
    gtk_moz_embed_set_comp_path(DEFAULT_MOZILLA_FIVE_HOME);
#  endif
#endif

    Init_gtk_moz_embed();
}

G_END_DECLS

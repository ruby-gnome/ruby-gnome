/* -*- c-file-style: "ruby" -*- */
/************************************************

  init.c -

  $Author: mutoh $
  $Date: 2002/09/10 17:42:51 $

  Copyright (C) 1998-2001 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

EXTERN VALUE rb_argv, rb_progname;

void
Init_gtk2()
{
    int argc, i;
    char **argv;

    gtk_set_locale();

    argc = (int)RARRAY(rb_argv)->len;
    argv = ALLOCA_N(char*,argc+1);
    argv[0] = STR2CSTR(rb_progname);
    for (i = 0; i < argc; i++) {
		if (TYPE(RARRAY(rb_argv)->ptr[i]) == T_STRING) {
			argv[i+1] = RSTRING(RARRAY(rb_argv)->ptr[i])->ptr;
		}
		else {
			argv[i+1] = "";
		}
    }
    argc++;
    {
		/* Gdk modifies sighandlers, sigh */
#ifdef NT
		RETSIGTYPE (*sigfunc[3])();
#else
		RETSIGTYPE (*sigfunc[7])();
#endif

#ifdef NT
		sigfunc[0] = signal(SIGINT, SIG_IGN);
		sigfunc[1] = signal(SIGSEGV, SIG_IGN);
		sigfunc[2] = signal(SIGTERM, SIG_IGN);
#else
		sigfunc[0] = signal(SIGHUP, SIG_IGN);
		sigfunc[1] = signal(SIGINT, SIG_IGN);
		sigfunc[2] = signal(SIGQUIT, SIG_IGN);
		sigfunc[3] = signal(SIGBUS, SIG_IGN);
		sigfunc[4] = signal(SIGSEGV, SIG_IGN);
		sigfunc[5] = signal(SIGPIPE, SIG_IGN);
		sigfunc[6] = signal(SIGTERM, SIG_IGN);
#endif

		gtk_init(&argc, &argv);

#ifdef NT
		signal(SIGINT,  sigfunc[0]);
		signal(SIGSEGV, sigfunc[1]);
		signal(SIGTERM, sigfunc[2]);
#else
		signal(SIGHUP,  sigfunc[0]);
		signal(SIGINT,  sigfunc[1]);
		signal(SIGQUIT, sigfunc[2]);
		signal(SIGBUS,  sigfunc[3]);
		signal(SIGSEGV, sigfunc[4]);
		signal(SIGPIPE, sigfunc[5]);
		signal(SIGTERM, sigfunc[6]);
#endif
    }

    for (i = 1; i < argc; i++) {
		RARRAY(rb_argv)->ptr[i-1] = rb_obj_taint(rb_str_new2(argv[i]));
    }
    RARRAY(rb_argv)->len = argc-1;

    Init_gtk_gdk();
    Init_gtk_gtk();

    Init_gtk_inits();
}

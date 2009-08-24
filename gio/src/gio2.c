#include "gio2.h"

void Init_gio2(void);

void
Init_gio2(void)
{
	VALUE glib = rb_define_module("GLib");

        DEF_CONSTANTS_CLASS(G_TYPE_FILESYSTEM_PREVIEW_TYPE,
                            "FilesystemPreviewType",
                            "G_",
                            glib);

	Init_gappinfo(glib);
	Init_gapplaunchcontext(glib);
	Init_gasyncresult(glib);
	Init_gbufferedinputstream(glib);
	Init_gbufferedoutputstream(glib);
	Init_gcancellable(glib);
	Init_gcontenttype(glib);
	Init_gdatainputstream(glib);
	Init_gfileattributematcher(glib);
	Init_gdataoutputstream(glib);
	Init_gdrive(glib);
	Init_gemblem(glib);
	Init_gemblemedicon(glib);
	Init_gfile(glib);
	Init_gfileattribute(glib);
	Init_gfileenumerator(glib);
	Init_gfileicon(glib);
	Init_gfileinfo(glib);
	Init_gfileinputstream(glib);
	Init_gfilemonitor(glib);
	Init_gfilenamecompleter(glib);
	Init_gfileoutputstream(glib);
	Init_gfilterinputstream(glib);
	Init_gfilteroutputstream(glib);
	Init_gicon(glib);
	Init_ginputstream(glib);
        Init_gioerror(glib);
	Init_gloadableicon(glib);
	Init_gmemoryinputstream(glib);
	Init_gmemoryoutputstream(glib);
	Init_gmount(glib);
	Init_gmountoperation(glib);
        Init_goutputstream(glib);
	Init_gseekable(glib);
	Init_gthemedicon(glib);
	Init_gvolume(glib);
	Init_gvolumemonitor(glib);
}

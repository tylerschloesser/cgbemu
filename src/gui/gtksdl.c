#include "gtksdl.h"

#include <gdk/gdkx.h>

static void gtk_sdl_class_init(GtkSdlClass *class);
static void gtk_sdl_init(GtkSdl *sdl);
static void gtk_sdl_realize(GtkWidget *widget);
static void gtk_sdl_size_allocate(GtkWidget	*widget, GtkAllocation *allocation);

static void gtk_sdl_send_configure(GtkSdl *sdl);
static void gtk_sdl_surface_attach(GtkSdl *sdl);
static gint gtk_sdl_expose(GtkWidget *widget, GdkEventExpose *event);

static GtkWidgetClass *parent_class = NULL;

GtkType gtk_sdl_get_type(void)
{
	static GtkType gtk_sdl_type = 0;

	if(!gtk_sdl_type)
	{
		static const GtkTypeInfo gtk_sdl_info =
		{
			"GtkSdl",
			sizeof(GtkSdl),
			sizeof(GtkSdlClass),
			(GtkClassInitFunc)gtk_sdl_class_init,
			(GtkObjectInitFunc)gtk_sdl_init,
			NULL,
			NULL,
			(GtkClassInitFunc)NULL,
		};
		gtk_sdl_type = gtk_type_unique(GTK_TYPE_WIDGET, &gtk_sdl_info);
	}

	return gtk_sdl_type;
}

static void gtk_sdl_class_init(GtkSdlClass *class)
{
	GtkObjectClass *object_class;
	GtkWidgetClass *widget_class;

#ifdef DEBUG_SDL_WIDGET
	puts ("before class init");
#endif

	widget_class = GTK_WIDGET_CLASS(class);
	object_class = GTK_OBJECT_CLASS(class);
	
	
	widget_class->realize = gtk_sdl_realize;
	widget_class->size_allocate = gtk_sdl_size_allocate;
	widget_class->expose_event = gtk_sdl_expose;

	parent_class = gtk_type_class(gtk_widget_get_type());

#ifdef DEBUG_SDL_WIDGET
	puts ("after class init");
#endif
}

static void gtk_sdl_init (GtkSdl *sdl)
{
#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl init");
#endif

	GTK_WIDGET_SET_FLAGS(sdl, GTK_CAN_FOCUS);
	if (GTK_WIDGET_CAN_FOCUS(sdl))
	{
		gtk_widget_grab_focus(GTK_WIDGET(sdl));
	}
	gtk_widget_set_events(GTK_WIDGET(sdl), GDK_KEY_PRESS_MASK);

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl init");
#endif
}

GtkWidget *gtk_sdl_new (gint width, gint height, gint bpp, Uint32 flags,SDL_Surface * screen)
{
	GtkSdl *sdl;

#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl new");
#endif

	sdl = gtk_type_new (gtk_sdl_get_type ());

	sdl->width = width;
	sdl->height = height;
	sdl->bpp = bpp;
	sdl->flags = flags;

	gtk_sdl_size ( GTK_SDL (sdl), width, height);

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl new");
#endif

	screen  = sdl->surface;

	return GTK_WIDGET (sdl);
}

void gtk_sdl_size (GtkSdl *sdl, gint width, gint height)
{
#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl size");
#endif

	g_return_if_fail(GTK_IS_SDL(sdl));

	GTK_WIDGET(sdl)->requisition.width = width;
	GTK_WIDGET(sdl)->requisition.height = height;

	gtk_widget_queue_resize(GTK_WIDGET(sdl));

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl size");
#endif
}

static void gtk_sdl_realize (GtkWidget *widget)
{
	GtkSdl *sdl;
	GdkWindowAttr attributes;
	gint attributes_mask;

#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl realize");
#endif

	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_SDL (widget));

	sdl = GTK_SDL(widget);
	GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);

	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = widget->allocation.x;
	attributes.y = widget->allocation.y;
	attributes.width = widget->allocation.width;
	attributes.height = widget->allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual(widget);
	attributes.colormap = gtk_widget_get_colormap(widget);
	attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;

	attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

	widget->window = gdk_window_new (gtk_widget_get_parent_window (widget), &attributes, attributes_mask);
	gdk_window_set_user_data (widget->window, sdl);

	widget->style = gtk_style_attach (widget->style, widget->window);
	gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);

	gtk_sdl_send_configure (GTK_SDL (widget));

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl realize");
#endif
}

static void gtk_sdl_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl size allocate");
#endif

	g_return_if_fail (widget != NULL);
	g_return_if_fail (GTK_IS_SDL (widget));
	g_return_if_fail (allocation != NULL);

	widget->allocation = *allocation;
	/* FIXME, TODO-1.3: back out the MAX() statements */
	widget->allocation.width = MAX (1, widget->allocation.width);
	widget->allocation.height = MAX (1, widget->allocation.height);

	if (GTK_WIDGET_REALIZED (widget))
	{
		gdk_window_move_resize (widget->window,
					allocation->x, allocation->y,
					allocation->width, allocation->height);

		gtk_sdl_send_configure (GTK_SDL (widget));
	}

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl size allocate");
#endif
}

static void gtk_sdl_send_configure (GtkSdl *sdl)
{
	
	GtkWidget *widget;
	GdkEventConfigure event;

#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl send configure");
#endif

	widget = GTK_WIDGET (sdl);

	event.type = GDK_CONFIGURE;
	event.window = widget->window;
	event.send_event = TRUE;
	event.x = widget->allocation.x;
	event.y = widget->allocation.y;
	event.width = widget->allocation.width;
	event.height = widget->allocation.height;

	gtk_widget_event (widget, (GdkEvent*) &event);

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl send configure");
#endif
}

static void gtk_sdl_surface_attach (GtkSdl *sdl)
{
	gchar SDL_windowhack[32];

#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl surface attach");
#endif

	/* Attach the SDL_Surface */
#ifdef DEBUG_SDL_WIDGET
	puts ("attaching the surface");
#endif

#ifdef _WIN32
	sprintf (SDL_windowhack, "SDL_WINDOWID=%ld", GDK_WINDOW_HWND ( GTK_WIDGET(sdl)->window ) );
#else
	sprintf( SDL_windowhack, "SDL_WINDOWID=%ld", GDK_WINDOW_XWINDOW( GTK_WIDGET( sdl )->window ) );
#endif

	puts (SDL_windowhack);
	putenv (SDL_windowhack);

	SDL_QuitSubSystem (SDL_INIT_VIDEO);
	
#ifdef DEBUG_SDL_WIDGET
	puts ("before creating new surface");
#endif

	if ( SDL_InitSubSystem ( SDL_INIT_VIDEO ) < 0)
	{
		fprintf (stderr, "unable to init SDL: %s", SDL_GetError() );
		return;
	}
	
#ifdef DEBUG_SDL_WIDGET
	puts ("after creating new surface");
#endif

	if (sdl->flags &= (SDL_OPENGLBLIT | SDL_DOUBLEBUF))
	{
		SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	}

	if (sdl->surface)
	{
		puts ("TODO: deallocate previus surface");
	}

	sdl->surface = SDL_SetVideoMode( sdl->width, sdl->height, sdl->bpp, sdl->flags);
	if (!sdl->surface)
	{
		fprintf (stderr, "Unable to set the video mode: %s", SDL_GetError() );
		return;
	}

#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl surface attach");
#endif
}

static gint gtk_sdl_expose (GtkWidget *widget, GdkEventExpose *event)
{
	SDL_Surface *surface;
	SDL_Rect dest;

#ifdef DEBUG_SDL_WIDGET
	puts ("before sdl expose");
#endif

	g_return_val_if_fail (widget != NULL, FALSE);
	g_return_val_if_fail (GTK_IS_SDL (widget), FALSE);
	g_return_val_if_fail (event != NULL, FALSE);

	if (event->count > 0) return FALSE;

	surface = SDL_GetVideoSurface ();

	if (!surface)
	{
		gtk_sdl_surface_attach (GTK_SDL (widget));
	}

	gtk_sdl_display_update ( GTK_SDL(widget) );

/* 	if (surface) */
/* 	{ */

/* 		puts ("update rects from expose"); */
	
/* 		dest.x = event->area.x; */
/* 		dest.y = event->area.y; */
/* 		dest.w = event->area.width; */
/* 		dest.h = event->area.height; */

		/* ADDED NOW! */
/* 		SDL_UpdateRect (surface, 0, 0, 0, 0); */

/* LATER OFF */
/* 		SDL_UpdateRects (surface, 1, &dest); */

/* 	} */
/* 	else */
/* 	{ */
/* 		gtk_sdl_surface_attach (GTK_SDL (widget)); */
/* 	} */
#ifdef DEBUG_SDL_WIDGET
	puts ("after sdl expose");
#endif

	surface = SDL_GetVideoSurface ();
	printf ("surface is : %p\n", surface);
/* 	SDL_UpdateRect (surface, 0, 0, 0, 0); */
/* 	puts ("rectangle updated"); */

	return FALSE;
}

void gtk_sdl_display_update (GtkSdl *sdl)
{

  SDL_Surface *surface;

	//puts ("before sdl display update");

	g_return_if_fail (sdl != NULL);
	g_return_if_fail (GTK_IS_SDL (sdl));
	g_return_if_fail (sdl->surface);

	if ( (sdl->flags &=SDL_OPENGL) || (sdl->flags &= SDL_OPENGLBLIT) )
	{
		/*
		if (dkActiveUpdaterGL) (*dkActiveUpdaterGL)();
		*/
		SDL_GL_SwapBuffers ();
	}
	else
	{
		/*
		if (dkActiveUpdater) (*dkActiveUpdater)();
		*/
#ifdef DEBUG_SDL_WIDGET
		puts ("get video surface");
#endif

		surface = SDL_GetVideoSurface ();
		
#ifdef DEBUG_SDL_WIDGET
		puts ("flip video surface");
#endif

		SDL_Flip (surface);
	}

	//puts ("before update rect");

	SDL_UpdateRect (surface, 0, 0, 0, 0);

	//puts ("after update rect");

	//puts ("after display update");
}

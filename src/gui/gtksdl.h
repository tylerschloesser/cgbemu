#ifndef GTK_SDL_H_
#define GTK_SDL_H_

# include <stdlib.h>

# include <gtk/gtk.h>
# include <gdk/gdk.h>
# include <gtk/gtkwidget.h>
# include <gdk/gdkwin32.h>

# include <SDL.h>

# define GTK_TYPE_SDL			(gtk_sdl_get_type ())
# define GTK_SDL(obj)			(GTK_CHECK_CAST ((obj), GTK_TYPE_SDL, GtkSdl))
# define GTK_SDL_CLASS(class)		(GTK_CHECK_CLASS_CAST ((class), GTK_TYPE_SDL, GtkSdlClass))
# define GTK_IS_SDL(obj)		(GTK_CHECK_TYPE ((obj), GTK_TYPE_SDL))
# define GTK_IS_SDL_CLASS(class)	(GTK_CHECK_CLASS_TYPE ((class), GTK_TYPE_SDL))

//#define DEBUG_SDL_WIDGET

typedef struct _GtkSdl		GtkSdl;
typedef struct _GtkSdlClass	GtkSdlClass;

GtkWidget* SDL;//temptemtempte6

struct _GtkSdl
{
  GtkWidget widget;

  SDL_Surface *surface;
  gint width, height, bpp;
  Uint16 flags;
};

struct _GtkSdlClass
{
  GtkWidgetClass parent_class;
};


GtkType    gtk_sdl_get_type	(void);
GtkWidget * gtk_sdl_new (gint width, gint height, gint bpp, Uint32 flags,SDL_Surface * screen);
void gtk_sdl_size(GtkSdl * sdl, gint width, gint height);

void gtk_sdl_display_update (GtkSdl *sdl);


#endif /* GTK_SDL_H_ */

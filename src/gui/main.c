#include "gtksdl.h"

#include "../cgb/screen.h"

//temp
#include "../cgb/graphics.h"

#include "../debug.h"
#include "../cgb/gameboy.h"

#include "SDL.h"

#include <Windows.h>

#include <gdk/gdkkeysyms.h>

#define NO_STDIO_REDIRECT

void initialize_main_window();
void initialize_options_window();

static void save_state();
static void load_state();

static GtkItemFactoryEntry main_window_menu_items[] = {
	{ "/_File", NULL, NULL, 0, "<Branch>" },
	{ "/File/Save State", NULL, save_state, 0, NULL },
	{ "/File/Load State", NULL, load_state, 0, NULL },
	{ "/File/Quit", "<control>Q", gtk_main_quit, 0, NULL },
	{ "/_View", NULL, NULL, 0, "<Branch>" },
	{ "/View/Debug", NULL, NULL, 0, "<ToggleItem>" },
	{ "/_Tools", NULL, NULL, 0, "<Branch>" },
	{ "/Tools/Options", "<control>O", initialize_options_window, 0, NULL },
};

void get_main_window_menu( GtkWidget *window, GtkWidget **menubar );

static gboolean on_key( GtkWidget *widget, GdkEventKey *event, gpointer user_data );
static gboolean on_destroy();

void save_state()
{
	gameboy_save_state( "save_state.bin" );
}

void load_state()
{
	gameboy_load_state( "save_state.bin" );
}

int main_window(void* unused) {

	initialize_main_window();

	gtk_main();
	
	return 0;
}

void close_all_threads()
{
	
}

int main( int argc, char** argv )
{
	/* this was taken from the old main...*/
	open_debug_console();
	
	
	/* initialize sdl */
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf( stderr, "error initializing SDL: %s\n", SDL_GetError( ) );
		return EXIT_FAILURE;
	}

	/* initialize gtk */
	if( gtk_init_check( &argc, &argv ) == FALSE ) {
		fprintf( stderr, "error initializing GTK\n" );
		return EXIT_FAILURE;
	}
	
	/* create a thread for the main window */
	SDL_Thread * main_window_thread = SDL_CreateThread(main_window, NULL);
	if(main_window_thread == NULL) {
		fprintf( stderr, "error creating main_window_thread: %s\n", SDL_GetError() );
		return EXIT_FAILURE;
	}

	fprintf( stdout, "welcome to cgbemu\n" );

	gameboy_power_on();
	
	// let this thread handle the cpu
	cpu_emulate();
	
	return EXIT_SUCCESS;
	
}

void initialize_options_window()
{
	GtkWidget* window;
	GtkWidget* layout;
	GtkWidget* image;
	GtkWidget* notebook;
	
	GdkPixbuf* gdk_pixbuf;
	
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_default_size( GTK_WINDOW( window ), 256, 256 );
	
	notebook = gtk_notebook_new();
	
	GtkWidget* page1;
	gtk_notebook_append_page( GTK_NOTEBOOK( notebook ), page1, NULL );
	/*
	u8* data = get_background_map(0x1C00, true);

	/*
	gdk_pixbuf = gdk_pixbuf_new_from_data((const guchar *)data, GDK_COLORSPACE_RGB, false, 8, 256, 256, 0, NULL, NULL );
	*
	gdk_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,false,8,256,256);
	
	guchar* pixels = gdk_pixbuf_get_pixels(gdk_pixbuf);
	
	
	int n_channels = gdk_pixbuf_get_n_channels(gdk_pixbuf);
	assert(n_channels== 3);
	
	int rowstride = gdk_pixbuf_get_rowstride(gdk_pixbuf);
	
	fprintf(stderr,"rowstride=%i\n", rowstride);
	
	
	int n = (rowstride)*256;
	int i;
	for(i = 0; i < n; ++i){
		pixels[i]=data[i];
	}
	*/
	
	layout = gtk_layout_new( NULL, NULL );
	gtk_container_add( GTK_CONTAINER( window ), layout );
	gtk_widget_show( layout );
	
	//image = gtk_image_new_from_file( "D:/Code/cgbemu/test_image.png" );
	//image = gtk_image_new_from_pixbuf( gdk_pixbuf );
	//gtk_layout_put( GTK_LAYOUT( layout ), image, 0, 0 );
	
	gtk_layout_put( GTK_LAYOUT( layout ), notebook, 0, 0 );
	
	gtk_widget_show_all( window );

}

void initialize_main_window() 
{
	GtkWidget* window;
	GtkWidget* box;
	GtkWidget* menu_bar;
	//GtkWidget* SDL;
	

	
	/* create a vertical container box */
	box = gtk_vbox_new( FALSE, 0 );
	
	
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	//SDL = gtk_sdl_new( 160, 144, 32, SDL_SWSURFACE, surface );
	SDL = gtk_sdl_new( 160, 144, 32, SDL_DOUBLEBUF | SDL_HWSURFACE, surface );
	
	/* menu stuff */
	get_main_window_menu( window, &menu_bar );
	/*
	GtkWidget* file_menu = gtk_menu_bar_new();
	GtkWidget* quit_item = gtk_menu_item_new_with_label( "Quit" );
	gtk_menu_append( GTK_MENU( file_menu ), quit_item );
	*/
	
	
	/* populate the continer box */
	/*
	gtk_box_pack_start( GTK_BOX( box ), file_menu, FALSE, TRUE, 0 );
	*/
	gtk_box_pack_start( GTK_BOX( box ), menu_bar, FALSE, TRUE, 0 );
	gtk_box_pack_start( GTK_BOX( box ), SDL, TRUE, TRUE, 0 );
	
	/* add container to window */
	gtk_container_add( GTK_CONTAINER( window ), box );
	
	//g_signal_connect( G_OBJECT( window ), "destroy", G_CALLBACK( gtk_main_quit ), NULL );
	g_signal_connect( G_OBJECT( window ), "destroy", G_CALLBACK( on_destroy ), NULL );
	// g_signal_connect( G_OBJECT( window ), "key_press_event", G_CALLBACK( sdl_on_key_press ), NULL );
	g_signal_connect( G_OBJECT( window ), "key-press-event", G_CALLBACK( on_key ), NULL );
	g_signal_connect( G_OBJECT( window ), "key-release-event", G_CALLBACK( on_key ), NULL );

	GTK_WIDGET_UNSET_FLAGS(SDL, GTK_CAN_FOCUS );
	
	gtk_widget_show_all( window );
	
}

void get_main_window_menu( GtkWidget *window, GtkWidget **menubar ) 
{
	GtkItemFactory *item_factory;
	GtkAccelGroup *accel_group;
	gint nmenu_items = sizeof( main_window_menu_items ) / sizeof( main_window_menu_items[0] );
	
	accel_group = gtk_accel_group_new();
	
	item_factory = gtk_item_factory_new( GTK_TYPE_MENU_BAR, "<main>", accel_group );
	
	gtk_item_factory_create_items( item_factory, nmenu_items, main_window_menu_items, NULL );
	
	gtk_window_add_accel_group( GTK_WINDOW( window ), accel_group );
	
	if( menubar ) {
		*menubar = gtk_item_factory_get_widget( item_factory, "<main>" );
	}
}	

gboolean on_destroy()
{
	if( debug_console_opened == true ) {
		close_debug_console();
	}
	gameboy_power_off();
	gtk_main_quit();
}

gboolean on_key( GtkWidget *widget, GdkEventKey *event, gpointer user_data ) 
{
	enum BUTTON button = BUTTON_INVALID;

	switch( event->keyval ) 
	{
		case GDK_Up:
			button = BUTTON_UP;
			break;
		case GDK_Down:
			button = BUTTON_DOWN;
			break;
		case GDK_Right:
			button = BUTTON_RIGHT;
			break;
		case GDK_Left:
			button = BUTTON_LEFT;
			break;
		case GDK_Return:
			button = BUTTON_START;
			break;
		case GDK_space:
			button = BUTTON_SELECT;
			break;
		case GDK_A:
		case GDK_a:
			button = BUTTON_A;
			break;
		case GDK_Z:
		case GDK_z:
			button = BUTTON_B;
			break;
	}
	
	if( button != BUTTON_INVALID ) {
	
		bool pressed = true;
		
		if( event->type == GDK_KEY_RELEASE ) {
			pressed = false;
		}
		
		gameboy_toggle_button( button, pressed );
	}
}

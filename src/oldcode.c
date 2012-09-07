
int render_background0(u8 lcd_control)
{
	u8 scanline = hardware_registers[LY];
	
	u8 scroll_y = hardware_registers[SCY];
	u8 scroll_x = hardware_registers[SCX];
	
	u8 window_y = hardware_registers[WY];
	u8 window_x = hardware_registers[WX];
	
	bool render_window = false;
	if(lcd_control & WINDOW_DISPLAY_ENABLE) {
		if(window_y <= scanline) {
			render_window = true;
			
		}
	}
	render_window = false; //temp
	
	//To improve performance for this next part, it might be a good
	//idea to switch which tile offset is default. I would think an
	//offset of 0 is more common than an offset of 0x800
	u16 tile_data_offset = 0x800;	//0x8800
	bool signed_tile = true;
	if(lcd_control & BG_WINDOW_TILE_DATA_SELECT) {
		tile_data_offset = 0; 		//0x8000
		signed_tile = false;
	}
	
	//Determine which tile map is used. There are 2 tile maps, each of
	//size 0x400 Bytes (32x32 tiles), located at 0x9800 and 0x9C00
	u16 tile_map_offset = 0x1800;
	if(render_window) {
		if(lcd_control & WINDOW_TILE_MAP_DISPLAY_SELECT) {
			tile_map_offset = 0xC000;
		}
	} else { //render background
		if(lcd_control & BG_TILE_MAP_DISPLAY_SELECT) {
			tile_map_offset = 0xC000;
		}
	}
	
	u8 y_position = 0;
	if(render_window) {
		y_position = scanline - window_y;
	} else { //render background
		y_position = scanline + scroll_y;
	}
	
	//Multiplying by 32 right here means tile_row isn't really the
	//tile row, but it saves from frequent multiplications below
	//and we shouldn't ever nead to use the actual tile row
	u16 tile_row = (y_position / 8) * 32;
	
	//render the horizontal scanline
	u8 x_pixel;
	for(x_pixel = 0; x_pixel < SCREEN_WIDTH; ++x_pixel) {
		u8 x_position = x_pixel + scroll_x;
		
		if(render_window) {
			if(x_pixel >= window_x) {
				x_position  = x_pixel - window_x;
			}
		}
		
		u16 tile_column = x_position / 8;
		u16 tile_address = tile_map_offset + tile_row + tile_column;
		
		u8 tile_number = gameboy_vram[tile_address];
		
		u32 tile_location = tile_data_offset;
		tile_location += (tile_number * 16); //temp
		
		/* //temp
		if(signed_tile && (tile_number & 0x80)) {
			tile_location += (((s8)tile_number + 128) * 16);
		} else {
			tile_location += (tile_number * 16);
		}
		*/ //temp
		
		u8 y_pixel = y_position % 8;
		y_pixel *= 2;
		
		u8 low  = gameboy_vram[tile_location + 0];
		u8 high = gameboy_vram[tile_location + 1];
		
		u8 bit = 0x80 >> (x_position % 8);
		u32 color;
		u8 color_number;
		if(high & bit) {
			color_number |= 0x2;
		}
		if(low & bit) {
			color_number |= 0x1;
		}
		
		switch(color_number) {
			case 3:
				//pallete_entry = pallete[pallete_index + 6];
				//pallete_entry |= (pallete[pallete_index + 7] << 8);
				color = SDL_MapRGB(surface->format, 0,0,0); //temp
				break;
			case 2:
				//pallete_entry = pallete[pallete_index + 4];
				//pallete_entry |= (pallete[pallete_index + 5] << 8);
				color = SDL_MapRGB(surface->format, 96,96,96); //temp
				break;
			case 1:
				//pallete_entry = pallete[pallete_index + 2];
				//pallete_entry |= (pallete[pallete_index + 3] << 8);
				color = SDL_MapRGB(surface->format, 192,192,192); //temp
				break;
			case 0:
				//pallete_entry = pallete[pallete_index + 0];
				//pallete_entry |= (pallete[pallete_index + 1] << 8);		
				color = SDL_MapRGB(surface->format, 192,192,192); //temp
				break;
		}
		
		if(scanline < 0 || scanline > 143 || x_pixel < 0 || x_pixel > 159) {
			continue;
		}
		
		u32 *pixmem32 = (u32*)surface->pixels + (x_pixel + (scanline * SCREEN_WIDTH));
		*pixmem32 = color;
	}
	
	
}
/*
int render_sprites()
{

	
}
*/

int line = 0;
int render_line()
{

	//Background
	//32*32 bytes (tiles) (1024)
	//Each byte represents 1 tile
	
	if(hardware_registers[LCDC] & BG_DISPLAY) {
		//Display Background
		u8 offset = (hardware_registers[LCDC] & BG_TILE_MAP_DISPLAY_SELECT) ? 0x400 : 0;
		
		int x = hardware_registers[SCX];
		int y = hardware_registers[SCY] + line;
		
		
		int i;
		for(i = 0; i < SCREEN_WIDTH; i++) {
			u8 tile =  x / 8 + y * 32;
			
		}
	}
	
	line++;
}


//Render the entire screen
int render_screenNEW()
{ 
	u8 lcd_control = hardware_registers[LCDC];
	if((lcd_control & LCD_DISPLAY_ENABLE) == 0) {
		#ifndef FORCE_LCD_DISPLAY_ENABLE
			return 0;
		#endif
	}
	
	if(lcd_control & BG_DISPLAY) {
		render_background(lcd_control);
	}
	
	if(lcd_control & WINDOW_DISPLAY_ENABLE) {
		//render_window(lcd_control);
	}
	
}

int render_backgroundNEW(u8 lcd_control) 
{
	u8 scroll_y = hardware_registers[SCY];
	u8 scroll_x = hardware_registers[SCX];
	
	//Determine whether we are using locations 0x8800-0x97FF
	//or 0x8000-0x8FFF. If we are using 0x8800-0x97FF, the
	//tile index must be read as a signed integer
	u16 tile_data_offset = 0;
	bool signed_tile = false;
	if((lcd_control & BG_WINDOW_TILE_DATA_SELECT) == 0) {
		tile_data_offset = 0x800;
		signed_tile = true;
	}
	
	//Determine whether we are using locations 0x9800-0x9BFF
	//or 0x9C00-0x9FFF for the tile map. Each map is 0x400 Bytes
	//(32x32 tiles)
	u16 tile_map_offset = 0x1800;
	if(lcd_control & BG_TILE_MAP_DISPLAY_SELECT) {
		tile_map_offset = 0x1C00;
	}
	
	u32 total_pixels = SCREEN_WIDTH * SCREEN_HEIGHT;
	u32 pixel;
	for(pixel = 0; pixel < total_pixels; ++pixel) {
		//FIX THIS - very ineffecient due to modulus operator
		u8 x_pixel = pixel % SCREEN_WIDTH;
		u8 y_pixel = pixel / SCREEN_WIDTH;
		
		
	}
	
	
}

int render_windowNEW(u8 lcd_control)
{
	u8 window_y = hardware_registers[WY];
	u8 window_x = hardware_registers[WX];
}

int render_spritesNEW()
{

}



//int render_screen() 
int render_spritesx()
{

	//SDL_Thread * thread;
	//thread = SDL_CreateThread(GUI, NULL);
	/*
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Surface*    Surf_Display;
	if((Surf_Display = SDL_SetVideoMode(128, 128, 0, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return 1;
    }*/
	/*
	       if(SDL_MUSTLOCK(Surf_Display)) 
        { 
                if(SDL_LockSurface(Surf_Display)) 
                { 
                        return; 
                } 
        } 
*/
	//printf("%i\n", SDL_MapRGB(Surf_Display->format, 255, 255, 255));

	SDL_Surface* Surf_Display = surface;

	int i, j;
	for(i = 0; i < 256; i++) { //256 tiles
		int x_tile = i % 16;
		int y_tile = i / 16;
		
		for(j = 0; j < 64; j++) { //64 pixels per tile
			//16 bytes per tile
			//8 pixels per 2 bytes
			
			//ineffecient
			int index = i * 16 + (j / 8)*2;
			//printf("index: %i\n", index);
			
			/*
			u8 high = MBC_read(0x8000+index);
			u8 low  = MBC_read(0x8000+index+1);
			*/
			
			u8 offset = hardware_registers[VBK] ? 0x2000 : 0;
			offset = 0;
			u8 high = gameboy_vram[index+offset];
			u8 low  = gameboy_vram[index+1+offset];
			
			
			//printf("%02X&%02X\n", high, low);
			u8 bit  = (0x80) >> (j % 8);
			//printf("bit: %i", bit);
			u32 color;
			if(high&bit && low&bit) {
				color = SDL_MapRGB(Surf_Display->format, 0,0,0);// printf("black");
			} else if(high&bit) {
				color = SDL_MapRGB(Surf_Display->format, 96,96,96); //printf("grey");
			} else if(low&bit) {
				color = SDL_MapRGB(Surf_Display->format, 192,192,192); //printf("grey");
			} else {
				color = SDL_MapRGB(Surf_Display->format, 255,255,255); //printf("white");
			}
			//printf("\n");
			int x_bit = j % 8;
			int y_bit = j / 8;
			
			int position = x_tile * 8 + x_bit + y_tile * 8 * 128 + y_bit * 128;
			//printf("x: %i y: %i\n", x_bit, y_bit);
			//printf("position: %i\n", position);
			u32 *pixmem32 = (u32*)Surf_Display->pixels+position;
			*pixmem32 = color;
		}
	}
	/*
	int i;
	for(i = 0; i < 16384; i++) {
		//printf("%i:%02X\n",i, vram[i/4]);
		u8 pixel = vram[i/4];
		if(pixel % 4 == 0) {
			pixel = pixel >> 6;
		} else if(pixel % 4 == 1) {
			pixel = pixel >> 4;
		} else if(pixel % 4 == 2) {
			pixel = pixel >> 2;
		}
		//pixel = pixel >> (i%4*2);
		pixel &= 3;
		u32 color;
		//printf("%i\n", pixel);
		//if(pixel != 0)
			//printf("pixel: %i\n", pixel);
		switch(pixel) {
			case 0: 
				color = SDL_MapRGB(Surf_Display->format, 255,255,255);
				//printf("white");
				break;
			case 1:
				color = SDL_MapRGB(Surf_Display->format, 192,192,192);
				//printf("grey");
				break;
			case 2:
				color = SDL_MapRGB(Surf_Display->format, 96,96,96);
				//printf("grey");
				break;
			case 3:
				color = SDL_MapRGB(Surf_Display->format, 0,0,0);
				//printf("black");
				break;
		}
		//if(color != 0){
			//printf("%i\n", color);
		
		int tile = i / 64; //256 tiles
		int x_tile = tile % 16;
		int y_tile = tile / 16;
		int x_bit = i % 8;
		int y_bit = (i / 8)%8;
		//printf("i: %i, ybit:%i\n",i,  y_bit);
		//printf("X:%i Y:%i \n", x_tile * 8 + x_bit, y_tile * 8 + y_bit);
		int position = x_tile * 8 + x_bit + y_tile * 8 * 128 + y_bit * 128;
		//printf("position: %i\n", position);
		if(i % 256 == 0){
			//getchar();
		}
		//u32 *pixmem32 = (u32*)Surf_Display->pixels+i;
		u32 *pixmem32 = (u32*)Surf_Display->pixels+position;
		//printf("%i\n", pixmem32);
		*pixmem32 = color;
		
	}
	*/
     /*
		 if(SDL_MUSTLOCK(Surf_Display)) 
        { 
                SDL_UnlockSurface(Surf_Display); 
        } 
*/
	
	SDL_UpdateRect(Surf_Display, 0, 0, 0, 0); 
	//printf("Screen updated");
	//getchar();
/* 	int done = 0;
	while(!done) {
		//message processing loop
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			//check for messages
			switch(event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_MOUSEMOTION:
					break;
			}
		}
		
		//clear screen
		//SDL_FillRect(Surf_Display, 0, SDL_MapRGB(Surf_Display->format, 0, 0, 0));
		//SDL_Flip(Surf_Display);
		SDL_UpdateRect(Surf_Display, 0, 0, 0, 0); 
	}	 */
	
	//getchar();
}

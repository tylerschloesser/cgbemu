#include "graphics.h"
#include "gameboy.h"
#include "cpu.h"

//testing
#include "screen.h"

#define MAX_TILES_PER_LINE 10

#define SPRITE_MODE_8X8  0
#define SPRITE_MODE_8x16 1

SDL_Surface *surface;

int render_background(u8 lcd_control);
int render_background_old();

int GUI(void *param)
{
	

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}
	
	if((surface = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE)) == NULL) {
		fprintf(stderr, "Unable to setup surface: %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_WM_SetCaption("CGB", "");

	int done = 0;
	while(!done) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					done = 1;
					exit(0);
					break;
			}
			if(done)
				break;
		}
		//clear screen
		//SDL_FillRect(surface, 0, SDL_MapRGB(surface->format, 0, 0, 0));
		//SDL_Flip(surface);
	}
}

void update_screen() 
{
	update_screen2();
/*
	int i, j;
	//OPTIMIZE THIS CODE EASY
	for(i = 0; i < SCREEN_HEIGHT; ++i) {
		for(j = 0; j < SCREEN_WIDTH; ++j) {
			u32 * pixel = (u32*)surface->pixels + ((i * SCREEN_WIDTH) + j);
			u8 red = screen_buffer[i][j][0];
			u32 color = SDL_MapRGB(surface->format, red*8, red*8, red*8);
			*pixel = color;
		}
	}
	*/

	//SDL_UpdateRect(surface, 0, 0, 0, 0); 
}

int render_scanline()
{
	u8 lcd_control = hardware_registers[LCDC];

	if(lcd_control & BG_DISPLAY) {
		render_background(lcd_control);
	}
	
	if(lcd_control & OBJ_DISPLAY_ENABLE) {
		render_sprites();
	}
}

int render_background(u8 lcd_control)
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
	
	u16 tile_data_offset = 0x0;	//0x8000
	bool signed_tile = false;
	if((lcd_control & BG_WINDOW_TILE_DATA_SELECT) == 0) {
		tile_data_offset = 0x800; 		//0x8800
		signed_tile = true;
	}
	
	//Determine which tile map is used. There are 2 tile maps, each of
	//size 0x400 Bytes (32x32 tiles), located at 0x9800 and 0x9C00
	u16 tile_map_offset = 0x1800;
	if(render_window) {
		if(lcd_control & WINDOW_TILE_MAP_DISPLAY_SELECT) {
			tile_map_offset = 0x1C00;
		}
	} else { //render background
		if(lcd_control & BG_TILE_MAP_DISPLAY_SELECT) {
			tile_map_offset = 0x1C00;
		}
	}
	
	u16 y_position = 0;
	if(render_window) {
		y_position = scanline - window_y;
	} else { //render background
		y_position = scanline + scroll_y;
	}
	
	u16 tile_row = y_position / 8;
	
	//render the horizontal scanline
	u8 x_pixel;
	for(x_pixel = 0; x_pixel < SCREEN_WIDTH; ++x_pixel) {
		u8 x_position = x_pixel + scroll_x;
		
		if(render_window) {
			if(x_pixel >= window_x) {
				//x_position  = x_pixel - window_x;
			}
		}
		
		u16 tile_column = x_position / 8;
		u16 tile_address = tile_map_offset + (tile_row * 32) + tile_column;
		
		
		
		

		
		//temp
		u8 tile_number = gameboy_vram[tile_address];
		u8 tile_attributes = gameboy_vram[tile_address + 0x2000];
        
		int vram_offset = (tile_attributes & TILE_VRAM_BANK_NUMBER) ? 0x2000 : 0;
		vram_offset += tile_data_offset;
		
        u8 pallete_number = tile_attributes & BACKGROUND_PALETTE_NUMBER;
		u8 pallete_index = pallete_number * 8;
		
		bool x_flip = false;
		if(tile_attributes & HORIZONTAL_FLIP) {
			x_flip = true;
		}
		bool y_flip = false;
		if(tile_attributes & VERTICAL_FLIP) {
			y_flip = true;
		}
		
		//u8 tile_location = tile_data_offset + (tile_number *  16);
		
		int tile_x_pixel = x_position % 8;
		
		//testing
		if(x_flip == true) {
			tile_x_pixel -= 7;
			tile_x_pixel *= -1;
		}
		
		int tile_y_pixel = y_position % 8;
		
		//testing NOT SURE IF THIS WORKS!!!!
		if(y_flip == true) {
			tile_y_pixel -= 7;
			tile_y_pixel *= -1;
		}
		
		int j = (tile_y_pixel * 8) + tile_x_pixel;
		
		int index;
		if(signed_tile) {
			index = (((s8)tile_number + 128) * 16) + (j / 8) * 2;
		} else {
			index = tile_number * 16 + (j / 8) * 2;
		}
		
		/*
		int x_bit = j % 8;
		int y_bit = j / 8;

		int x_location_temp = x_position + x_bit;
		int y_location_temp = y_position + y_bit;
		*/
		
		
		u8 high = gameboy_vram[index + vram_offset + 1];
		u8 low  = gameboy_vram[index + vram_offset + 0];
		u8 bit = 0x80 >> (j % 8);
        
		//u32 color;	
        u16 pallete_entry;			
        u8 r, g, b;			
			

		if((high & bit) && (low & bit)) {
            pallete_entry = pallete[pallete_index + 6];
			pallete_entry |= (pallete[pallete_index + 7] << 8);
		} else if(high & bit) {
			pallete_entry = pallete[pallete_index + 4];
			pallete_entry |= (pallete[pallete_index + 5] << 8);
		} else if(low & bit) {
			pallete_entry = pallete[pallete_index + 2];
			pallete_entry |= (pallete[pallete_index + 3] << 8);
		} else {
			pallete_entry = pallete[pallete_index + 0];
			pallete_entry |= (pallete[pallete_index + 1] << 8);		
		}
			
        r = (pallete_entry >> 0 ) & 0x1F;
        g = (pallete_entry >> 5 ) & 0x1F;
		b = (pallete_entry >> 10) & 0x1F;
        
		//color = SDL_MapRGB(surface->format, r*8, g*8, b*8);

		if(scanline < 0 || scanline > 143 || x_pixel < 0 || x_pixel > 159) {
			continue;
		}

		//int position = (x_pixel)  + ((scanline) * 160);

		/*
		u32 *pixmem32 = (u32*)surface->pixels+position;
		*pixmem32 = color;
		*/
		screen_buffer[scanline][x_pixel][0] = r;
		screen_buffer[scanline][x_pixel][1] = g;
		screen_buffer[scanline][x_pixel][2] = b;

        
		/*
		s16 tile_number;
		if(signed_tile) {
			tile_number = (s8)gameboy_vram[tile_address];
		} else {
			tile_number = (u8)gameboy_vram[tile_address];
		}
		
		s16 tile_location = tile_data_offset;
		
		if(signed_tile) {
			tile_location += ((tile_number + 128) * 16);
		} else {
			tile_location += (tile_number * 16);
		}
		*/
		
		/*
		u8 y_pixel = y_position % 8;
		y_pixel *= 2;
		
		u8 low  = gameboy_vram[tile_location + y_pixel + 0];
		u8 high = gameboy_vram[tile_location + y_pixel + 1];
		/*
		u8 bit = 0x80 >> (x_position % 8);
		*/
		/*
		int bit = x_position % 8;
		bit -= 7;
		bit *= -1;
		
		u32 color;
		
		
		u8 color_number;
		if(high & bit) {
			color_number |= 0x2;
		}
		if(low & bit) {
			color_number |= 0x1;
		}
		
		u8 temp_color = 0;
		
		switch(color_number) {
			case 3:
				break;
			case 2:
				temp_color = 96;
				break;
			case 1:
				temp_color = 192;
				break;
			case 0:
				temp_color = 255;				
				break;
		}
		
		if(scanline < 0 || scanline > 143 || x_pixel < 0 || x_pixel > 159) {
			continue;
		}
		
		screen_buffer[scanline][x_pixel][0] = temp_color;
		screen_buffer[scanline][x_pixel][1] = temp_color;
		screen_buffer[scanline][x_pixel][2] = temp_color;
		*/
	}
	
}

// only renders sprites on the current scanline
int render_sprites_new() {

    u8 sprite_mode = SPRITE_MODE_8X8;
    if(hardware_registers[LCDC] & OBJ_SIZE) {
		sprite_mode = SPRITE_MODE_8x16;
	}
    
	//TODO limit sprites to 10 per line (refer to pandocs)
	
    u16 sprite_attribute_index = 0xFE00; // OAM memory location
    int sprite_count = 40;
    
    int sprite;
    for(sprite = 0; sprite < sprite_count; ++sprite) {
    
        s16 y_position = MBC_read(sprite_attribute_index++) - 16;
        s16 x_position = MBC_read(sprite_attribute_index++) - 8;
        u8 tile_number = MBC_read(sprite_attribute_index++);
        u8 tile_attributes = MBC_read(sprite_attribute_index++);

		u8 pallete_number = tile_attributes & 0x07;

        if(y_position == -16 || y_position >= 144)
            continue;
        if(x_position == -8 || x_position >= 160)
            continue;
        
          
        u16 vram_offset = (tile_attributes & TILE_VRAM_BANK_NUMBER) ? 0x2000 : 0;
        
        bool x_flip = false;
        if(tile_attributes & HORIZONTAL_FLIP) {
            x_flip = true;
        }
        
        // TODO check for vertical flip

        s16 scanline = hardware_registers[LY];
        u8 sprite_height = 8;
        if(sprite_mode == SPRITE_MODE_8x16) {
            sprite_height = 16;
        }
        
        // check if the sprite is within the current scanline
        if((scanline >= y_position) && (scanline < (y_position + sprite_height))) {
        
            s8 sprite_line = scanline - y_position;
            
			
            u16 sprite_address = vram_offset + (tile_number * 16) + (sprite_line * 2);
            
            u8 high = gameboy_vram[vram_offset + sprite_address + 1];
            u8 low = gameboy_vram[vram_offset + sprite_address + 0];
            u32 color;
            
            int sprite_pixel;
            //for(sprite_pixel = 7; sprite_pixel >= 0; --sprite_pixel) {
            for(sprite_pixel = 0; sprite_pixel < 8; ++sprite_pixel){
                
                s8 x_bit = sprite_pixel;
                if(x_flip == true) {
                    x_bit -= 7;
                    x_bit *= -1;
                }
                
                u8 bit = 0x80 >> x_bit;
                
				//temp
				u8 pallete_index = pallete_number * 8;
				u16 pallete_entry;
				
                if((high & bit) && (low & bit)) {
                    pallete_entry = sprite_pallete[pallete_index + 6];
                    pallete_entry |= (sprite_pallete[pallete_index + 7] << 8);
                    color = SDL_MapRGB(surface->format, 0,0,0); //temp
                } else if(high & bit) {
                    pallete_entry = sprite_pallete[pallete_index + 4];
                    pallete_entry |= (sprite_pallete[pallete_index + 5] << 8);
                    color = SDL_MapRGB(surface->format, 96,96,96); //temp
                } else if(low & bit) {
                    pallete_entry = sprite_pallete[pallete_index + 2];
                    pallete_entry |= (sprite_pallete[pallete_index + 3] << 8);
                    color = SDL_MapRGB(surface->format, 192,192,192); //temp
                } else {
                    // pallete_entry = pallete[pallete_index + 0];
                    // pallete_entry |= (pallete[pallete_index + 1] << 8);		
                    // white is transparant
                    continue;
                    //color = SDL_MapRGB(surface->format, 255,255,255); //temp
                }
                
                u8 r = (pallete_entry >> 0 ) & 0x1F;
                u8 g = (pallete_entry >> 5 ) & 0x1F;
                u8 b = (pallete_entry >> 10) & 0x1F;
                
                // color = SDL_MapRGB(surface->format, r*8, g*8, b*8);
                
                int x_pixel = x_position + sprite_pixel;
                
                if(scanline < 0 || scanline > 143 || x_pixel < 0 || x_pixel > 159) {
                    continue;
                }
				
                int position = x_pixel + (scanline * 160);
			
				/*
                u32 *pixmem32 = (u32*)surface->pixels+position;
                *pixmem32 = color;
				*/
				
				screen_buffer[scanline][x_pixel][0] = r;
				screen_buffer[scanline][x_pixel][1] = g;
				screen_buffer[scanline][x_pixel][2] = b;
                
            }
            
        }        
    }
}

int render_sprites() 
{
       return render_sprites_new();

	   
	   
	   
       SDL_UpdateRect(surface, 0, 0, 0, 0); 
       return 0;

	u8 sprite_mode = SPRITE_MODE_8X8;
	if(hardware_registers[LCDC] & OBJ_SIZE) {
		sprite_mode = SPRITE_MODE_8x16;
	}
	
	const u8 sprite_count = 40;
	const u16 oam_index = 0xFE00;
	
	u16 current_index = oam_index;
	int i;
	
	for(i = 0; i < sprite_count; ++i) {
	
		u8 y_position = MBC_read(current_index++) - 16;
		u8 x_position = MBC_read(current_index++) - 8;
		u8 tile_number = MBC_read(current_index++);
		u8 tile_attributes = MBC_read(current_index++);
		
		if(y_position == -16 || y_position >= 144)
			continue;
		if(x_position == -8  || x_position >= 160)
			continue;
			
		int x_tile = tile_number % 32;
		int y_tile = tile_number / 32;
		u16 offset = (tile_attributes & TILE_VRAM_BANK_NUMBER) ? 0x2000 : 0;		
		
		bool x_flip = false;
		if(tile_attributes & HORIZONTAL_FLIP) {
			x_flip = true;
		}
		
		u16 pixels = 8 * 8;
		if(sprite_mode == SPRITE_MODE_8x16) {
			pixels *= 2;
		}
		
		u8 sprite_height = 8;
		if(sprite_mode == SPRITE_MODE_8x16) {
			sprite_height = 16;
		}
		
		int j;
		for(j = 0; j < pixels; ++j) {
			int index = tile_number * 16 + (j / 8)*2;
		
			s8 x_bit = j % 8;
			if(x_flip) {
				//x_bit = 8 % (x_bit + 1);
				x_bit -= 7;
				x_bit *= -1;
			}
			int y_bit = j / 8; //this will need to be modified for 8x16

			int x_location = x_position + x_bit;
			int y_location = y_position + y_bit;
		
			u8 high = gameboy_vram[offset + index + 1];
			u8 low  = gameboy_vram[offset + index + 0];
		
			u8 bit = 0x80 >> (j % 8);
			u32 color;
			u16 pallete_entry;			
			u8 r, g, b;			
			
			int num;
			if((high & bit) && (low & bit)) {
				num = 3;
			} else if(high & bit) {
				num = 2;
			} else if(low & bit) {
				num = 1;
			} else {
				num = 0;
			}
			
			switch(num) {
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
				{
					//pallete_entry = pallete[pallete_index + 0];
					//pallete_entry |= (pallete[pallete_index + 1] << 8);		
					color = SDL_MapRGB(surface->format, 255,255,255); //temp
					continue;
					break;
				}
				default:
					printf("pallete Error!!!\n");
					getchar();
			}

			r = (pallete_entry >> 0 ) & 0x1F;
			g = (pallete_entry >> 5 ) & 0x1F;
			b = (pallete_entry >> 10) & 0x1F;
			//color = SDL_MapRGB(surface->format, r*8, g*8, b*8);
			
			if(x_location > 160 || y_location > 144)
				continue;
			
			int position = x_location  + y_location * 160;
			
			u32 *pixmem32 = (u32*)surface->pixels+position;
			*pixmem32 = color;
		}
	}
	SDL_UpdateRect(surface, 0, 0, 0, 0); 
}



int render_background_old()  //WORKS don't delete
{

	const u16 background_tile_count = 1024; //32 * 32
	//const u16 sprite_tile_offset = 0x8000;
    
    u8 lcd_control = hardware_registers[LCDC];
	
	u8 scroll_x = hardware_registers[SCX];
	u8 scroll_y = hardware_registers[SCY];
	
    u8 window_x = hardware_registers[WY];
    u8 window_y = hardware_registers[WX];
    
    bool render_window = false;
    if(lcd_control & WINDOW_DISPLAY_ENABLE) {
        render_window = true;
    }
    
	int i;
	for(i = 0; i < background_tile_count; i++) {
		
		//Get start location of tile map
		u16 offset = (lcd_control & BG_TILE_MAP_DISPLAY_SELECT) ? 0x400 : 0;
        if(render_window) {
            offset = (lcd_control & WINDOW_TILE_MAP_DISPLAY_SELECT) ? 0x400 : 0;
        }
		//u16 offset = 0;

		u8 tile_number = gameboy_vram[0x1800 + i + offset];
		u8 tile_attributes = gameboy_vram[0x1800 + i + offset + 0x2000];
		
		u8 pallete_number = tile_attributes & BACKGROUND_PALETTE_NUMBER;
		u8 pallete_index = pallete_number * 8;
		
		int x_tile = tile_number % 32;
		int y_tile = tile_number / 32;
		
		//Get the VRAM bank of the tile
		bool signed_index = false;
		offset = (tile_attributes & TILE_VRAM_BANK_NUMBER) ? 0x2000 : 0;
		//u8 offset = 0;
			
		if((hardware_registers[LCDC] & BG_WINDOW_TILE_DATA_SELECT) == 0) {
			signed_index = true;
			offset += 0x800;
		}
		
		
		int j;
		for(j = 0; j < 64; j++) {
			//64 pixels per tile
			
			//16 bytes per tile
			int index;
			if(signed_index) {
				index = (((s8)tile_number +128)*16) + (j/8)*2;
			} else {
				index = tile_number * 16 + (j / 8)*2;
			}
			
	
			int x_bit = j % 8;
			int y_bit = j / 8;

			int x_location = i % 32 * 8 + x_bit;
			int y_location = i / 32 * 8 + y_bit;
			
			
			u8 high = gameboy_vram[index + offset + 1];
			u8 low  = gameboy_vram[index + offset + 0];
			
			u8 bit = 0x80 >> (j % 8);
			u32 color;
			
			u16 pallete_entry;			
			u8 r, g, b;			
			
			int num;
			if((high & bit) && (low & bit)) {
				num = 3;
			} else if(high & bit) {
				num = 2;
			} else if(low & bit) {
				num = 1;
			} else {
				num = 0;
			}
			
			
			//switch(((high & bit) >> (6-j%8)) | ((low & bit) >> (7-j%8))) {
			switch(num) {
				case 3:
					pallete_entry = pallete[pallete_index + 6];
					pallete_entry |= (pallete[pallete_index + 7] << 8);
					break;
				case 2:
					pallete_entry = pallete[pallete_index + 4];
					pallete_entry |= (pallete[pallete_index + 5] << 8);
					break;
				case 1:
					pallete_entry = pallete[pallete_index + 2];
					pallete_entry |= (pallete[pallete_index + 3] << 8);
					break;
				case 0:
				{
					pallete_entry = pallete[pallete_index + 0];
					pallete_entry |= (pallete[pallete_index + 1] << 8);					
					break;
				}
				default:
					printf("pallete Error!!!\n");
					getchar();
			}

			r = (pallete_entry >> 0 ) & 0x1F;
			g = (pallete_entry >> 5 ) & 0x1F;
			b = (pallete_entry >> 10) & 0x1F;
			color = SDL_MapRGB(surface->format, r*8, g*8, b*8);
			
			
			
			/*
			if((high & bit) && (low & bit)) {
				//black //3
				r = pallete[pallete_index + 6] & 0x1F;
				//b = 0;
				b = (pallete[pallete_index + 6] & 0xE0) >> 5;
				b |= ((pallete[pallete_index + 7] & 0x02) << 5);
				g = (pallete[pallete_index + 7] & 0x7C) >> 2;
				//printf("r:%x g:%x\n", r, g);
				//printf("3 ");
				color = SDL_MapRGB(surface->format, r*8,g*8,b*8);
				
			} else if(high & bit) {
				//grey //2
				r = pallete[pallete_index + 4] & 0x1F;
				//b = 0;
				b = (pallete[pallete_index + 4] & 0xE0) >> 5;
				b |= ((pallete[pallete_index + 5] & 0x02) << 5);
				g = (pallete[pallete_index + 5] & 0x7C) >> 2;
				color = SDL_MapRGB(surface->format, r*8,g*8,b*8);
				//printf("2 ");
				//color = SDL_MapRGB(surface->format, 96,96,96);
			} else if(low & bit) {
				//grey //1
				//printf("1 ");
				r = pallete[pallete_index + 2] & 0x1F;
				//b = 0;
				b = (pallete[pallete_index + 2] & 0xE0) >> 5;
				b |= ((pallete[pallete_index + 3] & 0x02) << 5);
				g = (pallete[pallete_index + 3] & 0x7C) >> 2;
				color = SDL_MapRGB(surface->format, r*8,g*8,b*8);
				//printf("r:%x g:%x\n", r, g);
				//color = SDL_MapRGB(surface->format, 192,192,192);
			} else {
				//white //0
				r = pallete[pallete_index + 0] & 0x1F;
				//b = 0;
				b = (pallete[pallete_index + 0] & 0xE0) >> 5;
				b |= ((pallete[pallete_index + 1] & 0x02) << 5);
				g = (pallete[pallete_index + 1] & 0x7C) >> 2;
				color = SDL_MapRGB(surface->format, r*8,g*8,b*8);
				//C
			}
			*/

			
			
			int left_x_bounds = scroll_x;
			int right_x_bounds = scroll_x + 160;
			int top_y_bounds = scroll_y;
			int bottom_y_bounds = scroll_y + 144;
			
			/*
			if(x_location == left_x_bounds || x_location == right_x_bounds) {
				color = SDL_MapRGB(surface->format, 0,0,0);
			} else if(scroll_y == y_location || (scroll_y + 144) == y_location) {
				color = SDL_MapRGB(surface->format, 0,0,0);
			}
			*/
			
			if(x_location > right_x_bounds || x_location < left_x_bounds)
				continue;//color = SDL_MapRGB(surface->format, 0,0,0);
					
			
			if(y_location > bottom_y_bounds || y_location < top_y_bounds) 
				continue;//color = SDL_MapRGB(surface->format, 0,0,0);
			
			
			//int position = (i%32) * 8 + x_bit + (i/32) * 8 * 256 + y_bit * 256;
			int position = (x_location-scroll_x)  + ((y_location-scroll_y) * 160);
			//x_location -= scroll_x;

			
			//int position = x_location  + (y_location * 256);
			
			u32 *pixmem32 = (u32*)surface->pixels+position;
			*pixmem32 = color;


		}
	}
	//disable this because we are going to update the screen
	//after rendering the sprites
	//SDL_UpdateRect(surface, 0, 0, 0, 0); 
}




int setup_gui() 
{
	dprintf("Creating UI thread...");
	SDL_Thread *thread;
	thread = SDL_CreateThread(GUI, NULL);
	if(thread == NULL) {
		fprintf(stderr, "Unable to create UI thread (%s)\n", SDL_GetError());
		fatal_error();
	}
	dprintf("Done (%X)\n", thread);
	return 0;
}


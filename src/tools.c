#include "tools.h"

unsigned int last_error = 0;

const int ERROR_FILE_NOT_FOUND = 1;
const int ERROR_MAX_SIZE_EXCEEDED = 2;
const int ERROR_FILE_NOT_READ = 3;

bool debug_mode = true;

const char error_message[][32] = {
"ERROR_NONE",
"ERROR_FILE_NOT_FOUND",
"ERROR_MAX_SIZE_EXCEEDED",
"ERROR_FILE_NOT_READ"
};

//Use just like printf in situations where temporary output is neccessary
void tprintf( const char* format, ... )
{
	va_list args;
	fprintf(stderr, "Temp: ");
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
}

//input: size in bytes
//output: string with size in B, KB, or MB
char size_as_string[32];
char *size_to_string(int size) 
{
	char  B[] = " B";
	char KB[] = " KB";
	char MB[] = " MB";
	char* unit = B;
	static const int limit = 1024 * 10;
	if(size > limit) {
		size /= 1024;
		if(size > limit) {
			size /= 1024;
			unit = MB;
		} else {
			unit = KB;
		}		
	}
	itoa(size, size_as_string, 10);
	strncat(size_as_string, unit, 32 - strlen(size_as_string));
	return size_as_string;
}

char *get_last_error() 
{
	if(last_error > ((sizeof(error_message) / sizeof(*error_message)) - 1)) {
		fprintf(stderr, "Error getting error???\n");
		return NULL;
	}
	return (char*)error_message[last_error];
}

void fatal_error() 
{
	fprintf(stderr, "cgbemu encountered a fatal error and must terminate\n");
	fprintf(stderr, "press enter to finish\n");
	getchar();
	exit(last_error);
}

//input:
//	filepath: the file to read
//	dest: a pointer to a u8 array
//	max_size: the max number of bytes to read
//output: The total number of bytes read	
unsigned int binary_read_file( char* filepath, u8* dest, int max_size ) {
	
	FILE *file = fopen(filepath, "rb");
	if(!file) {
		last_error = ERROR_FILE_NOT_FOUND;
		return 0;
	}

	fseek(file, 0, SEEK_END);
	int	size = ftell(file);
	rewind(file);
	
	//do not continue if the size exceeds the maximum size
	if(size > max_size) {
		last_error =  ERROR_MAX_SIZE_EXCEEDED;
		fclose(file);
		return 0;
	}
	
	//TEMP
	
	
	if(fread(dest, sizeof(*dest), size, file) == 0) {
		last_error = ERROR_FILE_NOT_READ;
		fclose(file);
		return 0;
	}
	
	fclose(file);
	return size;
	
}
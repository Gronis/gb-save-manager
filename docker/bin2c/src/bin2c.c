#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fputs(
			"Transform a binary file into a c compatible header\n"
			"Usage:\n"
			"        bin2c <file> <identifier>\n\n"
			""
			"<file>       - binary file to include. Use \"-\" for stdin\n"
			"<identifier> - name of identifier in header used to access data. \n"
			"               will also add <identifier>_length for accessing\n"
			"               length of the file in bytes\n\n"
			""
			"Code will print to stdout\n"
			"\n",
			stderr);
		return 1;
	} else {
		FILE *f = !strcmp(argv[1], "-")? stdin : fopen(argv[1], "rb");
		if (f == NULL) {
			fprintf(stderr,"Unable to open input file '%s'", argv[1]);
			return errno;
		}
		unsigned char i;
		size_t bytes_size = 1024;
		size_t bytes_to_read = bytes_size;
		unsigned char *bytes = malloc(bytes_size);
		size_t bytes_read = fread(bytes, 1, bytes_to_read, f);
		unsigned int size = bytes_read;
		while (bytes_read > 0) {
			if(bytes_read == bytes_to_read){
				size_t read_offset = bytes_size;
				bytes_to_read = bytes_size;
				bytes_size *= 2;
				bytes = realloc(bytes, bytes_size);
				bytes_read = fread(bytes + read_offset, 1, bytes_to_read, f);
				size += bytes_read;
			} else {
				bytes_read = 0;
			}
		}
		fclose(f);
		size--;

		for (; size != -1 && (bytes[size] == 0xFF); --size);
		size++;

		char* var_name = argv[2];
		for (unsigned int i = 0; var_name[i] != 0; ++i){
			if (var_name[i] == '-'){
				var_name[i] = '_';
			}
		}

		// fprintf(stderr, "Using name: %s.\n", var_name);

		printf("#define %s_length %u\n",var_name,size);
		printf("const uint8_t %s[%s_length] = {\n    ",var_name, var_name);

		for (size_t i = 0; i < size; ++i){
			char *separator = i + 1 == size? "": ",";
			printf("0x%02X%s ", bytes[i], separator);
			if (i % 8 == 7) {
				printf("\n    ");
			}
		}

		free(bytes);
		printf("\n};\n");
	}

}

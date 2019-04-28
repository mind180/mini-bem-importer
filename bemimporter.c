#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bemimporter.h"
extern char *optarg;//аргумент опции
extern int optind, // текущий индекс argv
		   opterr, // !=0-сама выводит сообщение, 0-возврощает '?'
		   optopt; // значение нераспознаной функции
//если впереди идёт : подавляет вывод про ошибку независимо от opterr

void showHelp(){
  printf(" About\n");
  printf("-e | -e entry_point_path		entry point.\n");
  printf("-o | -o 	                  output.\n");
  printf("-x | -x extension_name      extension for files.\n");
  //printf("-i | -i import_template     string for including (default = @import ).\n");
  printf("-a | -a 	                  author name.\n");
  printf("-h | -h 	                  help.\n");
}


int main( int argc, char *argv[] )
{
	int option;
	opterr = 0;

  struct params {
    char* entry;
    char* output;
    char* ext;
  } params;

	while( (option=getopt(argc, argv, "ho:e:x:i:"))!=-1 )
	{

		switch(option)
		{
			case 'h':
        showHelp();
				break;

			case 'a':
				printf("Pilipenko Alexander (programm for own purpose) \n");
				break;

			case 'e':
				params.entry = optarg;
				break;

			case 'x':
        params.ext = optarg;
				break;

      case 'i':
          //reserved
  				break;

      case 'o':
      		params.output = optarg;
      		break;

			case '?':
        printf ("Unknown option: '%c'\n",(unsigned char) optopt);
				return EXIT_FAILURE;
				break;

			default:
        showHelp();
				break;
		}
	}
	
  importFiles( params.entry, params.output, params.ext );


	return EXIT_SUCCESS;
}

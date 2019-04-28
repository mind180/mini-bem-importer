#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

extern char *optarg;//аргумент опции
extern int optind, // текущий индекс argv
		   opterr, // !=0-сама выводит сообщение, 0-возврощает '?'
		   optopt; // значение нераспознаной функции
//если впереди идёт : подрвляет вывод про ошибку независимо от opterr

// открывает директорию и считает файлы в ней,
//если больше 2х(./..), то возвращает true
int isDirectoryEmpty(char *dirname) {
  int n = 0;
  struct dirent *d;
  DIR *dir = opendir(dirname);
  if (dir == NULL) //Not a directory or doesn't exist
    return 1;
  while ((d = readdir(dir)) != NULL) {
    if(++n > 2)
      break;
  }
  closedir(dir);
  if (n <= 2) //Directory Empty
    return 1;
  else
    return 0;
}


int extIndex(char* filename, int fLenth)
{
  int extBeg;

  for(extBeg = fLenth; extBeg >= 0; extBeg--){
    if( filename[extBeg] == '.' ){
      extBeg++;
      break;
    }
  }

  return extBeg;
}

char* getExtension(char* filename, int len, int extIndex){
	int p;
  char* ext = (char*)malloc(len - extIndex + 1);

	p = 0;
  for(int i = extIndex; i < len; i++, p++ ){
    ext[p] = filename[i];
  }
	ext[p] = '\0';
  return ext;
}


int extcmp(char* filename, char *extension){
  int fLenth = 0;
  int extBeg;
  char *ext = NULL;

  fLenth = strlen(filename);
  if( !fLenth )
    return -1;

  extBeg = extIndex(filename, fLenth);
  if( extBeg == -1) return -1;

  ext = getExtension(filename, fLenth, extBeg);

  return strcmp( ext, extension);
}

void addImportToOutput( char* filename, int fd ){
	char importPath[1024] = "";
	strcat( importPath, "@import \'");
	strcat( importPath, filename );
	strcat( importPath, "\';\n");
	write(fd, importPath, strlen(importPath));
}

void importCollector( char * dirname, char* ext, int fd )
{
		static int level = 0;//fast patch, exclude file from entry point
		level++;
		DIR *dir;
    struct dirent *entry;
		char fullpath [1024];

    dir = opendir( dirname );
    if (!dir) {
        perror("diropen");
        exit(1);
    };

    while ( (entry = readdir(dir)) != NULL)
		{
      if( entry->d_type == DT_DIR )
			{
				if( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") ) continue;
				fullpath[0] = '\0';
				strcat(fullpath, dirname);
				strcat(fullpath, "/");
				strcat(fullpath, entry->d_name );
				importCollector( fullpath, ext, fd);
				level--;
      }
      else if(entry->d_type == DT_REG)
			{
				if(level == 1 ) { continue;}//skip level 1
			
				if( extcmp( entry->d_name, ext) == 0 )
				{
					fullpath[0] = '\0';
					strcat(fullpath, dirname);
					strcat(fullpath, "/");
					strcat(fullpath, entry->d_name );
					addImportToOutput(fullpath, fd);
				}
      }
    }

    closedir(dir);
}


void importFiles(char *entry, char *output, char *ext)
{
  int fd;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  fd = creat(output, mode);

	importCollector(entry, ext, fd);

	close(fd);
}

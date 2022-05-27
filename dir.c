#include  <stdio.h>
#include  <sys/types.h>
#include <string.h>
#include  <dirent.h>

void recursive_list_dirs(char dirname[])
{
	DIR 	       *dir_ptr;
	struct 	dirent *direntp;
	char buf[1024];

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
			fprintf(stderr, "cannot open %s \n",dirname);
	else 
	{
		while ( ( direntp=readdir(dir_ptr) ) != NULL )
		{
			if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) continue;
			memset(buf, 0, sizeof(buf));
			// item is not a directory
			if(direntp->d_type != DT_DIR) {
				sprintf(buf, "%s/%s", dirname, direntp->d_name); // insert in queue
				printf("%s\n", buf) ;
			}
			else { // item is a directory
				char sub_dir[1024];
				sprintf(sub_dir, "%s/%s", dirname, direntp->d_name);
				recursive_list_dirs(sub_dir);

			}
		}
		closedir(dir_ptr);
	}
}

int main(int argc, char *argv[]) {

if (argc == 1 ) 
	recursive_list_dirs(".");
else 
	while ( --argc ){
		printf("%s: \n", *++argv ) ;
		recursive_list_dirs(*argv);
	}
}	



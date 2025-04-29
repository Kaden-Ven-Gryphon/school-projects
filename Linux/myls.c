/*Kaden Gryphon*/
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT 1024

int main(int argc, char *argv[])
{
	char path[MAX_INPUT];
	char workingDir[MAX_INPUT];
	/*uses a single token ls*/
	if(argc == 1)
	{
		DIR *dir;
		struct dirent *entry;
		int files = 0;
		
		
		getcwd(workingDir, MAX_INPUT);
		strcat(workingDir, "/");
		printf("cwd: %s\n", workingDir);
		
		/*attempt to open dir*/
		dir = opendir(workingDir);
		if(dir == NULL)
		{
			printf("unable to open current directory\n");
			return 1;
		}
		/*if it opens loop through and print files*/
		while( (entry=readdir(dir)) )
		{
			if(entry->d_name[0] == '.') continue;
			/*for each entry get to total path*/
			struct stat buf;
			strcpy(path, workingDir);
			strcat(path, entry->d_name);
			/*get the status of that file*/
			if(stat(path, &buf) < 0)
			{
				printf("unknown error\n");
			}
			else
			{
				/*determine if it is a file or a dir and print it to screen*/
				if(!S_ISDIR(buf.st_mode))
				{
					printf("File %3d: %s\n", files, entry->d_name);
				}
				else
				{
					printf("Dir  %3d: %s\n", files, entry->d_name);
				}
			}
			files++;
			
		}
		closedir(dir);
		
	}
	else if(argc == 2)
	{
		/*if the all flag is set*/
		if(strcmp(argv[1], "-a") == 0)
		{
			DIR *dir;
			struct dirent *entry;
			int files = 0;
			
			
			getcwd(workingDir, MAX_INPUT);
			strcat(workingDir, "/");
			printf("cwd: %s\n", workingDir);
			
			/*attempt to open dir*/
			dir = opendir(workingDir);
			if(dir == NULL)
			{
				printf("unable to open current directory\n");
				return 1;
			}
			/*if it opens loop through and print files*/
			while( (entry=readdir(dir)) )
			{
				/*for each entry get to total path*/
				struct stat buf;
				strcpy(path, workingDir);
				strcat(path, entry->d_name);
				/*get the status of that file*/
				if(stat(path, &buf) < 0)
				{
					printf("unknown error\n");
				}
				else
				{
					/*determine if it is a file or a dir and print it to screen*/
					if(!S_ISDIR(buf.st_mode))
					{
						printf("File %3d: %s\n", files, entry->d_name);
					}
					else
					{
						printf("Dir  %3d: %s\n", files, entry->d_name);
					}
				}
				files++;
				
			}
			closedir(dir);
		}
		else
		{
			DIR *dir;
			struct dirent *entry;
			int files = 0;
			
			
			strcpy(workingDir, argv[1]);
			strcat(workingDir, "/");
			printf("cwd: %s\n", workingDir);
			
			/*attempt to open dir*/
			dir = opendir(workingDir);
			if(dir == NULL)
			{
				printf("unable to open current directory\n");
				return 1;
			}
			/*if it opens loop through and print files*/
			while( (entry=readdir(dir)) )
			{
				if(entry->d_name[0] == '.') continue;
				/*for each entry get to total path*/
				struct stat buf;
				strcpy(path, workingDir);
				strcat(path, entry->d_name);
				/*get the status of that file*/
				if(stat(path, &buf) < 0)
				{
					printf("unknown error\n");
				}
				else
				{
					/*determine if it is a file or a dir and print it to screen*/
					if(!S_ISDIR(buf.st_mode))
					{
						printf("File %3d: %s\n", files, entry->d_name);
					}
					else
					{
						printf("Dir  %3d: %s\n", files, entry->d_name);
					}
				}
				files++;
				
			}
			closedir(dir);
		}
	}
	else if(argc == 3)
	{
		int pathIndex = 0;
		/*if the all flag is set*/
		if(strcmp(argv[1], "-a") == 0) pathIndex = 2;
		else if(strcmp(argv[2], "-a") == 0) pathIndex = 1;
		else
		{
			printf("%s\n", "\'myls\' [-a] usage ls\tlist files");
			return 1;
		}

		DIR *dir;
		struct dirent *entry;
		int files = 0;
		
		
		strcpy(workingDir, argv[pathIndex]);
		strcat(workingDir, "/");
		printf("cwd: %s\n", workingDir);
		
		/*attempt to open dir*/
		dir = opendir(workingDir);
		if(dir == NULL)
		{
			printf("unable to open current directory\n");
			return 1;
		}
		/*if it opens loop through and print files*/
		while( (entry=readdir(dir)) )
		{
			/*for each entry get to total path*/
			struct stat buf;
			strcpy(path, workingDir);
			strcat(path, entry->d_name);
			/*get the status of that file*/
			if(stat(path, &buf) < 0)
			{
				printf("unknown error\n");
			}
			else
			{
				/*determine if it is a file or a dir and print it to screen*/
				if(!S_ISDIR(buf.st_mode))
				{
					printf("File %3d: %s\n", files, entry->d_name);
				}
				else
				{
					printf("Dir  %3d: %s\n", files, entry->d_name);
				}
			}
			files++;
			
		}
		closedir(dir);
	}
	else
	{
		printf("%s\n", "\'myls\' [-a] usage ls\tlist files");
	}	
	return 0;
}

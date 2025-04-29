/*Kaden Gryphon*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

#define MAX_INPUT 1024
#define MAX_ARGS 100
#define MAX_HISTORY 10

#define clear() printf("\033[H\033[J")

int Exit = FALSE;
char workingDir[MAX_INPUT];
char PS1[32];


/*funciton definitions*/
void initShell();
int getInput(char* inputStr);
int builtInCmdHandler(char** parsed, int count);
int parse(char* inputStr, char** parsed);
int addToHistory(char* inputStr, char** hist);

/*The list of comand names, and usages*/
#define NUMBER_OF_BI_COMANDS 11
char ComandList[NUMBER_OF_BI_COMANDS][20] = {
							"echo",
							"export",
							"cat",
							"cp",
							"rm",
							"mkdir",
							"rmdir",
							"exit",
							"help",
							"cd",
							"ls"
						};
char ComandUsage[NUMBER_OF_BI_COMANDS][500] = {
							"\'echo\' usage:  echo [-n] <text>\t-n option to remove linefeed",
							"\'export\' usage: export <variable>=<value>\tthis only works on PS1",
							"\'cat\' usage: cat <filename>\tprints file to screen\tUse \"s for filenames with spaces",
							"\'cp\' usage: cp <SOURCE> <DESTINATION>\tcopyfile\tUse \"s for filenames with spaces",
							"\'rm\' usage: rm <filename>\tremoves file\tUse \"s for filenames with spaces",
							"\'mkdir\' usage mkdir <dirname>\t make directory\tUse \"s for filenames with spaces",
							"\'rmdir\' usage rmdir <dirname>\t remove directory\tUse \"s for filenames with spaces",
							"\'exit\' usage exit\tclose shell",
							"\'help\' usage help\tlist avalible comands",
							"\'cd\' usage cd <dirname>\tchange working directroy\tUse \"s for filenames with spaces",
							"\'ls\' usage ls\tlist files"
						};
/*THe command funciton defintions*/
int mysh_echo(char **args, int count);
int mysh_export(char **args, int count);
int mysh_cat(char **args, int count);
int mysh_cp(char **args, int count);
int mysh_rm(char **args, int count);
int mysh_mkdir(char **args, int count);
int mysh_rmdir(char **args, int count);
int mysh_exit(char **args, int count);
int mysh_help(char **args, int count);
int mysh_cd(char **args, int count);
int mysh_ls(char **args, int count);


/*main loop*/
int main() {
	char inputString[MAX_INPUT];
	/*char history[MAX_HISTORY][MAX_INPUT];*/
	/*history[0][0] = '\0';*/
	int my_argc;
	char* my_argv[MAX_ARGS];
	
	strcpy(PS1, "$ ");
      
	initShell();
	
	while(1)
	{
		/*Print the prompt, the working directory, and the PS1 character*/
		printf("%s%s", workingDir, PS1);
		/*get the input from the terminal and set inputString to that input*/
		getInput(inputString);
		/*if inputString is not empty, parse it*/
		if(inputString[0] != '\n')
		{
			/*set my_argv to the list of tokens found in inputString*/
			my_argc = parse(inputString, my_argv);
			/*if there are tokens*/
			if(my_argv != NULL)
			{
				/*pase the tokens and run the type command*/
				builtInCmdHandler(my_argv, my_argc);
				
		
			}
		}
		if(Exit) break;
	}/*while(1)*/
	return 0;
}


/*prints the welcom message*/
void initShell()
{
      clear();
      printf("\n\n");
      printf("____________________________\n");
      printf("      Kaden's Shell         \n");
      printf("____________________________\n\n\n");
      getcwd(workingDir, MAX_INPUT);
      strcat(workingDir, "/");
      /*chdir("/");*/
}

/*takes the user input and puts it into inputStr*/
int getInput(char* inputStr)
{
	fgets(inputStr, MAX_INPUT, stdin);
	/*scanf("%s", inputStr);*/	
	/*getline(&inputStr, &Size, stdin);*/
	return 0;
}

/*takes the parsed input and determends which command it is*/
int builtInCmdHandler(char** parsed, int count)
{
	/*compare the first token with each of the commands names in the list
	  Untill a hit has been found, i will equal the index of the used command*/
	int i = 0;
	for(i = 0; i < NUMBER_OF_BI_COMANDS; i++)
	{
	
		if(strcmp(parsed[0], ComandList[i]) == 0)
		{
			
			break;
		}
	}
	
	/*use i to call the right command*/
	switch(i)
	{
	case 0:
		mysh_echo(parsed, count);
		break;
	case 1:
		mysh_export(parsed, count);
		break;
	case 2:
		mysh_cat(parsed, count);
		break;
	case 3:
		mysh_cp(parsed, count);
		break;
	case 4:
		mysh_rm(parsed, count);
		break;
	case 5:
		mysh_mkdir(parsed, count);
		break;
	case 6:
		mysh_rmdir(parsed, count);
		break;
	case 7:
		mysh_exit(parsed, count);
		break;
	case 8:
		mysh_help(parsed, count);
		break;
	case 9:
		mysh_cd(parsed, count);
		break;
	case 10:
		mysh_ls(parsed, count);
		break;
	case NUMBER_OF_BI_COMANDS:
		/*the loop at the begining of the funciton found no matches*/
		
		
		
		/*the command has an absulute path*/
		if(parsed[0][0] == '/')
		{
			/*check if the file is an exicutable*/
			struct stat sb;
			if(stat(parsed[0], &sb) == 0 && sb.st_mode & S_IXUSR)
			{
				/*create a child prosses*/
				if(fork()==0)
				{
					/*and run the program in the child*/
					parsed[count] = NULL;
					execv(parsed[0], parsed);
				}
				/*wait for child to die*/
				wait(NULL);
				break;
				
			}
			else
			{
				/*file is not exicutable*/
				printf("%s is not an exicutable file\n", parsed[0]);
			}
		}
		else
		{
			
			/*check for local path*/
			int hasSlash = 0;
			char * charPtr = parsed[0];
			while(*charPtr != '\0')
			{
				if(*charPtr == '/') hasSlash = 1;
				charPtr++;
			}
			
			/*the command is a local path*/
			if(hasSlash == 1)
			{
				/*get the absulute path*/
				char path[MAX_INPUT];
				strcpy(path, workingDir);
				strcat(path, parsed[0]);
				
				/*check if the file is an exicutable*/
				struct stat sb;
				if(stat(path, &sb) == 0 && sb.st_mode & S_IXUSR)
				{
					/*create a child prosses*/
					if(fork()==0)
					{
						/*and run the program in the child*/
						parsed[count] = NULL;
						execv(path, parsed);
					}
					/*wait for child to die*/
					wait(NULL);
					break;
					
				}
				else
				{
					/*file is not exicutable*/
					printf("%s is not an exicutable file\n", parsed[0]);
				}
			}
			/*else it does not have a slash and needs to check PATH*/
			else
			{
				int foundProgram = 0;
				/*create a string for stort the PATH variable*/
				char pathCopy[MAX_INPUT];
				/*creat point to the PATH envirment variable*/
				char *pathENVp;
				/*get pointer to PATH var and copy contents to string*/
				pathENVp = getenv("PATH");
				strcpy(pathCopy, pathENVp);
				
				/*loop through the dirs in the path and search for the program*/
				char *token = strtok(pathCopy, ":");
				while (token != NULL)
				{
					/*get the absulute path*/
						char path[MAX_INPUT];
						strcpy(path, token);
						strcat(path, "/");
						strcat(path, parsed[0]);
						
						/*check if the file is an exicutable*/
						struct stat sb;
						if(stat(path, &sb) == 0 && sb.st_mode & S_IXUSR)
						{
							foundProgram = 1;
							/*create a child prosses*/
							if(fork()==0)
							{
								/*and run the program in the child*/
								parsed[count] = NULL;
								execv(path, parsed);
							}
							/*wait for child to die*/
							wait(NULL);
							break;
							
						}
					token = strtok(NULL, ":");
				}
				if(foundProgram)
				{
				
				}
				else
				{
					printf("Comand not recognized: %s\n", parsed[0]);
					printf("%s\n", ComandUsage[8]);
				}
			}	
		}
	}
	
	
	return 0;
}

/*takes an input string and pases it into tokens.
  parsed will point at the array of tokens.  returns
  the number of tokens.
  This parser will ignore dillimters that are with in
  ""s. */
int parse(char* inputStr, char** parsed)
{

	
	
	int numberOfTokens = 0;
	int state = ' ';
	/*pointer to scan through the input string*/
	char* ptrChar = inputStr;
	
	/*while the end of string has not been reached*/
	while(*ptrChar != '\0')
	{
		switch(state)
		{
		case ' ':
		/*The parser is eating white space*/
		
			if(*ptrChar == '\"')/*found a "*/
			{
				/*found start of token that has "s*/
				parsed[numberOfTokens] = ptrChar+1;
				state = '\"';
			}
			else if (*ptrChar != ' ' && *ptrChar != '\t' && *ptrChar != '\n') /*switch to text*/
			{
				/*found start of normal token*/
				parsed[numberOfTokens] = ptrChar;
				state = 'T';
			}
			break;
		case 'T':
		/*The parser is collection text*/
			/*whitespace found, end of token*/
			if(*ptrChar == ' ' || *ptrChar == '\t' || *ptrChar == '\n')
			{
				*ptrChar = '\0';	/*seperate token into string*/
				numberOfTokens++;
				state = ' ';
			}
			else if(*ptrChar == '\"')
			{
				/*start of ignore dilimiters*/
				state = '\'';
				
				/*shift text one left to remove " character*/
				char* ptr = ptrChar;
				while(*ptr != '\0')
				{
					*(ptr) = *(ptr+1);
					ptr++;
				}
				
				ptrChar--;
			}
			break;
		case '\"':
		/*token that ignores dilimeters*/
			if(*ptrChar == '\"')
			{
				state = ' ';
				*ptrChar = '\0';
				numberOfTokens++;
			}
			break;
		case '\'':
		/*ignoreing dilimetes within token*/
			if(*ptrChar == '\"')
			{
				state = 'T';
				
				/*shift text one left to remove closing "*/
				char* ptr = ptrChar;
				while(*ptr != '\0')
				{
					*(ptr) = *(ptr+1);
					ptr++;
				}
				
				ptrChar--;
			}
			break;
		}
		ptrChar++;
	} 
	
	

	return numberOfTokens;
}

/*unused,  did not feel like rewriting the get input to
  capture arrow keys*/
int addToHistory(char* inputStr, char** hist)
{
	int i = 0;
	for(i = 1; i < MAX_HISTORY; i++)
	{
		hist[i] = hist[i-1];
	}
	strcpy(hist[0], inputStr);
	return 0;

}


/*echo command, takes option -n and tokens of text to print*/
int mysh_echo(char **args, int count)
{
	int flag_n = FALSE;
	/*there are not enough tokens to print*/
	if(count <2)
	{
		printf("%s\n", ComandUsage[0]);
	}
	else
	{
		/*loop through all the tokens*/
		int i = 1;
		for(i = 1; i < count; i++)
		{
			/*if it starts with a - it might be an option*/
			if(args[i][0] == '-')
			{
				/*if it is -n set the flage*/
				if(strcmp(args[i], "-n") == 0)
				{
					flag_n = TRUE;
					/*do not print option*/
					continue;
				}
				/*otherwise it is an unknown option. ignore it and print it as text*/
				else
				{
					/*unknow option*/
				}
				
			}
			/*print token*/
			printf("%s", args[i]);
			/*add a space after token if it is not the last one*/
			if(i < count -1) printf(" ");
			
		}
		/*unless the -n option has been found print linefeed*/
		if(!flag_n) printf("\n");
		
	}
	return 0;
}
/*export used to set varibles, only works on PS1 for this shell*/
int mysh_export(char **args, int count)
{
	/*there must be atleats two tokens, though anything after the second will be ignored*/
	if(count >= 2)
	{
		/*if the varible to be set is PS1*/
		if(args[1][0] == 'P' && args[1][1] == 'S' && args[1][2] == '1' && args[1][3] == '=')
		{
			/*if the next char is \0 then there was a space after PS1= so set the varible to " "*/
			if(args[1][4] == '\0') strcpy(PS1, " ");
			/*else copy the rest of the stirng into PS1*/
			else strcpy(PS1, (args[1] + 4));
		}
		else
		{
			/*this shell only suports var PS1*/
			printf("unknown var(this shell only know the var PS1)\n%s\n", ComandUsage[1]);
		}
	}
	return 0;
}
/*print file to terminal*/
int mysh_cat(char **args, int count)
{
	/*needs two tokens, cat and filename*/
	if(count == 2)
	{
		
		FILE* fp;
		char ch;
		char path[MAX_INPUT];
		/*is relative adress, append input to working dir*/
		if(args[1][0] != '/')
		strcpy(path, workingDir);
		/*else is abslute path*/
		else path[0] = '\0';
		strcat(path, args[1]);
		
		/*open file*/
		fp = fopen(path, "r");
		
		/*file failed to open*/
		if(fp == NULL)
		{
			printf("file: %s not found\n%s\n", args[1], ComandUsage[2]);
		}
		else
		{
			/*else get chars from file and print them to terminal*/
			do
			{
				ch = fgetc(fp);
				printf("%c", ch);
			} while (ch != EOF);
			fclose(fp);
		}
		
	}
	else
	{
		printf("%s\n", ComandUsage[2]);
	}
	
	return 0;
}
/*copy file*/
int mysh_cp(char **args, int count)
{
	/*needs three tokens, cp, source, destination*/
	if(count == 3)
	{
		FILE* source;
		char ch;
		char path[MAX_INPUT];
		/*relitive pate*/
		if(args[1][0] != '/')
		strcpy(path, workingDir);
		/*absolute path*/
		else path[0] = '\0';
		strcat(path, args[1]);
		/*open source file*/
		source = fopen(path, "r");
		
		FILE* destination;
		/*relative path*/
		if(args[2][0] != '/')
		strcpy(path, workingDir);
		/*absulute path*/
		else path[0] = '\0';
		strcat(path, args[2]);
		/*open destination*/
		destination = fopen(path, "w");
		
		if(source == NULL)
		{
			printf("file %s not found\n%s\n", args[1], ComandUsage[3]);
			if(destination == NULL)
			{
				printf("destination could not be opened\n%s\n", ComandUsage[3]);
			}
			else fclose(destination);
		}
		else
		{
			if(destination == NULL)
			{
				printf("destination could not be opened\n%s\n", ComandUsage[3]);
			}
			else
			{
				/*if both files open, copy each char from source to desination*/
				do
				{
					ch = fgetc(source);
					fputc(ch, destination);
				} while (ch != EOF);
				fclose(destination);
			}
			fclose(source);
		}
		
	}
	else
	{
		printf("%s\n", ComandUsage[3]);
	}
	return 0;
}
/*removes a file*/
int mysh_rm(char **args, int count)
{
	/*takes two tokens, rm and filename*/
	if(count == 2)
	{
		char path[MAX_INPUT];
		/*relative path*/
		if(args[1][0] != '/')
		strcpy(path, workingDir);
		/*absulute path*/
		else path[0] = '\0';
		strcat(path, args[1]);
		/*attempt to remove file*/
		int i = remove(path);
		if(i == 0)
		{
			/*remove file secsessfuly*/
		}
		else
		{
			printf("file %s failed to remove, file might not exist\n%s\n", args[1], ComandUsage[4]);
		}
	}
	else
	{
		printf("%s\n", ComandUsage[4]);
	}
	return 0;
}
/*make a directory file*/
int mysh_mkdir(char **args, int count)
{
	/*two tokens mkdir and dir name*/
	if(count == 2)
	{
		char path[MAX_INPUT];
		/*relative path*/
		if(args[1][0] != '/')
		strcpy(path, workingDir);
		/*absulute path*/
		else path[0] = '\0';
		strcat(path, args[1]);
		
		
		errno = 0;
		/*attampt to create dir*/
		int sucsess = mkdir(path, S_IRWXU);
		/*if failed to create print error*/
		if(sucsess == -1)
		{
			switch(errno)
			{
			case EACCES:
				printf("No write acces in parent dir\n");
				break;
			case EEXIST:
				printf("dir already exists\n");
				break;
			default:
				printf("unknown error\n%s\n", ComandUsage[5]);
				break;
			}
		}
	}
	else
	{
		printf("%s\n", ComandUsage[5]);
	}
	return 0;
}
/*remove directory file*/
int mysh_rmdir(char **args, int count)
{
	/*two tokens rmdir and dir name*/
	if(count == 2)
	{
		char path[MAX_INPUT];
		/*relative path*/
		if(args[1][0] != '/')
		strcpy(path, workingDir);
		/*absulute path*/
		else path[0] = '\0';
		strcat(path, args[1]);
		
		
		errno = 0;
		/*attempt to remove dir*/
		int sucsess = rmdir(path);
		/*if it fails print error*/
		if(sucsess == -1)
		{
			switch(errno)
			{
			case EACCES:
				printf("No write acces in parent dir\n");
				break;
			case ENOTEMPTY:
				printf("dir is not empty\n");
				break;
			default:
				printf("unknown error\n%s\n", ComandUsage[5]);
				break;
			}
		}
	}
	else
	{
		printf("%s\n", ComandUsage[6]);
	}
	return 0;
}
/*sets exit flag to true so main loop ends*/
int mysh_exit(char **args, int count)
{
	Exit = TRUE;
	printf("Good Bye!\n");
	return 0;
}
/*prints the usage of all commands*/
int mysh_help(char **args, int count)
{
	int i = 0;
	for(i = 0; i < NUMBER_OF_BI_COMANDS; i++)
	{
		printf("%s\n", ComandUsage[i]);
	}
	return 0;
}
/*change working directory*/
int mysh_cd(char **args, int count)
{
	/*two tokens cd and dir name*/
	if(count == 2)
	{
		/*it and absulute path*/
		if(args[1][0] == '/')
		{
			/*atempt to open dir to see if it exisits*/
			DIR* dir = opendir(args[1]);
			if(dir)
			{
				/*if it does set the workiing dir to the input*/
				strcpy(workingDir, args[1]);
				closedir(dir);
				/*added this line below so that the working dir of child prossess like myls is set*/
				chdir(workingDir);
			}
			else if (ENOENT == errno)
			{
				printf("Directory not found\n");
				printf("%s\n", ComandUsage[9]);
			}
			else
			{
		
			}
			/*find the length of working dir*/
			int i = 0;
			while(workingDir[i] != '\0')
			{
				i++;
			}
			/*if it does not end with a / add one*/
			if(workingDir[i-1] != '/') strcat(workingDir, "/");
		}
		/*special case where relative path is . or ..*/
		else if(args[1][0] == '.')
		{
			/*if it is ../ or ..*/
			if(args[1][1] == '.' && ((args[1][2] == '/' && args[1][3] == '\0') || args[1][2] == '\0'))
			{
				/*find the length of working dir*/
				char* ptrChar = workingDir;
				int i = 0;
				while(workingDir[i] != '\0')
				{
					i++;
				}
				/*find the location of second to last / to remove the last directory in path*/
				int j = 0;
				for(j = 0; j < i - 2; j++)
				{
					if (workingDir[j] == '/') ptrChar = &workingDir[j+1];
				}
				*ptrChar = '\0'; /*remove last directory*/
				/*added this line below so that the working dir of child prossess like myls is set*/
				chdir(workingDir);
			}
			/*else the dir name is . or ./ ignore it*/
			/*just relized that this probluy also makes it ignore hidden directorys that start with .*/
		}
		else
		{
			/*else it is a normal relative path*/
			char path[MAX_INPUT];
			/*find the end of string if it is missing / add it*/
			int i = 0;
			while(args[1][i] != '\0')
			{
				i++;
			}
			i--;
			if(args[1][i] != '/') strcat(args[1], "/");
			
			
			strcpy(path, workingDir);
			strcat(path, args[1]);
			/*open dir to make sure it exitest*/
			DIR* dir = opendir(path);
			if(dir)
			{
				/*set the working dir to the path of the opened dir*/
				strcpy(workingDir, path);
				closedir(dir);
				/*added this line below so that the working dir of child prossess like myls is set*/
				chdir(workingDir);
			}
			else if (ENOENT == errno)
			{
				printf("Directory not found\n");
				printf("%s\n", ComandUsage[9]);
			}
			else
			{
		
			}
		}
		
	}
	else
	{
		printf("%s\n", ComandUsage[9]);
	}
	return 0;
}
/*list the files in working dir*/
int mysh_ls(char **args, int count)
{
	char path[MAX_INPUT];
	/*uses a single token ls*/
	if(count == 1)
	{
		DIR *dir;
		struct dirent *entry;
		int files = 0;
		
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
	else if(count == 2)
	{
		/*if the all flag is set*/
		if(strcmp(args[1], "-a") == 0)
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
	}
	else
	{
		printf("%s\n", ComandUsage[10]);
	}
	
	return 0;
}




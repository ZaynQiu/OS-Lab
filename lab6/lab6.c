#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

int src_file(char *src_path, char *des_path)
{
	// judge whether the des_path is a file or a directory
	struct stat buf;
	if (stat(des_path, &buf) < 0)
	{
		printf("%s\n", des_path);
		printf("stat error\n");
		return -1;
	}
	if(S_ISDIR(buf.st_mode))	// is a directory
	{
		printf("destination_path is a directory\n");
		// create a file in the directory, the name is same as the source file
		char *des_path_cpy = strdup(des_path);
		char *src_path_cpy = strdup(src_path);
		char *file_name = NULL;
		while (*src_path_cpy != '\0')
		{
			if (*src_path_cpy == '/')
			{
				file_name = src_path_cpy;
			}
			src_path_cpy++;
		}
		strcat(des_path_cpy, file_name); // combine des_path with the file name
		des_path = strdup(des_path_cpy);
		// src and des path
		printf("src_path: %s\n", src_path);
		printf("des_path: %s\n", des_path);
	}
	else if(S_ISREG(buf.st_mode))	// is a file
	{
		printf("destination_path is a file\n");
	}
	else
	{
		printf("destination_path is not a file or a directory\n");
		return -1;
	}

	// open files
	int src_fd = open(src_path, O_RDWR);
	if (src_fd == -1) // file error
	{
		printf("open src_file error\n");
		return -1;
	}
	int des_fd = open(des_path, O_RDWR | O_CREAT, 0777);
	if (des_fd == -1) // file error
	{
		printf("open des_file error\n");
		return -1;
	}

	// copy file
	char buffer[1024];
	int len = read(src_fd, buffer, 1024);
	while(len > 0)
	{
		if(write(des_fd, buffer, len) == -1)
		{
			printf("write error\n");
			return -1;
		}
		len = read(src_fd, buffer, 1024);
	}

	// close file
	close(src_fd);
	close(des_fd);
	return 1;
}

int src_dir(char *src_path, char *des_path)
{
	// get current directory of src_path
	int src_len = strlen(src_path), dir_len = 0;
	if (src_path[src_len - 1] == '/') // if the last char is '/', delete it
		src_path[--src_len] = '\0';
	while(src_path[src_len-dir_len] != '/')  // get the length of the directory name
		dir_len++;
	char *src_dir_name = (char *)malloc(dir_len+1);
	strncpy(src_dir_name, src_path+src_len-dir_len+1, dir_len);
	printf("src_dir_name: %s\n", src_dir_name);
	
	// create a new directory in des_path with the same name as in src_path
	int des_len = strlen(des_path);
	strcat(des_path, "/");
	strcat(des_path, src_dir_name);
	
	if (mkdir(des_path, 0777) == -1)
	{
		printf("mkdir error\n");
		return -1;
	}
	printf("after makedir des_path: %s\n", des_path);

	// open directory
	DIR *dir = opendir(src_path);
	if (dir == NULL)
	{
		printf("open dir error\n");
		return -1;
	}
	// read directory
	struct dirent *ptr;
	printf("1 des_path: %s\n", des_path);
	while((ptr = readdir(dir)) != NULL)
	{
		printf("d_name: %s\n", ptr->d_name);
		// ignore . and ..
		if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		char *src_path_cpy = (strcat(strdup(src_path), "/"));
		src_path_cpy = strcat(src_path_cpy, ptr->d_name);
		if(ptr->d_type == DT_REG)
		{
			src_file(src_path_cpy, des_path);
		}
		else if(ptr->d_type == DT_DIR)
		{
			char *des_path_cpy = strcat(des_path, "/");
			des_path_cpy = strcat(des_path_cpy, ptr->d_name);
			mkdir(des_path_cpy, 0777);
			src_dir(src_path, des_path_cpy);
		}
	}

	closedir(dir);
	return 1;
}

int copy_judge(char *src_path, char *des_path)
{
	// judge whether the path is a file or a directory
	struct stat buf;
	if (stat(src_path, &buf) < 0)
	{
		printf("stat error\n");
		return -1;
	}
	if (S_ISREG(buf.st_mode)) // is a file
	{
		printf("source_path is a file\n");
		src_file(src_path, des_path);
	}
	else if (S_ISDIR(buf.st_mode)) // is a directory
	{
		printf("source_path is a directory\n");
		src_dir(src_path, des_path);
	}
	else
	{
		printf("source_path is not a file or a directory\n");
		return -1;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	//input example : ./lab6 source_path destination_path
	for(int i=0; i<argc; i++)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	// if the last char is '/', delete it
	int src_len = strlen(argv[1]), des_len = strlen(argv[2]);
	if (argv[1][src_len - 1] == '/') 
		argv[1][--src_len] = '\0';
	if (argv[2][des_len - 1] == '/')
		argv[2][--des_len] = '\0';

	copy_judge(argv[1], argv[2]);
	
	return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int src_file(char *src_path, char *des_path)
{
	// judge whether the des_path is a file or a directory
	struct stat buf;
	if (stat(des_path, &buf) < 0)
	{
		printf("stat error\n");
		return -1;
	}
	if(S_ISDIR(buf.st_mode))	// is a directory
	{
		printf("destination_path is a directory\n");
		// create a file in the directory, the name is same as the source file
		char *des_path_cpy = des_path;
		char *src_path_cpy = src_path;
		char *file_name = src_path_cpy;
		while (*src_path_cpy != '\0')
		{
			if (*src_path_cpy == '/')
			{
				file_name = src_path_cpy;
			}
			src_path_cpy++;
		}
		strcat(des_path_cpy, file_name); // combine des_path with the file name
		des_path = des_path_cpy;
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
	int des_fd = open(des_path, O_RDWR | O_CREAT, 0666);
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
	// // get pwd
	// char pwd[1024];
	// getcwd(pwd, 1024);
	// printf("pwd: %s\n", pwd);

	// open directory
	DIR *dir = opendir(src_path);
	if (dir == NULL)
	{
		printf("open dir error\n");
		return -1;
	}

	// read directory
	struct dirent *ptr;
	while((ptr = readdir(dir)) != NULL)
	{
		printf("d_name: %s\n", ptr->d_name);
		// ignore . and ..
		if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		// ptr->d_type is directory
		if(ptr->d_type == DT_REG)
		{

		}
		else if(ptr->d_type == DT_DIR)
		{

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
	if (S_ISDIR(buf.st_mode)) // is a directory
	{
		printf("source_path is a directory\n");
		src_dir(src_path, des_path);
	}
	else if (S_ISREG(buf.st_mode)) // is a file
	{
		printf("source_path is a file\n");
		src_file(src_path, des_path);
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
	
	copy_judge(argv[1], argv[2]);
	
	return 0;
}


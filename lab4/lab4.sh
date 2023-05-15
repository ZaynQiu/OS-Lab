#!/bin/bash
# read the status of the given file and print it to the screen
clear
echo "Hello! Welcome to my file status checker!"
while :
do
	echo "Please enter the file name you want to check:"
	read file_name
	if [ -f $file_name ]
	then
		break
	else
		echo "The file does not exist! Please try again!"
	fi
done

check_times=1
change_times=0
file_size_new=$(stat -c %s $file_name) #use $() to get the output of the command
file_size_old=$file_size_new

echo "The given file is $file_name"
echo "The satus of the file is  \\n$(stat $file_name)"
echo "The size of the file is ${file_size_old} bytes"

while ((($check_times <= 20)) && (($change_times <= 1)))
do
	echo "The file has been checked $check_times times"
	check_times=$(( $check_times + 1 ))
	file_size_new=$(stat -c %s $file_name)
	if [ $file_size_new -ne $file_size_old ] #file size changed
	then
		change_times=$(( $change_times + 1 ))
		file_size_old=$file_size_new
		check_times=1
		echo "File $file_name size changed!"
		echo "Change times: $change_times"
		echo "The new size is: ${file_size_old}"
	fi
	sleep 1 #sleep 1 second
done

echo "Thank you for using the file status checker!"


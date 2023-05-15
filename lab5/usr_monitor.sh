#!/bin/bash
# to log in the given user
# check if just one argument is given
if [ $# -ne 1 ]
then
	echo "Usage: $0 username"
	exit 1
fi
#check all the log in users
echo "The current log in users are:"
find_user=0
username=$(who)
while read -r line
do
	each_user=$(echo "$line" | awk  '{print $1}')
	echo " $each_user "
	if [[ $each_user = $1 ]]
	then
		find_user=1
		continue
	fi
done <<< "$username"

if [ $find_user == 1 ]
then
	echo "The user $1 is  logon!"
	exit 1
fi

echo "waiting user $1 logon..."
#check if given user is log in
while :
do
	username=$(who)
	while read -r line
	do
		each_user=$(echo "$line" | awk  '{print $1}')
		if [[ $each_user = $1 ]]
		then
			echo "The user $1 is logon!"
			exit 1
		fi
	done <<< "$username"
	sleep 5
done

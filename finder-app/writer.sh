#!/bin/bash

if [ $# -ne 2 ]; then
	echo "> error usage: $0 <dir> <search_str>"
	exit 1
fi

# Get the arguments
file_name=$1
write_str=$2

echo "> output file: $file_name"
echo "> output string: $write_str"

# Extract the directory
dir=$(dirname $file_name)

echo "> file path : $dir"

if [ ! -d $dir ]; then
	echo "> creating directory"
	mkdir -p $dir
        echo "> created directory: $dir"
fi

if [ -e $file_name ]; then
	echo "> file exists: $file_name"
else
	touch $file_name
	echo "> file created: $file_name"
fi

# Write the string to a file

echo $write_str > $file_name


#!/bin/bash

if [ $# -ne 2 ]; then
	echo "error usage: $0 <dir> <search_str>"
	exit 1
fi

# Get the arguments
dir=$1
search_str=$2

echo "search directory: $dir"
echo "search string: $search_str"

# Check if the directory exists.
if [ -d "$dir" ]; then
	echo "searching in: $dir for: $search_str"    
else
	echo "error invalid directory: $dir"
	exit 1
fi

files_count=$(find $dir -type f | wc -l)

string_count=$(find $dir -type f -exec grep -o $search_str {} + | wc -l)


echo "The number of files are $files_count and the number of matching lines are $string_count"


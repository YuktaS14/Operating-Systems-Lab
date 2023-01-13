#!/bin/bash

# Checking whether no of arguments given are correct or not 
if [ $# == 0 ]
then
	directory=$( pwd )
elif [ $# == 1 ]
then
	directory=$1
	echo "usage $0 <arg1> where <arg1> is the absolute path to the directory in which duplicates should be replaced by softlinks"

	echo "Did u give argument correctly?(type yes/no)"
	read truth_value 

	if [ $truth_value  == "no" ]
	then 
		exit 1 
	fi
	
else
	echo "Wrong number of arguments received"
	echo "usage $0 <arg1> where <arg1> is the absolute path to the directory in which duplicates should be replaced by softlinks"
	exit 1 
fi

#check if its regular file
for file1 in $( find $directory -maxdepth 1 -type f | sort )  
do
	 
	 for file2 in $( find $directory -maxdepth 1 -type f )
	 do
                
                if [ $file1 = $file2 ]
		then
                	continue
                fi

                diff $file1 $file2 > discard.txt
                
                if [ $? -eq 0 ]
                then
                        ln -sf $file1 $file2                    
                fi
        done
done

rm -f discard.txt

#!/bin/bash

#Example usage:
#find ./program3 -type f -name "*.cpp" -o -name "*.h" | ./GryphonBeaver_cs390Program3.sh

while read line 
do
	echo $line
	sed -i 's|\([^A-Za-z0-9_]*\)unsigned int\([^A-Za-z0-9_]\)|\1CS390_UINT32\2|g' $line      	#unsigned int -> CS390_UINT32
	sed -i 's|\([^A-Za-z0-9_]*\)int\([^A-Za-z0-9_]\)|\1CS390_INT32\2|g' $line                 #int -> CS390_INT32
	sed -i 's|\([^A-Za-z0-9_]*\)unsigned char\([^A-Za-z0-9_]\)|\1CS390_UCHAR\2|g' $line		#unsigned char -> CS390_UCHAR
	sed -i 's|\([^A-Za-z0-9_]*\)char\([^A-Za-z0-9_]\)|\1CS390_CHAR\2|g' $line			#char -> CS390_CHAR
	sed -i 's|\([^A-Za-z0-9_]*\)unsigned short\([^A-Za-z0-9_]\)|\1CS390_UINT16\2|g' $line	#unsigned short -> CS390_UINT16
	sed -i 's|\([^A-Za-z0-9_]*\)short\([^A-Za-z0-9_]\)|\1CS390_INT16\2|g' $line			#short -> CS390_INT16
	sed -i 's|\([^A-Za-z0-9_]*\)double\([^A-Za-z0-9_]\)|\1CS390_FLOAT64\2|g' $line		#double -> CS390_FLOAT64
	sed -i 's|\([^A-Za-z0-9_]*\)float\([^A-Za-z0-9_]\)|\1CS390_FLOAT32\2|g' $line			#float -> CS390_FLOAT32
	sed -i 's|\([^A-Za-z0-9_]*\)bool\([^A-Za-z0-9_]\)|\1CS390_BOOL\2|g' $line			#bool -> CS390_BOOL
	
done


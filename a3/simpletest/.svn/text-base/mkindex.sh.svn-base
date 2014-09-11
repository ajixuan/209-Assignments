#!/bin/sh

# create the index for the files in each directory.
# note that indexer must be in the parent directory
# and must have been built already.

for d in d*
do
	cd $d
	rm index filenames
	../../indexer
	cd ..
done

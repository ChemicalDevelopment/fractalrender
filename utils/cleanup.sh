#!/bin/bash

###
#
# Deletes all extra files
#
###

FILE_TYPES_TO_DELETE="png gif mp4 raw bmp"

for TYPE in $FILE_TYPES_TO_DELETE; do
    find . -name "*.$TYPE" -type f -delete
done


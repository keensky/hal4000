#!/bin/bash

echo "Packing binary.."
gzip -9 $1
mv $1.gz h1.gz

echo "Generating executable shelldroper.."

FILE=$1_packed
if test -f "$FILE"; then
    echo "$FILE exist"
  rm $1_packed
fi
   
touch $1_packed
cat unpack_header h1.gz > $1_packed
echo "final size is: "
ls -la | grep $1_packed | awk '{print $5}'
chmod +x $1_packed
echo "cleaning"
rm h1.gz

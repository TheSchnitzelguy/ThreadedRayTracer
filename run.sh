#!/bin/sh
echo "enter horizontal resolution: " 
read hor

echo "enter vertical resolution: " 
read vert

echo "enter sample amount: "
read amount

echo "enter filename: "
read fileName

./raytracer $hor, $vert, $amount, $fileName

convert $fileName.ppm  $fileName.png
rm $fileName.ppm
mv $fileName.png outputs/

echo "I saved the the generated scene in the outputs folder!"

eog outputs/$fileName.png

#test
#./raytracer 800 400 1 output


mkdir 310x90
mkdir 60x70
mkdir 50x90
magick mogrify -path 310x90 -filter Lanczos -resize 340x340 -gravity Center -extent 310x90+0-10 -define png:color-type=6 *.png 
magick mogrify -path 60x70 -filter Lanczos -resize 116x116 -gravity Center -extent 60x70 -define png:color-type=6 *.png
magick mogrify -path 50x90 -filter Lanczos -resize 106x106 -gravity Center -extent 50x90 -define png:color-type=6 *.png
#! /bin/bash
FILES="$(find 'Textures' -name '*.png')"
# echo $FILES
for i in $FILES
do
	FILE=$(basename "$i")
	if ! grep -rq $FILE ../Classes/ ./CCG/Resources/ ../Resources/json; then
		# echo "$FILE present"
	# else
		echo "$i    ABSENT"
		# rm $i
	fi
done

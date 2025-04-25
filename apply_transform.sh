# create a script to run filter.py on all jpg files in a directory
# and save the output in a new directory specified by the user
#
# Usage: ./apply_transform.sh <filter.py> <input_directory> <output_directory>
#
#!/usr/bin/env bash
#
# check if the correct number of arguments is provided

# use virtual environment in the current directory
# if it is not activated, activate it
# NOTE: not working
#if [ -z "$VIRTUAL_ENV" ]; then
#	echo "Activating virtual environment..."
#	source .venv/bin/activate
#fi

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <filter.py> <input_directory> <output_directory>"
    exit 1
fi

# assign arguments to variables
FILTER_SCRIPT=$1
INPUT_DIR=$2
OUTPUT_DIR=$3

# check if input directory exists
if [ ! -d "$INPUT_DIR" ]; then
    echo "Input directory $INPUT_DIR does not exist."
    exit 1
fi

# create output directory if it does not exist
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
fi

# check if filter.py exists
if [ ! -f "$FILTER_SCRIPT" ]; then
    echo "Filter script $FILTER_SCRIPT does not exist."
    exit 1
fi


# loop through all jpg files in the input directory
for file in "$INPUT_DIR"/*.JPG; do
    # check if the file exists
    # echo "Processing $file"
    if [ ! -f "$file" ]; then
	echo "No jpg files found in $INPUT_DIR."
	exit 1
    fi

    # get the base name of the file
    filename=$(basename "$file")

    # run the filter script on the file and save the output in the output directory
    # the python script handles the output file name
    python3 "$FILTER_SCRIPT" "$file" "$OUTPUT_DIR"


done

#loop through all txt files in the input directory and
# create a new file with the same name in the output directory + filter script name wihout .py extension
# for example: image.txt -> image_filter.txt
for file in "$INPUT_DIR"/*.txt; do
	# check if the file exists
	if [ ! -f "$file" ]; then
	echo "No txt files found in $INPUT_DIR."
	exit 1
	fi

	# get the base name of the file
	filename=$(basename "$file")

	# add to filename the filter script name before the extension
	# for example: image.txt -> image_filter.txt
	filename="$(basename "$FILTER_SCRIPT" .py)_${filename%.*}.txt"

	# copy the file to the output directory with the new name
	cp "$file" "$OUTPUT_DIR/$filename"
done

#finally, loop through all jpg and txt files in the input directory and copy them to 
# the output directory with the same name
# that way augmented files will be in the output directory + the original files
for file in "$INPUT_DIR"/*; do
	echo "processing $file"
	# check if the file exists
	if [ ! -f "$file" ]; then
	echo "No files found in $INPUT_DIR."
	exit 1
	fi

	# get the base name of the file
	filename=$(basename "$file")

	# copy the file to the output directory with the new name
	cp "$file" "$OUTPUT_DIR/$filename"
done

# print success message if all files are processed
echo "All jpg files in $INPUT_DIR have been processed and saved in $OUTPUT_DIR."
# exit with success status
exit 0

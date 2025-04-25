# helper to transform an image using a canny filter

import cv2 as cv
import sys

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python canny_filter.py <image_path> <output_folder>")
        sys.exit(1)

    image_path = sys.argv[1]
    # get image name but strip the extension
    image_name = image_path.split("/")[-1].split(".")[0]

    # target folder for the output
    target_folder = sys.argv[2]

    # Load the image
    image = cv.imread(image_path)
    if image is None:
        print(f"Error: Could not load image {image_path}")
        sys.exit(1)

    # Convert to grayscale
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)

    # Apply Canny edge detection
    edges = cv.Canny(gray, 100, 200)

    # Save the result
    output_path = target_folder + "/" + "canny_" + image_name + ".JPG"
    cv.imwrite(output_path, edges)
    print(f"Canny edge detection result saved to {output_path}")

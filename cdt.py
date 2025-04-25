# helper to transform an image pdf image

import cv2 as cv
import sys
import numpy as np
from pytranskit.optrans.utils import signal_to_pdf


def pdf_to_8bit(pdf):
    """
    Convert a PDF image to 8-bit grayscale.
    """
    # Normalize the PDF to the range [0, 255]
    pdf_normalized = cv.normalize(pdf, None, 0, 255, cv.NORM_MINMAX)
    # Convert to uint8
    pdf_8bit = pdf_normalized.astype('uint8')
    return pdf_8bit


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python filter.py <image_path> <output_folder>")
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

    # get probality density function of an image
    pdf = signal_to_pdf(image)
    cdt_pdf = np.cumsum(pdf, axis=0)
    cdt_pdf = np.cumsum(cdt_pdf, axis=1)

    # print(f"max value from matrix: {pdf.max()}")
    # print(f"min value from matrix: {pdf.min()}")

    pdf_8bit = pdf_to_8bit(cdt_pdf)
    # convert pdf using a viridis colormap
    pdf_colored = cv.applyColorMap(pdf_8bit, cv.COLORMAP_VIRIDIS)

    # save colorized pdf to output folder as jpg image
    pdf_colored_path = f"{target_folder}/{image_name}_cdf.jpg"
    cv.imwrite(pdf_colored_path, pdf_colored)

    print(f"PDF saved to {pdf_colored_path}")

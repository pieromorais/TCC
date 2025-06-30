#include<opencv2/opencv.hpp>
#include <fstream>

// function declarations
cv::Mat loadImage(const std::string& filename);
cv::Mat insertNoise(cv::Mat& image, double noiseLevel);

void displayImage(const cv::Mat& image, const std::string& windowName);
void saveImage(const cv::Mat& image, const std::string& filename);
void saveTxtFile(const std::string& ipath, const std::string& ifilename, const std::string& opath, const std::string& oname);

int main(int argc, char** argv) {
    
    if (argc == 4) {
        // insert a single noise level to all images in the directory
        std::string input_dir = argv[1]; // Directory path from command line argument
        std::string out_dir = argv[2]; // Output directory path from command line argument
        std::string noiseLevelStr = argv[3]; // Noise level from command line argument
        double noiseLevel = std::stod(noiseLevelStr); // Convert string to double

        // get all image files in the directory jpg or JPG and keep both extensions
        // meaning that jpg could be capslocked or not

        std::vector<std::string> imageFiles;

        std::vector<std::string> imageFiles_jpg; 
        cv::glob(input_dir + "/*.jpg", imageFiles_jpg, false); // Get all jpg files in the directory
        //
        std::vector<std::string> imageFiles_JPG;
        cv::glob(input_dir + "/*.jpg", imageFiles_JPG, false); // Get all jpg files in the directory
        
        std::vector<std::string> imageFiles_png; 
        cv::glob(input_dir + "/*.png", imageFiles_jpg, false); // Get all jpg files in the directory
        
        std::vector<std::string> imageFiles_PNG; 
        cv::glob(input_dir + "/*.PNG", imageFiles_jpg, false); // Get all jpg files in the directory
        
        // Combine all image files into a single vector
        imageFiles.insert(imageFiles.end(), imageFiles_jpg.begin(), imageFiles_jpg.end());
        imageFiles.insert(imageFiles.end(), imageFiles_JPG.begin(), imageFiles_JPG.end());
        imageFiles.insert(imageFiles.end(), imageFiles_png.begin(), imageFiles_png.end());
        imageFiles.insert(imageFiles.end(), imageFiles_PNG.begin(), imageFiles_PNG.end());

        // cv::glob(input_dir + "/*.png", imageFiles, false);

        if (imageFiles.empty()) {
            std::cerr << "No images found in the directory: " << input_dir << std::endl;
            return -1;
        }

        for (const auto& filename : imageFiles) {
            cv::Mat original_image = loadImage(filename);
            if (original_image.empty()) {
                std::cerr << "Error: Could not load image from " << filename << std::endl;
                continue;
            }
            cv::Mat noisy_image = insertNoise(original_image, noiseLevel);
            //displayImage(original_image, "Original Image");
            // displayImage(noisy_image, "Noisy Image");
            
            // Save the noisy image to the output directory
            std::string n_filename = filename.substr(filename.find_last_of("/\\") + 1); // Get the file name without path
            saveImage(noisy_image, out_dir + "/noisy_" + noiseLevelStr + n_filename);
            
            // get image txt file w/ same name as the image
            std::string txt_filename = n_filename.substr(0, n_filename.find_last_of('.')) + ".txt"; // Change extension to .txt
            saveTxtFile(input_dir, txt_filename, out_dir, "noisy_" + noiseLevelStr + txt_filename); 

            // save the original image to the output directory
            saveImage(original_image, out_dir + "/" + n_filename);
            saveTxtFile(input_dir, txt_filename, out_dir, txt_filename); // Save the original image txt file

        }
    }

    return 0;
}

// functions implementations

cv::Mat loadImage(const std::string& filename) {
    // Load an image from the specified file path
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error: Could not load image from " << filename << std::endl;
        return cv::Mat();
    }
    return image;
}

cv::Mat insertNoise(cv::Mat& image, double noiseLevel) {
    // insert noise to the image
    // Create a noise matrix with the same size as the image
    cv::Mat noise(image.size(), CV_8UC3);
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(noiseLevel));
    // Add the noise to the original image
    cv::Mat noisy_image;
    cv::add(image, noise, noisy_image);

    return noisy_image; // Return the noisy image

}

void displayImage(const cv::Mat& image, const std::string& windowName) {
    // Display the image in a window
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::resizeWindow(windowName, 640, 480); // Resize the window to a fixed size
    cv::imshow(windowName, image);
    cv::waitKey(0); // Wait for a key press
}

void saveImage(const cv::Mat& image, const std::string& filename) {
    // Save the image to the specified file path
    // filename should include the extension (e.g., .jpg, .png) and the directory
    if (!cv::imwrite(filename, image)) {
        std::cerr << "Error: Could not save image to " << filename << std::endl;
    }
}

void saveTxtFile(const std::string& ipath, const std::string& ifilename, const std::string& opath, const std::string& oname) {
    // copy a file from a directory to another directory
    // input is the path + txt name
    // output is the path + txt name that should be created
    //
    std::ifstream inputFile(ipath + "/" + ifilename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file " << ipath + "/" + ifilename << std::endl;
        return;
    }
    std::ofstream outputFile(opath + "/" + oname);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file " << opath + "/" + oname << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        outputFile << line << std::endl; // Write each line to the output file
    }
    inputFile.close();
    outputFile.close();
    // std::cout << "File copied successfully from " << ipath + "/" + ifilename << " to " << opath + "/" + oname << std::endl;

}

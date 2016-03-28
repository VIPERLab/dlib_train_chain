/*

This work is hereby released into the Public Domain.
To view a copy of the public domain dedication, visit
http://creativecommons.org/licenses/publicdomain/ or send a
letter to
    Creative Commons
    171 Second Street
    Suite 300,
    San Francisco, California, 94105, USA.

Public domain dedications are not recognized by some countries.  So
if you live in an area where the above dedication isn't valid then
you can consider the example programs to be licensed under the Boost
Software License.

Note that this program executes fastest when compiled with at least SSE2
instructions enabled.  So if you are using a PC with an Intel or AMD chip
then you should enable at least SSE2 instructions.  If you are using cmake
to compile this program you can enable them by using one of the following
commands when you create the build project:
    cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
    cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
    cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
This will set the appropriate compiler options for GCC, clang, Visual
Studio, or the Intel compiler.  If you are using another compiler then you
need to consult your compiler's manual to determine how to enable these
instructions.  Note that AVX is the fastest but requires a CPU from at least
2011.  SSE4 is the next fastest and is supported by most current machines.

Extended by Florian Lier [flier AT techfak.uni-bielefeld DOT de]

*/


#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/svm_threaded.h>
#include <dlib/data_io.h>

using namespace dlib;
using namespace std;

int main(int argc, char** argv)
{
    try
    {

        if (argc != 2)
        {
            cout << ">> Provide the path to the trained svm" << endl;
            cout << ">> Example:   ./dtc-single-detection path/to/svm" << endl;
            cout << endl;
            return 0;
        }

        const std::string svm_directory = argv[1];

        cv::VideoCapture cap(0);
        image_window win;

        typedef scan_fhog_pyramid<pyramid_down<6> > image_scanner_type;
        object_detector<image_scanner_type> detector;
        deserialize(svm_directory) >> detector;
        cout << ">> Using <-- " << svm_directory << endl;

        // Grab and process frames until the main window is closed by the user.
        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;
            cap >> temp;
            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            cv_image<bgr_pixel> cimg(temp);

            // Detect faces
            std::vector<rectangle> things = detector(cimg);
            // cout << things.size() << endl;
            win.set_title(":: DTC Single Detection (Quit: close this window) :: ");
            win.clear_overlay();
            win.set_image(cimg);
            win.add_overlay(things, dlib::rgb_pixel(255, 0, 0));
        }
    }
    catch(serialization_error& e)
    {
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}
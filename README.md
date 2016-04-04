DLIB Based HOG Training Toolchain
==================================

A tool set to automate DLIB fHOG object training and detection.

# Requirements

    sudo apt-get install python-opencv python-numpy

Install dlib as explained here: http://dlib.net/compile.html
Please compile **using AVX instruction set** (cmake .. -DUSE_AVX_INSTRUCTIONS=ON)

    
# Installation
    
    mkdir build
    cd build
    cmake ..
    
    
# Usage

    python dtc-collect-data.py --test on --rectangle 250

    python dtc-collect-data.py --path /tmp --rectangle 250 --imagecount 10 --name thing_1
    
    dtc-trainer /tmp/thing_1/ thing_1
    
    dtc-single-detection /tmp/thing_1/thing_1.svm
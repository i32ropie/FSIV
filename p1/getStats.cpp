/*!
   Esto es un esqueleto de programa para usar en las prácticas
   de Visión Artificial.

   Se supone que se utilizará OpenCV.

   Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

 */

#include <ctype.h>
#include <vector>
#include <stdlib.h>
// #include <unistd.h>
#include <iostream>
#include <exception>
#include <algorithm>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include "getStats.hpp"
#include "stats.hpp"


int main (int argc, char* const* argv){
    int retCode=EXIT_SUCCESS;
    try {
        CLIParams params;
        int argObligados = parseCLI(argc, argv, params);
        if(argObligados < argc){
            params.image_name = std::string(argv[argObligados]);
            params.image_flag = true;
            argObligados++;
            if(argObligados < argc){
                params.mask_name = std::string(argv[argObligados]);
                params.mask_flag = true;
                argObligados++;
                if(argObligados < argc){
                    std::cerr << "\e[1;31m[Error]\e[m-The following aregument is not an option <" << argv[argObligados] << ">" << std::endl;
                    show_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(!params.image_flag){
            std::cerr << "\e[1;31m[Error]\e[m-Image name required" << std::endl;
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
        cv::Mat img;
        cv::Mat mask;
        std::vector<cv::Mat> img_channels, mask_channels;
        cv::Mat img_roi;
        fsiv::Stats stats;
        img = cv::imread(params.image_name, -1);
        if(img.empty()){
            std::cerr << "\e[1;31m[Error]\e[m-Image couldn't be opened." << std::endl;
            exit(EXIT_FAILURE);
        }
        if(params.mask_flag){
            mask = cv::imread(params.mask_name, -1);
            if(mask.empty()){
                std::cerr << "\e[1;31m[Error]\e[m-Mask couldn't be opened." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if(params.w_flag){
            img_roi = img(cv::Rect(params.w_values[0], params.w_values[1], params.w_values[2], params.w_values[3]));
            if(params.mask_flag){
                mask = mask(cv::Rect(params.w_values[0], params.w_values[1], params.w_values[2], params.w_values[3]));
            }
        }
        else{
            img_roi = img.clone();
        }
        cv::split(img_roi, img_channels);
        cv::split(mask, mask_channels);
        if(img_channels.size() == 3)
            std::swap(img_channels[0], img_channels[2]);
        for( uint i = 0 ; i < img_channels.size() ; ++i ){
            std::cout << "\e[1mChannel " << i << ":" << std::endl << std::endl;
            stats.calculate_stats(img_channels[i], params.mask_flag, mask_channels[i]);
            std::cout << stats << std::endl;
        }
    }
    catch (std::exception& e){
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }
    return retCode;
}

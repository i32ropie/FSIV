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
#include "getStats.hpp"
#include "stats.hpp"

bool drag = false;
cv::Point p1, p2;
cv::Mat img;

void mouse_handler(int event, int x, int y, int flags, void *param){
    if(event == CV_EVENT_LBUTTONDOWN && !drag){
        p1 = cv::Point(x, y);
        drag = true;
    }
    if(event == CV_EVENT_MOUSEMOVE && drag){
        p2 = cv::Point(x, y);
        cv::Mat aux = img.clone();
        cv::rectangle(aux, p1, p2, CV_RGB(143, 255, 0), 1, 4, 0);
        cv::imshow("Image", aux);
    }
    if(event == CV_EVENT_LBUTTONUP && drag){
        p2 = cv::Point(x, y);
        drag = false;
    }
}

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
        if(params.i_flag && params.w_flag){
            std::cerr << "\e[1;31m[Error]\e[m-You can't use -w and -i options at the same time." << std::endl;
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
        if(!params.image_flag){
            std::cerr << "\e[1;31m[Error]\e[m-Image name required" << std::endl;
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
        cv::Mat mask;
        std::vector<cv::Mat> img_channels, mask_channels;
        cv::Mat img_roi;
        fsiv::Stats stats;
        cv::Rect rect;
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
        if(params.i_flag){
            if(params.mask_flag){
                std::vector<cv::Mat> aux1, aux2, aux3;
                cv::Mat aux4;
                cv::split(img, aux1);
                cv::split(mask, aux2);
                for( uint k = 0 ; k < aux1.size() ; ++k ){
                    for( int i = 0 ; i < aux1[k].rows ; ++i ){
                        for( int j = 0 ; j < aux1[k].cols ; ++j ){
                            if(aux2[k].at<uchar>(i,j) == 255)
                                aux2[k].at<uchar>(i,j) = aux1[k].at<uchar>(i,j);
                        }
                    }
                    aux3.push_back(aux2[k]);
                }
                cv::merge(aux3, aux4);
                img = aux4.clone();
            }
            cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
            cv::imshow("Image", img);
            while(true){
                cv::setMouseCallback("Image", mouse_handler, NULL);
                int key = cv::waitKey(0)%256;
                if(key == 10){
                    if(p1.x == p2.x && p1.y == p2.y)
                        std::cout << "Select an area with your mouse, please." << std::endl;
                    else
                        break;
                }
                if(key == 27)
                    return EXIT_SUCCESS;
            }
            if(p1.x < p2.x)
                rect = p1.y < p2.y ? cv::Rect(p1.x, p1.y, p2.x-p1.x, p2.y-p1.y) : cv::Rect(p1.x, p2.y, p2.x-p1.x, p1.y-p2.y);
            else
                rect = p1.y < p2.y ? cv::Rect(p2.x, p1.y, p1.x-p2.x, p2.y-p1.y) : cv::Rect(p2.x, p2.y, p1.x-p2.x, p1.y-p2.y);
            img_roi = img(rect);
            if(params.mask_flag)
                mask = mask(rect);
        }
        else if(params.w_flag){
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
        if(img_channels.size() == 3) // BGR to RGB
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

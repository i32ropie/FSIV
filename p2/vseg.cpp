/*
Compilar con la línea:

g++ -Wall -g -o vseg vseg.cc `pkg-config opencv --cflags --libs`

*/


#include <iostream>
#include <cstdlib>
#include <exception>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "vseg.hpp"



int main(int argc, char *const argv[]) {
    int retCode = EXIT_SUCCESS;
    try{
        CLIParams params;
        int argObligados = parseCLI(argc, argv, params);
        if(argObligados < argc){
            params.i_name = std::string(argv[argObligados]);
            params.i_flag = true;
            argObligados++;
            if(argObligados < argc){
                params.o_name = std::string(argv[argObligados]);
                params.o_flag = true;
                argObligados++;
                if(argObligados < argc){
                    std::cerr << "\e[1;31m[Error]\e[m-The following aregument is not an option <" << argv[argObligados] << ">" << std::endl;
                    show_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(!(params.i_flag && params.o_flag)){
            std::cerr << "\e[1;31m[Error]\e[m-Both Input and Output files are required." << std::endl;
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
        std::cout << "i_name = " << params.i_name << "\no_name = " << params.o_name << "\nt_value = " << params.t_value << std::endl;
        cv::VideoCapture input(params.i_name);
        if(!input.isOpened()){
            std::cerr << "\e[1;31m[Error]\e[m-Input file couldn't be created." << std::endl;
            exit(EXIT_FAILURE);
        }
        cv::Mat output, final_output, capture, mask, frame, cached_frame, diff;
        cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);
        cv::createTrackbar("Threshold detection", "Video", &params.t_value, 255);
        int frames = 0;
        cv::VideoWriter out;
        bool aux = true;
        int ex = static_cast<int>(input.get(CV_CAP_PROP_FOURCC));
        cv::Size size = cv::Size(int(input.get(CV_CAP_PROP_FRAME_WIDTH)), int(input.get(CV_CAP_PROP_FRAME_HEIGHT)));

        out.open(params.o_name, ex, 30, size, true);

        if(!out.isOpened()){
            std::cerr << "\e[1;31m[Error]\e[m-Output file couldn't be created." << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << params.i_name << "    " << params.o_name << std::endl;
        while(true){
            input >> frame;
            if(frame.empty())
                break;
            frames++;
            cv::cvtColor(frame, output, CV_BGR2GRAY);
            mask = output.clone();
            if(!aux){
                cv::absdiff(cached_frame, output, diff);
                for(int i = 0 ; i < diff.rows ; ++i){
                    for(int j = 0 ; j < diff.cols ; ++j){
                        if(diff.at<uchar>(i,j) > params.t_value)
                            mask.at<uchar>(i,j) = 255;
                        else
                            mask.at<uchar>(i,j) = 0;
                    }
                }
                final_output = frame.clone();
                capture = frame.clone();
                for(int i = 0 ; i < output.rows ; ++i){
                    for(int j = 0 ; j < output.cols ; ++j){
                        if(mask.at<uchar>(i,j) != 255){
                            for(int k = 0 ; k < final_output.channels() ; ++k){
                                final_output.at<cv::Vec3b>(i,j)[k] = 0;
                                capture.at<cv::Vec3b>(i,j)[k] = 255;
                            }
                        }
                    }
                }
                if((cv::waitKey(10)&0xFF) == 32)
                    cv::imwrite("sal_"+std::to_string(frames)+".png", capture);
                cv::imshow("Video", final_output);
                out << final_output;
                cv::namedWindow("Mask", CV_WINDOW_AUTOSIZE);
                cv::imshow("Mask", mask);
            }
            cached_frame = output.clone();
            aux = false;
        }
    }
    catch(std::exception &e){
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }


    return retCode;
}
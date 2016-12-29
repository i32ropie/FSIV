/*
Compilar con la línea:

g++ -Wall -g -o getContour getContour.cpp `pkg-config opencv --cflags --libs`

*/

#include <iostream>
#include <cstdlib>
#include <exception>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "getContour.hpp"

int main(int argc, char *const argv[]) {
    int retCode = EXIT_SUCCESS;
    try{
        CLIParams params;
        int argObligados = parseCLI(argc, argv, params);
        for(int index = optind ; index < argc ; ++index){
            std::cerr << "\e[1;31m[Error]\e[m-El siguiente parámentro no es una opción <" << argv[index] << ">" << std::endl;
        }
        if(!(params.i_flag && params.c_flag)){
            std::cerr << "\e[1;31m[Error]\e[m-Las opciones -c y -i son obligatorias." << std::endl;
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
        cv::Mat img = cv::imread(params.i_value, 0);
        if(!img.data){
            std::cerr << "\e[1;31m[Error]\e[m-La imagen solicitada no existe." << std::endl;
            exit(EXIT_FAILURE);
        }
        cv::Mat contorno_img = img.clone();
        cv::Mat contorno_fondo = img.clone();
        cv::threshold(contorno_img, contorno_fondo, 0, 255, CV_THRESH_BINARY);
        std::vector<std::vector<cv::Point>> matriz_contornos;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(contorno_fondo, matriz_contornos, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        guardar_contorno(matriz_contornos.back(), params.c_value);
        cv::Mat draw = cv::Mat::zeros(contorno_fondo.size(), CV_8UC3);
        cv::Scalar color(255, 255, 255);
        for(int i = 0 ; i >= 0 ; i=hierarchy[i][0]){
            cv::drawContours(draw, matriz_contornos, i, color, 1, 8, hierarchy);
        }
        cv::namedWindow("Imagen binaria");
        cv::namedWindow("Contorno");
        cv::moveWindow("Imagen binaria", 50, 250);
        cv::moveWindow("Contorno", 700, 250);
        cv::imshow("Imagen binaria", img);
        cv::imshow("Contorno", draw);
        cv::waitKey(0);
    }
    catch(std::exception &e){
        std::cerr << "Capturada excepcion: " << e.what() << std::endl;
        retCode = EXIT_FAILURE;
    }


    return retCode;
}
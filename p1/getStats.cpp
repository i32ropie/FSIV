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
                }

        }
        catch (std::exception& e){
                std::cerr << "Capturada excepcion: " << e.what() << std::endl;
                retCode = EXIT_FAILURE;
        }
        return retCode;
}

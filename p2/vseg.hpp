//
// Created by i32ropie on 1/11/16.
//

#ifndef P2_VSEQ_HPP
#define P2_VSEQ_HPP

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

struct CLIParams{
    CLIParams():t_flag(false), i_flag(false), o_flag(false), i_name(""), o_name(""), t_value(20){};
    bool t_flag;
    bool i_flag;
    bool o_flag;
    std::string i_name;
    std::string o_name;
    int t_value;
};

void show_usage(const char * progname) throw(){
    std::cout << std::endl << "The program syntaxis is:" << std::endl << std::endl;
    std::cout << "\t\e[1m" << progname << " [-h] [-t <int> ] input_video output_video" << std::endl << std::endl;
    std::cout << "\e[1minput_video\e[m   : Name of the input video file.          (\e[4;1mRequired\e[m)" << std::endl;
    std::cout << "\e[1moutput_video\e[m  : Name of the output video file.         (\e[4;1mRequired\e[m)" << std::endl;
    std::cout << "\e[1m-t <int>\e[m      : Threshold value.                       (\e[4;1mOptional\e[m)" << std::endl;
    std::cout << "\t\tcorner and w,h as width and height" << std::endl;
    std::cout << "\e[1m-h\e[m            : Displays the help." << std::endl;
}

int parseCLI (int argc, char *const * argv, CLIParams& params) throw (){
    // Esta es una forma habitual de recoger argumentos con getopt
    // se usa una iteracion y cada elemento se pasa por un switch-case
    int option;
    while ((option = getopt (argc, argv, "ht:")) != -1){
        switch (option){
            case 'h':
                show_usage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 't':
                params.t_flag = true;
                params.t_value = atoi(optarg);
                break;
            case '?': // en caso de error getopt devuelve el caracter ?
                if(optopt == 't')
                    std::cerr << "Error: -t option requires an argument."<< std::endl;
                if (isprint (optopt))
                    std::cerr << "Error: Opción desconocida \'" << optopt
                              << "\'" << std::endl;
                else
                    std::cerr << "Error: Caracter de opcion desconocido \'x" << std::hex << optopt
                              << "\'" << std::endl;
                show_usage(argv[0]);
                exit(EXIT_FAILURE);

                // en cualquier otro caso lo consideramos error grave y salimos
            default:
                std::cerr << "Error: línea de comandos errónea." << std::endl;
                show_usage(argv[0]);
                exit(EXIT_FAILURE);
        } // case

    }// while
    return optind;
}

#endif //P2_VSEQ_HPP

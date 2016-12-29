#ifndef P2_GETCONTOUR_HPP
#define P2_GETCONTOUR_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>


struct CLIParams{
    CLIParams():c_flag(false), i_flag(false), i_value(""), c_value(""){};
    bool c_flag;
    bool i_flag;
    std::string i_value;
    std::string c_value;
};

void show_usage(const char * progname) throw(){
    std::cout << std::endl << "La sintaxis del programa es:" << std::endl << std::endl;
    std::cout << "\t\e[1m" << progname << " -i imagen_binaria -c salida_de_contorno" << std::endl << std::endl;
    std::cout << "\e[1m-i <fichero>\e[m  : Nombre de la imagen binaria.           (\e[4;1mRequired\e[m)" << std::endl;
    std::cout << "\e[1m-c <fichero>\e[m  : Nombre de la salida del contorno.      (\e[4;1mRequired\e[m)" << std::endl;
    std::cout << "\e[1m-h\e[m            : Esta ayuda." << std::endl;
}

int parseCLI (int argc, char *const * argv, CLIParams& params) throw (){
    // Esta es una forma habitual de recoger argumentos con getopt
    // se usa una iteracion y cada elemento se pasa por un switch-case
    int option;
    while ((option = getopt (argc, argv, "hi:c:")) != -1){
        switch (option){
            case 'h':
                show_usage(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'c':
                params.c_flag = true;
                params.c_value = std::string(optarg);
                break;
            case 'i':
                params.i_flag = true;
                params.i_value = std::string(optarg);
                break;
            case '?': // en caso de error getopt devuelve el caracter ?
                if(optopt == 'i' || optopt == 'c')
                    std::cerr << "Error: Las opciones -i y -c requieren un argumento."<< std::endl;
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

void guardar_contorno(const std::vector<cv::Point> &p, const std::string &f_name){
    std::ofstream f(f_name.c_str());
    if(f.is_open()){
        for(auto it = p.cbegin() ; it != p.cend() ; ++it ){
            f << it->x << " " << 256 - it->y << std::endl;
        }
        f.close();
    }
    else{
        std::cerr << "Error: No se pudo abrir " << f_name << std::endl;
        exit(EXIT_FAILURE);
    }
}

#endif

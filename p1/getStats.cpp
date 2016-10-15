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
#include <unistd.h>
#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

/*!\brief Define los parámetro opcionales de tu programa.

   Redefine esta clase en función de tus necesitados
 */
struct CLIParams{
    CLIParams(): w_flag(false), h_flag(false), image_flag(false), mask_flag(false), w_value(""), image_name(""), mask_name(""){};
    bool w_flag, h_flag, image_flag, mask_flag;
    std::string w_value, image_name, mask_name;
    std::vector<int> w_values;
};


/*!\brief Muestra la ayuda del programa.
   Redefinir en función de los parámetros utilizados realmente.
   \arg[in] programe es el nombre del programa en la cli.
 */
 static void show_usage (const char * progname) throw ()
 {
    std::cout << "\nUsage: \e[1m" << progname << "\e[m [-h] [-w \"x,y,w,h\"] \e[1mimage\e[m [mask]" << std::endl;
    std::cout << std::endl;
    std::cout << "\e[1m-h\e[m\tDisplays this help menu." << std::endl;
    std::cout << "\e[1m-w\e[m\tAllows you to specify an area of interest with \e[1mx\e[m and \e[1my\e[m are the top-left coords of the area and \e[1mw\e[m and \e[1mh\e[m are the width and the height of that area." << std::endl;
    std::cout << "\e[1mimage\e[m\tName of the image to be studied.\e[1;4mThis param is required.\e[m" << std::endl;
    std::cout << "\e[1mmask\e[m\tName of the image used as a mask for the original image." << std::endl;
}

/*!\brief Parsea la linea de comandos.
   \arg[in] argc es el número de argumentos en la línea.
   \arg[in] argv son los argumentos de la línea.
   \arg[out] params son los parámetros opcionales parseados.
   \return El índice del primer argumento no opcional de la línea.
   \warning Esta función no retorna si hay algún error en la cli.
 */
static int parseCLI (int argc, char* const* argv, CLIParams& params) throw ()
{
    // Esta es una forma habitual de recoger argumentos con getopt
    // se usa una iteracion y cada elemento se pasa por un switch-case
    int option;
    while ((option = getopt (argc, argv, "hw:")) != -1)
    {
        switch (option)
        {

        case 'h':
            show_usage(argv[0]);
            exit (EXIT_SUCCESS);
            break;

        case 'w':
            params.w_flag = true;
            params.w_value = optarg;
            do {
                params.w_values.push_back(atoi(params.w_value.substr(0,params.w_value.find(",")).c_str()));
                params.w_value.erase(0, params.w_value.find(",")+1);
            } while(params.w_value.find(",") < 100);
            if(params.w_values.size() != 4){
                std::cerr << "\e[1;31m[Error]\e[m - The option \e[1m-w\e[m requieres 4 params." << std::endl;
                show_usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            break;

        case '?': // en caso de error getopt devuelve el caracter ?

            if (isprint (optopt))
                std::cerr << "Error: Opción desconocida \'" << optopt
                          << "\'" << std::endl;
            else
                std::cerr << "Error: Caracter de opcion desconocido \'x" << std::hex << optopt
                          << "\'" << std::endl;
            show_usage(argv[0]);
            exit (EXIT_FAILURE);

        // en cualquier otro caso lo consideramos error grave y salimos
        default:
            std::cerr << "Error: línea de comandos errónea." << std::endl;
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        } // case

    }// while
    return optind;
}



int
main (int argc, char* const* argv)
{
        int retCode=EXIT_SUCCESS;

        try {
                CLIParams params;

                int argObligados = parseCLI(argc, argv, params);

                std::cout << "Los parámetros opcioneales son:" << std::endl;
                // std::cout << "-v\t" << ((params.verbose) ? "True" : "False") << std::endl;
                // std::cout << "-i\t" << params.entero << std::endl;
                // std::cout << "-f\t" << params.flotante << std::endl;
                // std::cout << "-c\t" << '\"' << params.cadena << '\"' << std::endl;

                std::cout << "Hay " << argc-argObligados << " parámetros obligados que son: " << std::endl;

                for (int i = argObligados; i<argc; ++i)
                        std::cout << '\"' << argv[i] << '\"' << std::endl;


                /*Ahora toca que tu rellenes con lo que hay que hacer ...*/


        }
        catch (std::exception& e)
        {
                std::cerr << "Capturada excepcion: " << e.what() << std::endl;
                retCode = EXIT_FAILURE;
        }
        return retCode;
}

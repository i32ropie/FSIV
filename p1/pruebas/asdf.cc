/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o asdf asdf.cc `pkg-config opencv --cflags --libs`

*/

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>
#include <vector>
#include <math.h>
//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

//Prototypes
struct estadisticos{
  double canal;
  double min;
  double max;
  double media;
  double varianza;
  double suma;
  double suma_cuadrados;
  double area_positiva;
  double area_negativa;
  double num_ceros;
  double coef_asimetria;
  double ancho;
  double alto;
};


/* Var globales */
cv::Mat imagen, mascara;


/*!\brief Define los parámetro opcionales de tu programa.

  Redefine esta clase en función de tus necesitados
*/
struct CLIParams
{
  CLIParams ()
    :
      iflag(false),
      wflag(false),
      entero(0), //Valores por defecto de los argumentos opcionales.
      flotante(0.0),
      cadena(""),
      verbose(false),
      roi(false),
      maskflag(false),
      imageflag(false),
      maskName(""),
      imageName("")

    {}
  bool iflag;
  bool wflag;
  int entero;
  float flotante;
  std::string cadena;
  bool verbose;
  bool roi;
  std::vector<int> wvalues;
  bool maskflag;
  bool imageflag;
  std::string maskName;
  std::string imageName;

};

void flags(int argc,char* const* argv, CLIParams &params);
int openImage(char* cadena);
int openImageM(char* cadena, char* mascara);
cv::Mat applyMask(cv::Mat img, cv::Mat mask);
void calcularEstadisticos(const cv::Mat& capa, const cv::Mat& mascara, const bool mflag, const bool iflag, struct estadisticos& e);


/*
  Funcion que muestras los estadisticos.
  e: estructura de la que muestra los estadisticos.
*/
void mostrarEstadisticos(struct estadisticos e) {
  std::cout << "\033[0;3;1mEstadísticos: " << "\033[0m"<< std::endl << "\tMinimo: " << e.min << std::endl << "\tMaximo: " << e.max << std::endl << "\tMedia: " << e.media << std::endl << "\tVarianza: " << e.varianza << std::endl << "\tSuma: " << e.suma << std::endl << "\tSuma²: " << e.suma_cuadrados << std::endl << "\tArea Positiva: " << e.area_positiva << std::endl << "\tArea Negativa: " << e.area_negativa << std::endl << "\tNumero de ceros: " << e.num_ceros << std::endl << "\tCoeficiente de asimetria: " << e.coef_asimetria << std::endl;
}


/*!\brief Muestra la ayuda del programa.
  Redefinir en función de los parámetros utilizados realmente.
  \arg[in] programe es el nombre del programa en la cli.
*/
static void
mostrarUso (const char * progname) throw ()
{
  std::cout << "Este programa sirve para ...." << std::endl;
  std::cout << "Uso: getStats <image> [-h] [-w \"x,y,w,h\"] [0mask]" << std::endl;
  std::cout << "Donde: " << std::endl;
  std::cout << "-h\tMuestra  esta la ayuda." << std::endl;
  std::cout << "-w\tIndicar un area de interes ROI mediante x,y de la esquina superior izquierda, ancho y alto." << std::endl;
}

/*!\brief Parsea la linea de comandos.
  \arg[in] argc es el número de argumentos en la línea.
  \arg[in] argv son los argumentos de la línea.
  \arg[out] params son los parámetros opcionales parseados.
  \return El índice del primer argumento no opcional de la línea.
  \warning Esta función no retorna si hay algún error en la cli.
*/
static int
parseCLI (int argc, char* const* argv, CLIParams& params) throw ()
{
  // Esta es una forma habitual de recoger argumentos con getopt
  // se usa una iteracion y cada elemento se pasa por un switch-case
  int opcion;
  while ((opcion = getopt (argc, argv, "hviw:")) != -1)
  {
    switch (opcion)
    {

      case 'h':
	      mostrarUso(argv[0]);
	      exit (EXIT_SUCCESS);
	    break;

      case 'v':
	      params.verbose=true;
	    break;

      case 'i':
        if(!params.wflag){
          params.iflag=true;
        }
        else{
          std::cout << "\033[31;1mNo se puede usar -i y -w a la vez. \033[0m" << std::endl;
          //Mostramos el uso para que sepa cómo usarlo.
		    	mostrarUso(argv[0]);
		    	exit(EXIT_FAILURE);
        }
      break;

      case 'w':
        if(!params.iflag){
        params.wflag = true;
        params.roi=true;
        params.cadena = optarg;
        std::cout << params.cadena;
        //Vamos copiando los 4 números que hemos metido como argumento de -w en un vector
        for(size_t i=0; params.cadena.find(",")<100; ++i)
		    		{
		    			params.wvalues.push_back(atoi(params.cadena.substr(0,params.cadena.find(",")).c_str()));
		    			params.cadena.erase(0, params.cadena.find(",")+1);
		    		}
            //Insertamos en el vector
		    		params.wvalues.push_back(atoi(params.cadena.c_str()));
        //Si son menos de 4 numeros es que esta mal introducido, por lo que hay que comunicarlo.
        if(params.wvalues.size() != 4)
		    		{
		    			std::cout << "\033[31;1mLa opción -w necesita 4 parámetros.\033[0m" << std::endl;
              //Mostramos el uso para que sepa cómo usarlo.
		    			mostrarUso(argv[0]);
		    			exit(EXIT_FAILURE);
		    		}
        }
      else{
              std::cout << "\033[31;1mNo se puede usar -i y -w a la vez. \033[0m" << std::endl;
              //Mostramos el uso para que sepa cómo usarlo.
		    			mostrarUso(argv[0]);
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
	mostrarUso(argv[0]);
	exit (EXIT_FAILURE);

	// en cualquier otro caso lo consideramos error grave y salimos
      default:
	std::cerr << "Error: línea de comandos errónea." << std::endl;
	mostrarUso(argv[0]);
	exit(EXIT_FAILURE);
    }  // case

  }// while
  return optind;
}



int main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;

  try {
    CLIParams params;
    cv::Mat mascara;
    cv::Mat imagen_roi;
    cv::Rect rect;
    std::vector<cv::Mat> canales_img, canales_mascara;
    int argObligados = parseCLI(argc, argv, params);
    struct estadisticos e;

    std::cout << "Los parámetros opcionales son:" << std::endl;
    //std::cout << "-v\t" << ((params.verbose)?"True":"False") << std::endl;
    //std::cout << "-i\t" << params.entero << std::endl;
    //std::cout << "-f\t" << params.flotante << std::endl;
    //std::cout << "-c\t" << '\"' << params.cadena << '\"' << std::endl;
    std::cout << "-w\t" << ((params.roi)?"True":"False") << std::endl;


    if(argc-argObligados == 0){
      std::cout << "\033[31;1mNo hay parámetros obligados." << std::endl << "Debes insertar mínimo un parámetro\033[0m" << std::endl;
    }
    else{
      std::cout << "\033[33;1mHay " << argc-argObligados << " parámetros obligados que son: \033[0m" << std::endl;
    }
    for (int i = argObligados; i<argc; ++i)
      std::cout << '\"' << argv[i] << '\"' << std::endl;



    /*Ahora toca que tu rellenes con lo que hay que hacer ...*/
    flags(argc,argv, params);
    imagen = cv::imread(params.imageName, -1);
    if(imagen.empty()){
      std::cout << "\033[31;1mFallo al abrir la imagen!\033[0m" << std::endl;
      exit(EXIT_FAILURE);
    }
    if(params.maskflag){
      mascara = cv::imread(params.maskName, -1);
      if(mascara.empty()){
      std::cout << "\033[31;1mFallo al abrir la mascara!\033[0m" << std::endl;
      exit(EXIT_FAILURE);
      }
    }

if (params.wflag)
		{
			imagen_roi = imagen(cv::Rect(params.wvalues[0],params.wvalues[1],params.wvalues[2],params.wvalues[3]));
			if(params.maskflag)
				mascara = mascara(cv::Rect(params.wvalues[0],params.wvalues[1],params.wvalues[2],params.wvalues[3]));
		}
		else
			imagen_roi = imagen.clone();


		split(imagen_roi, canales_img);
		split(mascara, canales_mascara);
		std::cout << "\n\033[34;1mEstadísticos de la imagen:" << std::endl;
    std::cout << "Canales: " << canales_img.size();
    std::cout << std::endl << "Ancho: " << canales_img[0].cols << std::endl;
    std::cout << "Alto: " << canales_img[0].rows << "\033[0m" << std::endl;

		for(unsigned int i=0; i<canales_img.size();++i)
		{
			std::cout << std::endl << "\t\033[32;1;4mCanal " << i << ":\033[0m" << std::endl << std::endl;
			calcularEstadisticos(canales_img[i], canales_mascara[i],  params.maskflag, params.iflag, e);
			mostrarEstadisticos(e);
		}

    /*FIN*/


  }
  catch (std::exception& e)
  {
    std::cerr << "\033[31;1mCapturada excepcion: " << e.what() << "\033[0m" << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}


/*
  Función que aplica la mascara a la imagen.
  Mat img: Imagen
  Mat mask: Máscara
*/
cv::Mat applyMask(cv::Mat img, cv::Mat mask){
  for(int i = 0; i < mask.rows; i++) {
    for(int j = 0; j < mask.cols; j++) {
      int px = mask.at<uchar>(i, j);
      if(px == 255){
        mask.at<uchar>(i, j) = img.at<uchar>(i, j);
      }
    }
  }
  return mask;
}


/*
  Funcion que da valor a variables dependiendo de los argumentos.
  argc: Numero de argumentos.
  argv: Los argumentos en cadena.
  params: todos los flags y variables necesarias.
*/
void flags(int argc,char* const* argv, CLIParams &params)
{
	if(params.wflag and argc > 3 and argc < 6)
	{
		for(int i=1;i<argc;++i)
		{
			if(argv[i][0] == '-' and argv[i][1] == 'w')
				i++;
			else
			{
				if(!params.imageflag)
				{
					params.imageName = argv[i];
					params.imageflag = true;
				}
				else
				{
					params.maskName = argv[i];
					params.maskflag = true;
				}
			}
		}
	}
	else
	{
		if(params.iflag and argc > 2 and argc < 5)
		{
			for(int i=1; i<argc; ++i)
			{
				if(argv[i][0] != '-')
				{
					if(!params.imageflag)
					{
						params.imageName = argv[i];
						params.imageflag = true;
					}
					else
					{
						params.maskName = argv[i];
						params.maskflag = true;
					}
				}
			}
		}

		else
		{
			if(!params.wflag and !params.iflag and argc > 1 and argc < 4)
			{
				for(int i=1; i<argc; ++i)
				{
					params.imageName = argv[1];
          std::cout << "\033[33;1mImagen capturada\033[0m" << std::endl;
					params.imageflag = true;
					if(argc == 3)
					{
						params.maskName = argv[2];
						params.maskflag = true;
					}
				}
			}
			else
			{
				std::cout << "\033[31;1mError en los argumentos. Verbose:" << params.verbose << std::endl;
				std::cout << "Se debe especificar una imagen obligatoriamente.\033[0m" << std::endl;
				mostrarUso(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
	}
}


/*
  Funcion que calcula los estadisticos dependiendo de la máscara.
  capa: Capa de la cual se sacarán los estadisticos
  amscara: Si hay máscara, se enmascarará la imagen y se sacarán los estadisticos.

*/
void calcularEstadisticos(const cv::Mat& capa, const cv::Mat& mascara, const bool mflag, const bool iflag, struct estadisticos& e)
{
  e.min = 255;
  e.max = 0  ;
  e.media = 0;
  e.varianza = 0;
  e.suma = 0;
  e.suma_cuadrados = 0;
  e.area_positiva = 0;
  e.area_negativa = 0;
  e.num_ceros = 0;
  e.coef_asimetria = 0;
  e.ancho = 0;
  e.alto = 0;
  double contador = 0;

  for(int i = 0; i < capa.rows; i++){
    for(int j = 0; j < capa.cols; j++){
      if(!mflag or mascara.at <uchar> (i,j)==255){
      contador++;
      //Minimo
      if(capa.at <uchar> (i,j) < e.min)
        e.min = capa.at <uchar> (i,j);
      //Maximo
      if(capa.at <uchar> (i,j) > e.max)
        e.max = capa.at <uchar> (i,j);
      //Suma
      e.suma = e.suma + capa.at <uchar> (i,j);
      //suma de cuadrados
      e.suma_cuadrados = e.suma_cuadrados + pow(capa.at <uchar> (i,j), 2);
      //numero de ceros
      if(capa.at <uchar> (i,j) == 0)
        e.num_ceros += 1;
      if(capa.at <uchar> (i,j) > 0)
        e.area_positiva += capa.at <uchar> (i,j);
      if(capa.at <uchar> (i,j) < 0)
        e.area_negativa += capa.at <uchar> (i,j);
      }
    }
}
    e.ancho = capa.cols;
    e.alto = capa.rows;
    e.media = e.suma / contador;
    for(int i = 0; i < capa.rows; i++){
      for(int j = 0; j < capa.cols; j++){
          e.varianza += pow((capa.at <uchar> (i,j) - e.media) , 2);
          e.coef_asimetria += pow((capa.at <uchar> (i,j) - e.media) , 3);
      }
    }

    e.varianza /= (contador);
    e.coef_asimetria /= ((contador) * (pow(sqrt(e.varianza), 3)));


}

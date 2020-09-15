#include <iostream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include "bmplib.h"

using namespace std;
void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
              int N, double kernel[][11]);
void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);
void gaussian(double k[][11], int N, double sigma);
void gaussian_filter(unsigned char out[][SIZE][RGB], unsigned char 
		in[][SIZE][RGB], int N, double sigma);
void unsharp(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
		int N, double sigma, double alpha);

#ifndef AUTOTEST

int main(int argc, char* argv[])
{
  if(argc < 3)
    {
      cout << "usage: ./filter <input file> <filter name> <filter parameters>";
      cout << " <output file name>" << endl;
      return -1;
    }
   unsigned char input[SIZE][SIZE][RGB];
   unsigned char output[SIZE][SIZE][RGB];
   char* outfile;
   int N;
   double sigma, alpha;
   int status = readRGBBMP(argv[1], input); 
   if(status != 0)
   {
      cout << "unable to open " << argv[1] << " for input." << endl;
      return -1;
   }
   if( strcmp("sobel", argv[2]) == 0)
   {
     sobel(output, input);
     outfile = argv[3];
   }
   else if( strcmp("blur", argv[2]) == 0)
   {
     if(argc < 6)
       {
	 cout << "not enough arguments for blur." << endl;
	 return -1;
       }
     N = atoi(argv[3]);
     sigma = atof(argv[4]);
     outfile = argv[5];
     gaussian_filter(output, input, N, sigma);
   }
   else if( strcmp("unsharp", argv[2]) == 0)
   {
     if(argc < 7)
       {
	 cout << "not enough arguments for unsharp." << endl;
	 return -1;
       }
     N = atoi(argv[3]);
     sigma = atof(argv[4]);
     alpha = atof(argv[5]);
     outfile = argv[6];
     unsharp(output, input, N, sigma, alpha);

   }
   else if( strcmp("dummy", argv[2]) == 0)
   {
     dummy(output, input);
     outfile = argv[3];
   }
   else
   {
      cout << "unknown filter type." << endl;
      return -1;
   }

   if(writeRGBBMP(outfile, output) != 0)
   {
      cout << "error writing file " << argv[3] << endl;
   }

}   

#endif
void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB])
{
   double k[11][11];
   for (int i = 0; i < 3; i++)
   {
      for(int j = 0; j < 3; j++)
      {
         k[i][j] = 0;
      }
   }
   k[1][1] = 1;
   convolve(out, in, 3, k);
}
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB], 
	      int N, double kernel[][11])
{
 
   int padded[SIZE+10][SIZE+10][RGB];
   int temp[SIZE][SIZE][RGB];
    for (int a = 0; a < (SIZE + 10); a++){
       for (int b = 0; b < (SIZE + 10); b++){
           for (int c = 0; c < RGB; c++){
               padded[a][b][c] = 0;
           }
       }
   }
    for (int a = N/2; a < (SIZE + N/2); a++){
       for (int b = N/2; b < (SIZE + N/2); b++){
           for (int c = 0; c < RGB; c++){
               padded[a][b][c] = in[a-(N/2)][b-(N/2)][c];
           }
       }
   }
   for (int a = 0; a < SIZE; a++){
       for (int b = 0; b < SIZE; b++){
           for (int c = 0; c < RGB; c++){
               temp[a][b][c] = 0;
           }
       }
   }
  double convolve_temp;
  for(int y= 0 ; y<SIZE ;y++){
      for(int x=0 ; x<SIZE ;x++){
          for(int k=0; k<RGB;k++){
              convolve_temp = 0;
              for(int i=0 ; i<N ; i++){
                  for(int j=0 ; j<N ; j++){
                      convolve_temp += kernel[i][j] * padded[y+i][x+j][k];
                  }
              }
              temp[y][x][k] = (int) convolve_temp;
          }
      }
  }
   for(int a= 0 ; a<SIZE ;a++){
      for(int b=0 ; b<SIZE ;b++){
          for(int c=0; c<RGB;c++){
              if (temp[a][b][c] > 255){
                  temp[a][b][c] = 255;
              }
              else if (temp[a][b][c] < 0){
                  temp[a][b][c] = 0;
              }
              out[a][b][c] = (unsigned char) temp[a][b][c];
          }
      }
   }
    
   



}
void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB])
{
   double k[11][11];
   double s_h1[3][3] = { {-1, 0, 1}, 
                         {-2, 0, 2}, 
                         {-1, 0, 1} };
   double s_h2[3][3] = { {1, 0, -1}, 
                         {2, 0, -2}, 
                         {1, 0, -1} };
   
   unsigned char h1_soble[SIZE][SIZE][RGB];
   unsigned char h2_soble[SIZE][SIZE][RGB]; 

   for (int i = 0; i < 11; i++)
   {
      for(int j=0; j < 11; j++)
      {
         k[i][j] = 0;
      }
   }
    for (int a = 0; a<3; a++){
        for (int b = 0; b<3; b++){
            k[a][b] = s_h1[a][b];
        }
    }
    convolve(h1_soble, in, 3, k);
    for (int a = 0; a<3; a++){
        for (int b = 0 ; b<3; b++){
            k[a][b] = s_h2[a][b];
        }
    }
    convolve(h2_soble, in, 3, k);
    unsigned char temp[SIZE][SIZE][RGB] = {0};
    for(int a= 0 ; a<SIZE ;a++){
      for(int b=0 ; b<SIZE ;b++){
          for(int c=0; c<RGB;c++){
              temp[a][b][c] = (unsigned char) h1_soble[a][b][c] + 
                  h2_soble[a][b][c];
              if (temp[a][b][c] > 255){
                  temp[a][b][c] = 255;
              }
              else if (temp[a][b][c] < 0){
                  temp[a][b][c] = 0;
              }
              out[a][b][c] = temp[a][b][c];
          }
      }
    }


}

void gaussian(double k[][11], int N, double sigma){
    double sum = 0;

    for (int a = 0; a<N; a++){
        for (int b = 0; b<N; b++){
            k[a][b] = exp(-((pow(b-(N/2),2)/(2*pow(sigma,2))) 
                     + (pow(a - (N/2),2)/(2*pow(sigma,2)))));
          
        }
    }
    for (int a = 0; a<N; a++){
        for (int b = 0; b<N; b++){
            sum+= k[a][b];
        }
    }
    
    for (int a = 0; a<N; a++){
        for (int b = 0; b<N; b++){
            k[a][b] = (k[a][b]/sum);
            cout << k[a][b] << ' ';
        }
        cout << endl;
    }
}

void gaussian_filter(unsigned char out[][SIZE][RGB], 
                     unsigned char in[][SIZE][RGB], 
                     int N, double sigma){
    double k[11][11];
    for(int a=0; a<11;a++){
        for(int b=0; b<11; b++){
            k[a][b]=0;
        }
    }
    gaussian(k, N, sigma);
    convolve(out, in, N, k);
    }
    
void unsharp(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
		int N, double sigma, double alpha)
{
    unsigned char temp_array[SIZE][SIZE][RGB];
    int temp_int;
    gaussian_filter(temp_array, in, N, sigma);
    for(int a = 0; a < SIZE; a++){
        for(int b = 0; b < SIZE; b++){
            for(int c = 0; c < RGB; c++){
                temp_int = -temp_array[a][b][c] + in[a][b][c];
                temp_int = (int) in[a][b][c] + (alpha*temp_int);
                if (temp_int < 0){
                    temp_int = 0;
                }
                else if (temp_int > 255){
                    temp_int = 255;
                }
                out[a][b][c] = (unsigned char) temp_int;
                
            }
        }
    }
}
                                    
    
        


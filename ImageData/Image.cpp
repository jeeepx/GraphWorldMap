#include "Image.h"
#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"
#include <iostream>

using cs225::HSLAPixel;
using cs225::PNG;


void Image::lighten(){
   for(unsigned x = 0; x < width(); x++){
     for(unsigned y = 0; y < height(); y++){
       HSLAPixel & pixel = getPixel(x,y);
       pixel.l += 0.1;
       if(pixel.l > 1)  
         pixel.l = 1;
      }
    }    
}
void Image::lighten(double amount){
   for(unsigned x = 0; x < width(); x++){
       for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.l += amount;
           if(pixel.l > 1)
             pixel.l = 1;
        }
    }
}

void Image::darken(){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.l -= 0.1;
           if(pixel.l < 0)
             pixel.l = 0;
        }
    }
}
void Image::darken(double amount){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.l -= amount;
           if(pixel.l < 0)
             pixel.l = 0;
        }
    }
}

void Image::saturate(){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.s += 0.1;
           if(pixel.s > 1)
             pixel.s = 1;
        }
    }
}
void Image::saturate(double amount){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.s += amount;
           if(pixel.s > 1)
             pixel.s = 1;
        }
    }
}

void Image::desaturate(){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.s -= 0.1;
           if(pixel.s < 0)
             pixel.s = 0;
        }
    }
}
void Image::desaturate(double amount){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
           HSLAPixel & pixel = getPixel(x,y);
           pixel.s -= amount;
           if(pixel.s < 0)
             pixel.s = 0;
        }
    }
}
void Image::grayscale(){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
            HSLAPixel & pixel = getPixel(x,y);
            pixel.s = 0;
        }
    }
}
void Image::illinify(){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            if( (pixel.h > 113.5) && (pixel.h < 293.5) )
              pixel.h = 216;
         
            else 
              pixel.h = 11;

        }
    }
}
void Image::rotateColor(double degrees){
    for(unsigned x = 0; x < width(); x++){
        for(unsigned y = 0; y < height(); y++){
            HSLAPixel & pixel = getPixel(x,y);
            pixel.h += degrees;
            while(pixel.h > 360){
                pixel.h -= 360;
            }
            while(pixel.h < 0){
                pixel.h += 360;
            }
        }
    }
}
void Image::scale(double factor){
    if(factor > 1){
        unsigned new_width = factor * width();
        unsigned new_height = factor * height();
        resize(new_width, new_height);             //new size image-PNG class

        for(int x = width() - 1; x >= 0 ; x--){
          for(int y = height() - 1; y >= 0; y--){
              HSLAPixel & white_pixel = getPixel((unsigned) x ,(unsigned)y);
              HSLAPixel & filled_pixel = getPixel( (unsigned) x / factor , (unsigned) y / factor);
              white_pixel = filled_pixel;
            } 
        }

    }
    if(factor < 1){
        unsigned new_width = factor * width();
        unsigned new_height = factor * height();
        for( unsigned x = 0; x < new_width; x++){
            for(unsigned y = 0; y < new_height; y++){
                HSLAPixel & scaled_pixel = getPixel(x,y);
                HSLAPixel & old_pixel = getPixel( x / factor, y/factor);
                scaled_pixel = old_pixel;
            }
        }
        resize(new_width, new_height);
    }

}

void Image::scale(unsigned w, unsigned h){
    double newwidth_f = (double) w / (double) width();
    double newheight_f = (double) h / (double) height();
    double f = std::min(newwidth_f, newheight_f);
    scale(f);
}
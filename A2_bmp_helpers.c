/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 * 
 * Each of the functions below can be considered a start for you. 
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */
/*  NAME : FURAHA DAMIEN
    ID # : 260754407
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

//-----------------------------HELPER FUNCTIONS----------------------------------------
//helper function for Q3
/*I use this function checks if the dimensions of the second image fall on the 
top of the first image or the image should be attached to the first depending on the
y and x values
*/
int  fit(int x,int y,int x_max,int x_min,int y_max, int y_min){
  if(x<x_max & x>=x_min & y<y_max& y>=y_min){
    return 1;
  }else{
    return 0;
  }
}

//max function for Q3
//This function returns the maximum of two numbers
/*I call this function in question three to check if the width& and height of 
the second image plus the offset is greater than that of the first image so that
 i can calculate the area when my result image will be
*/
int max(int value_1,int value_2){
  if(value_1>value_2){
    return value_1;
  }else{
    return value_2;
  }
}

//I use this function to convert bits to pixels
int convert_bits_to_pixel(int bits_per_pixel, int value){
  return value/bits_per_pixel;
}
//---------------------------END OF HELPER FUNCTIONS-----------------------------------

int bmp_open( char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset, unsigned char** img_data ){

              
  // YOUR CODE FOR Q1 SHOULD REPLACE EVERYTHING FROM HERE
  
  *width=0;
  *height=0;
  *bits_per_pixel=0;
  *padding=0;
  *data_size=0;
  *data_offset=0;
  *img_data=NULL;
  


  FILE *bmpfile = fopen( bmp_filename, "rb" );

  // Read the B and M characters into chars
  char b, m;
  fread (&b,1,1,bmpfile);
  fread (&m,1,1,bmpfile);

  // Read the overall file size 
  fread( data_size, 1, sizeof(unsigned int), bmpfile );
  
  // Close the file, re-open it to be at the beginning, and read the entire contents
  fclose(bmpfile);
  bmpfile = fopen(bmp_filename, "rb" );

  //SET DATA SIZE USING BYTE 2

  /*setting the value to the parameters depending on type at which they are
   in the header. Heap memory used instead of stack memory for image data
  */
  *img_data = (unsigned char *) malloc(*data_size);
  fread(*img_data, 1, *data_size, bmpfile);

  *width = *(unsigned int *)(*img_data + 18); 
  *height = *(unsigned int *)(*img_data + 22); 
  *bits_per_pixel = *(unsigned int *)(*img_data + 28); 
  *padding = (4-((((*width)*(*bits_per_pixel))/8)%4))%4;
  *data_offset = *(unsigned int *)(*img_data + 10); 


  
  // TO HERE
  return 0;  
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close( unsigned char **img_data ){

  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
}

int bmp_mask( char* input_bmp_filename, char* output_bmp_filename, 
              unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max,
              unsigned char red, unsigned char green, unsigned char blue )
{
  unsigned int img_width;
  unsigned int img_height;
  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned int data_offset;
  unsigned char* img_data    = NULL;
  
  int open_return_code = bmp_open( input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data ); 
  
  if( open_return_code ){ printf( "bmp_open failed. Returning from bmp_mask without attempting changes.\n" ); return -1; }
 
  // YOUR CODE FOR Q2 SHOULD REPLACE EVERYTHING FROM HERE

  /*In this function, i use two nested loops.One loop iterates throught the vertical
    pixels while the other goes through the horizontal pixels while setting the appropriate 
    colors. The i write the changed pixels to my output file    
  */
  
  unsigned char *pixel_data = img_data + data_offset;
  unsigned char num_colors = bits_per_pixel/8;

  //iterating through the pixels
  for(int x = x_min; x<= x_max; x++){
    for( int y = y_min; y<=y_max; y++){

      //setting the colors as done in bmp_info.c
      pixel_data[ y*(img_width*num_colors+padding) + x*num_colors + 2 ] = red;
      pixel_data[ y*(img_width*num_colors+padding) + x*num_colors + 1 ] = green;
      pixel_data[ y*(img_width*num_colors+padding) + x*num_colors + 0 ] = blue;
    }
  }

FILE *output_bmp_file = fopen(output_bmp_filename, "wb");

//writing the output image from the collected image data
fwrite(img_data, sizeof(unsigned int), data_size,output_bmp_file);

fclose(output_bmp_file);


  // TO HERE!
  
  bmp_close( &img_data );
  
  return 0;
}         

int bmp_collage( char* bmp_input1, char* bmp_input2, char* bmp_result, int x_offset, int y_offset ){

  unsigned int img_width1;
  unsigned int img_height1;
  unsigned int bits_per_pixel1;
  unsigned int data_size1;
  unsigned int padding1;
  unsigned int data_offset1;
  unsigned char* img_data1    = NULL;
  
  int open_return_code = bmp_open( bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1 ); return -1; }
  
  unsigned int img_width2;
  unsigned int img_height2;
  unsigned int bits_per_pixel2;
  unsigned int data_size2;
  unsigned int padding2;
  unsigned int data_offset2;
  unsigned char* img_data2    = NULL;
  
  open_return_code = bmp_open( bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2 ); return -1; }
  
  // YOUR CODE FOR Q3 SHOULD REPLACE EVERYTHING FROM HERE

  /*For this function i use two helper functions decribed above
    First i obtain width and height for the whole image that i am gonna have after the
    collage.
    I run two for loops that through the horizontal and vertical bytes of the file i am 
    collaging it while setting the pixels appropriately based on the conditions that i set 
  */
  
  unsigned int canvas_width; 
  unsigned int canvas_height; 

//setting the dimensions of the template(canvas) imagedepending on provided offsets
//I call the MAX helper function
if(y_offset < 0){
  
  canvas_height = max(img_height1 - y_offset,img_height2);
}else{

  canvas_height = max(img_height1,img_height2+y_offset);
}

if(x_offset < 0){
  canvas_width = max(img_width1 - x_offset,img_width2);
}else{

 
  canvas_width=max(img_width1,img_width2+x_offset);

}

  unsigned int canvas_x_max = canvas_width;
  unsigned int canvas_y_max = canvas_height;

  //x-y boundaries for img2
  unsigned int img2_x_min ;
  unsigned int img2_y_min ;
  unsigned int img2_x_max ;
  unsigned int img2_y_max ;

  //x-y boundaries for img1
  unsigned int img1_x_min ;
  unsigned int img1_y_min ;
  unsigned int img1_x_max ;
  unsigned int img1_y_max ;

    /*canvas_img_data will contain the data for the tempory header of the file that we
    create after collaging  
    */
  unsigned char* canvas_img_data; //holds data for th etemplate image
  unsigned int padding = (4-((((canvas_width)*(bits_per_pixel1))/8)%4))%4;
    
  unsigned int width_b = (canvas_width * bits_per_pixel1);
  unsigned int height_b = (canvas_height * bits_per_pixel1);
  unsigned int padding_b = padding/8;
   
  unsigned int row_width_byte = (width_b + padding_b)/8;
  
  unsigned int canvas_total_size = (data_offset1)+((width_b+padding_b)*height_b/8); //combined img size
  canvas_img_data = (unsigned char*) calloc((canvas_total_size),sizeof(unsigned int));
  unsigned int canvas_img_size = canvas_total_size - data_offset1;

  unsigned int num_colors; //helps in setting pixels

  if(data_offset1 > data_offset2){
    memcpy(canvas_img_data, img_data1,data_offset1);
    num_colors = bits_per_pixel1/8; //relative to img1
  }
  else{
    memcpy(canvas_img_data,img_data2,data_offset2); //relative to img2
    num_colors = bits_per_pixel2 / 8;
  }

//adjusting the header
 memcpy(canvas_img_data +18 , &canvas_width, sizeof(unsigned int));
 memcpy(canvas_img_data +22 , &canvas_height, sizeof(unsigned int));
 memcpy(canvas_img_data +2 , &canvas_total_size, sizeof(unsigned int));
 memcpy(canvas_img_data + 34,&canvas_img_size,sizeof(unsigned int));

 unsigned char *canvas_pixel_data = canvas_img_data + data_offset1;
 unsigned char *img1_pixel_data = img_data1 + data_offset1;
 unsigned char *img2_pixel_data = img_data2 + data_offset2;

 unsigned int red,green,blue;
 unsigned int num_colors1 = (bits_per_pixel1)/8;
 unsigned int num_colors2 = (bits_per_pixel2)/8;

 //getting offsets for img1 and img2 to calculate where the images meet
 unsigned int x_offset_img1;
 unsigned int x_offset_img2;
 unsigned int y_offset_img1;
 unsigned int y_offset_img2;

  if(x_offset >= 0){
    x_offset_img1 = 0;
    x_offset_img2 = x_offset;
  }else{
    x_offset_img2 = 0;
    x_offset_img1 = -x_offset;
  }

  if(y_offset >= 0){
    y_offset_img1 = 0;
    y_offset_img2 = y_offset;
  }else{
    y_offset_img2 = 0;
    y_offset_img1 = -y_offset;
  }

  //img2 boundaries relative to its offsets
  img2_x_min = x_offset_img2;
  img2_y_min = y_offset_img2;
  img2_x_max = img_width2 + x_offset_img2;
  img2_y_max = img_height2 + y_offset_img2;

  //img1 boundaries relative to its offsets
  img1_x_min = x_offset_img1;
  img1_y_min = y_offset_img1;
  img1_x_max = img_width1 + x_offset_img1;
  img1_y_max = img_height1 + y_offset_img1;

  /*This nested for loop sets the colors on the pixels by first going in the horizonta direction the vertical
    I use the FIT function to check if at the specific poin i shoud input the pixels of img1 or img2 or nothing
  */
  for(int y = 0; y <= canvas_y_max; y++){
    for(int x = 0; x<= canvas_x_max; x++){
      if(fit(x,y,img2_x_max,img2_x_min,img2_y_max,img2_y_min)){
        //put pixels of img2
        blue = img2_pixel_data[ (y-y_offset_img2)*(img_width2*num_colors2+padding2) + (x-x_offset_img2)*num_colors2 + 2 ] ;
        green = img2_pixel_data[ (y-y_offset_img2)*(img_width2*num_colors2+padding2) + (x-x_offset_img2)*num_colors2 + 1 ] ;
        red = img2_pixel_data[ (y-y_offset_img2)*(img_width2*num_colors2+padding2) + (x-x_offset_img2)*num_colors2 + 0 ] ;
      
      }
      else if(fit(x,y,img1_x_max,img1_x_min,img1_y_max,img1_y_min)){
        
        //put pixel of img1
        blue = img1_pixel_data[ (y-y_offset_img1)*(img_width1*num_colors1+padding1) + (x-x_offset_img1)*num_colors1 + 2 ] ;
        green = img1_pixel_data[ (y-y_offset_img1)*(img_width1*num_colors1+padding1) + (x-x_offset_img1)*num_colors1 + 1 ] ;
        red = img1_pixel_data[ (y-y_offset_img1)*(img_width1*num_colors1+padding1) + (x-x_offset_img1)*num_colors1 + 0 ] ;

      }else{
        //these are default pixels(white) when this part of canvas image shouldnt have img1 or img2 pixels
        //basically just sets the pixels of the background that we have calculated
        red = 255;
        green = 255;
        blue = 255;

      }
      //I set the pixels of theh template image to respective color values
      canvas_pixel_data[ y*(canvas_width*num_colors+padding) + x*num_colors + 2 ] = blue;
      canvas_pixel_data[ y*(canvas_width*num_colors+padding) + x*num_colors + 1 ] = green;
      canvas_pixel_data[ y*(canvas_width*num_colors+padding) + x*num_colors + 0 ] = red;
    }
  }

  //finally i write all my obatined pixels into the result bmp file
  FILE *output_bmp_file = fopen(bmp_result, "wb");

fwrite(canvas_img_data,sizeof(unsigned int),canvas_total_size,output_bmp_file);

fclose(output_bmp_file);
//fclose(canvas_img_data);

  // TO HERE!
      
  bmp_close( &img_data1 );
  bmp_close( &img_data2 );
  bmp_close( &canvas_img_data);
  
  return 0;
}
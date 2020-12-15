#include "noisetex.h"

#include <cstdio>

#include <glm/glm.hpp>
// Include the GLM header that includes the noise functions
#include <glm/gtc/noise.hpp>


// Stores texture in texID after creating it from scratch
int NoiseTex::storeTex( GLubyte * data, int w, int h ) {
  GLuint texID;
  // creating a new texture object, store in texID
  glGenTextures(1, &texID);
  // bind new texture object to GL_TEXTURE_2D
  glBindTexture(GL_TEXTURE_2D, texID);
  // allocate immutable storage for the texture
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
  // copy the data for that texture into the texture object, last argument is pointer to raw data
  glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data);
  // setting the magnification and minification filters for the texture object
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // the integer part of the coordinate will be ignored and a repeating pattern is formed (s,t,r) -> (x,y,z) in texture coordinates
  // repeating in direction x
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // repeating in direction y
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  return texID;
}

// instead of loading an image we generate one texel by texel with a noise function
int NoiseTex::generate2DTex(float baseFreq, float persistence, int w, int h, bool periodic) {

  int width = w;
  int height = h;

  //printf("Generating noise texture...");

  // allocating a buffer named data to hold the generated noise values
  GLubyte *data = new GLubyte[ width * height * 4 ];

  // generating x and y coordinates with width and height
  float xFactor = 1.0f / (width - 1);
  float yFactor = 1.0f / (height - 1);

  // nested for-loop over each texel
  for( int row = 0; row < height; row++ ) {
    for( int col = 0 ; col < width; col++ ) {
      float x = xFactor * col;
      float y = yFactor * row;

      float sum = 0.0f;
      float freq = baseFreq;
      float persist = persistence;

      // loop over octaves, compute the sum, 4 octaves
      for( int oct = 0; oct < 4; oct++ ) {
        // vector 2 with frequency x and y needed for perlin input parameter
        glm::vec2 p(x * freq, y * freq);
        
        float val = 0.0f;
        // GLM library Perlin Noise function, returns float between -1 and 1
        if (periodic) {
          // for seamless texture overload Perlin Noise function
          // the 2nd parameter determines the period in x and y of the noise values
          // We use freq as the period because we are sampling the noise in the range from 0 to freq for each octave
          val = glm::perlin(p, glm::vec2(freq)) * persist;
        } else {    // for a non seamless texture don't
          val = glm::perlin(p) * persist;
          //val = glm::simplex(p) * persist;
        }
        // sum of the previous equation, storing the first term in the first component, the first two terms in the second, and so on
        sum += val;
        //
        float result = (sum + 1.0f) / 2.0f;

        // Clamp strictly between 0 and 1
        result = result > 1.0f ? 1.0f : result;
        result = result < 0.0f ? 0.0f : result;

        // Multiplied by 255 and cast to a byte -> Grey value
        data[((row * width + col) * 4) + oct] = (GLubyte) ( result * 255.0f );
        // Double the frequency
        freq *= 2.0f;
        persist *= persistence;
      }
    }
  }
  // save 
  int texID = NoiseTex::storeTex(data, width, height);
  // tidying up
  delete [] data;

  //printf("done.\n");
  return texID;
}

int NoiseTex::generatePeriodic2DTex(float baseFreq, float persist, int w, int h) {
  return NoiseTex::generate2DTex(baseFreq, persist, w, h, true);
}

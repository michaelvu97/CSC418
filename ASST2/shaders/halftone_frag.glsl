precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

float round(float x) {
  if (x > 0.0)
    return floor(x + 0.5);
  else
    return floor(x - 0.5);

}

float segment(float position, float divisor) {
  return round(position / divisor) * divisor;
}

void main() {

  // The screen will be divided into circles with <dist> between them
  float dist = 10.0;

  // Find the nearest centroid
  vec4 centre = gl_FragCoord;
  centre.x = segment(centre.x, dist);
  centre.y = segment(centre.y, dist);

  // Makes the halftone look really nice
  float radiusGrowthMagicNumber = 0.25;

  float intensity = max(0.0, dot(normalInterp, normalize(lightPos - vertPos)));
  intensity = intensity * Kd;

  // Cutoff is the radius to draw the ambient colour, as a function of intensity
  float cutoff = radiusGrowthMagicNumber * dist * (1.0 - intensity);
  cutoff = cutoff * cutoff;

  // Draw ambient if the "darkness" cutoff is greater than the dist to the
  // nearest centroid.
  if (dot(gl_FragCoord - centre, gl_FragCoord - centre) <= cutoff) {

    gl_FragColor = vec4(ambientColor, 1.0);

  } else {
    gl_FragColor = vec4(diffuseColor, 1.0);  
  }

  
}
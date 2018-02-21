attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;    // Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal (eye space?)
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex


void main(){
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.

  // Move the vertex from it's object-local space to the view space.
  vec4 vertPos4 = modelview * vec4(position, 1.0);

  // Transform the vertex from view space to projected space.
  gl_Position = projection * vertPos4;

  viewVec = (mat3(modelview) * position);

  // Transform the vertex from object space to view space.
  vertPos = vec3(vertPos4);
  normalInterp = normalize(mat3(normalMat) * normal);
}

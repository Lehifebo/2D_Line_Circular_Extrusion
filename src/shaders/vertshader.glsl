#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout(location = 0) in vec3 vertCoordinates_in;
layout(location = 1) in vec3 vertNormals;
uniform mat4 modelTransform;
uniform mat4 projectionTransform;


// Specify the output of the vertex stage
out vec3 vertColor;

void main() {
  // gl_Position is the output (a vec4) of the vertex shader

  gl_Position =  projectionTransform * modelTransform * vec4(vertCoordinates_in, 1.0F) ;
  vertColor = vertNormals;
}

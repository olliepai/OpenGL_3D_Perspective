attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

varying vec2 textureCoordinate;

void main(){
    textureCoordinate = texCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
}
attribute vec2 position;
attribute vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

varying vec2 textureCoordinate;

void main(){
    textureCoordinate = texCoord;
    gl_Position = modelMatrix * projectionMatrix * vec4(position, 0, 1);
}
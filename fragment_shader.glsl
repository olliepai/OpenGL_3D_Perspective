uniform sampler2D tex;

varying vec2 textureCoordinate;

void main(){
    vec4 col = texture2D(tex, textureCoordinate);
    gl_FragColor = col;
}
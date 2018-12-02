#include "SDL2/SDL.h"
#include "OpenGL/gl3.h"

#include <time.h>
#include <stdlib.h>

#include "shader_compiler.h"
#include "graphics_math.h"

#define GRAVITY -1
#define MAX_TRIANGLES 4096

#define WIDTH 900
#define HEIGHT 500

struct Triangle {
    float position[2];
    float velocity[2];
};

int main(int argc, char** argv){
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    char* vertexData = readFileToCharArray("vertex_shader.glsl");
    char* fragData = readFileToCharArray("fragment_shader.glsl");

    unsigned int shader = compileShaderVF(vertexData, fragData);
    glUseProgram(shader);
    int postionId = glGetAttribLocation(shader, "position");
    int texCoordId = glGetAttribLocation(shader, "texCoord");

    int modelMatId = glGetUniformLocation(shader, "modelMatrix");
    int projectionMatId = glGetUniformLocation(shader, "projectionMatrix");
    int viewMatId = glGetUniformLocation(shader, "viewMatrix");


    Camera camera;
    camera.forward = vec3(0, 0, 1);
    camera.right = vec3(1, 0, 0);
    camera.up = vec3(0, 1, 0);
    camera.orientation = quat(0, 0, 0, 1);
    camera.setPerspectiveProjection(70.0, (float)WIDTH/(float)HEIGHT, 0.001, 1000.0);
    glUniformMatrix4fv(projectionMatId, 1, false, camera.projection.m[0]);

    mat4 viewMatrix;
    viewMatrix.setIdentity();
    camera.position = vec3(0, 0, -10);
    viewMatrix.translate(camera.position);
    glUniformMatrix4fv(viewMatId, 1, false, viewMatrix.m[0]);

    float verts[] = {
        -1, -1, 0, 0, 1, 
        0, 1, 0, 0.5, 0,
        1, -1, 0, 1, 1,
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(postionId, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(postionId);
    glVertexAttribPointer(texCoordId, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(texCoordId);

    unsigned char texture[] = {
        255,255,255,255,0,0,0,255,
        0,0,0,255,255,255,255,255
    };

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture); 

    glClearColor(0, 0.5, 1, 1);

    mat4 modelMatrix;
    modelMatrix.setIdentity();
    modelMatrix.translate(vec3(0, 0, 0));

    float p = 0.0;
    glUniformMatrix4fv(modelMatId, 1, false, modelMatrix.m[0]);

    SDL_Event event;
    bool isRunning = true;

    bool moveForward = false;
    bool moveBack = false;
    bool moveUp = false;
    bool moveDown = false;
    bool moveRight = false;
    bool moveLeft = false;
    bool pitchUp = false;
    bool pitchDown = false;
    bool rollLeft = false;
    bool rollRight = false;
    bool yawLeft = false;
    bool yawRight = false;

    float camMoveSpeed = 0.1;
    float camRotateSpeed = 0.01;

    while(isRunning){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:{
                    isRunning = false;
                    break;
                }
                case SDL_KEYDOWN :{
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        isRunning = false;
                    }else if(event.key.keysym.sym == SDLK_UP){ 
                        pitchUp = true;
                    }else if(event.key.keysym.sym == SDLK_DOWN){
                        pitchDown = true;
                    }else if(event.key.keysym.sym == SDLK_LEFT){
                        yawLeft = true;
                    }else if(event.key.keysym.sym == SDLK_RIGHT){
                        yawRight = true;
                    }else if(event.key.keysym.sym == SDLK_w){
                        moveForward = true;
                    }else if(event.key.keysym.sym == SDLK_s){
                        moveBack = true;
                    }else if(event.key.keysym.sym == SDLK_a){
                        moveLeft = true;
                    }else if(event.key.keysym.sym == SDLK_d){
                        moveRight = true;
                    }else if(event.key.keysym.sym == SDLK_q){
                        rollLeft = true;
                    }else if(event.key.keysym.sym == SDLK_e){
                        rollRight = true;
                    }else if(event.key.keysym.sym == SDLK_r){
                        moveUp = true;
                    }else if(event.key.keysym.sym == SDLK_f){
                        moveDown = true;
                    }

                    break;
                }
                case SDL_KEYUP :{
                    if(event.key.keysym.sym == SDLK_UP){ 
                        pitchUp = false;
                    }else if(event.key.keysym.sym == SDLK_DOWN){
                        pitchDown = false;
                    }else if(event.key.keysym.sym == SDLK_LEFT){
                        yawLeft = false;
                    }else if(event.key.keysym.sym == SDLK_RIGHT){
                        yawRight = false;
                    }else if(event.key.keysym.sym == SDLK_w){
                        moveForward = false;
                    }else if(event.key.keysym.sym == SDLK_s){
                        moveBack = false;
                    }else if(event.key.keysym.sym == SDLK_a){
                        moveLeft = false;
                    }else if(event.key.keysym.sym == SDLK_d){
                        moveRight = false;
                    }else if(event.key.keysym.sym == SDLK_q){
                        rollLeft = false;
                    }else if(event.key.keysym.sym == SDLK_e){
                        rollRight = false;
                    }else if(event.key.keysym.sym == SDLK_r){
                        moveUp = false;
                    }else if(event.key.keysym.sym == SDLK_f){
                        moveDown = false;
                    }
                    break;
                }
                case SDL_MOUSEBUTTONDOWN :{
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    break;
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);

        camera.position -= vec3(camera.forward.x * camMoveSpeed * moveForward,
                                camera.forward.y * camMoveSpeed * moveForward,
                                camera.forward.z * camMoveSpeed * moveForward);

        camera.position += vec3(camera.forward.x * camMoveSpeed * moveBack,
                                camera.forward.y * camMoveSpeed * moveBack,
                                camera.forward.z * camMoveSpeed * moveBack);

        camera.position -= vec3(camera.right.x * camMoveSpeed * moveRight,
                                camera.right.y * camMoveSpeed * moveRight,
                                camera.right.z * camMoveSpeed * moveRight);

        camera.position += vec3(camera.right.x * camMoveSpeed * moveLeft,
                                camera.right.y * camMoveSpeed * moveLeft,
                                camera.right.z * camMoveSpeed * moveLeft);

        camera.orientation.rotate(camera.right, -camRotateSpeed * pitchUp);
        camera.orientation.rotate(camera.right, camRotateSpeed * pitchDown);
        camera.orientation.rotate(camera.up, -camRotateSpeed * yawLeft);
        camera.orientation.rotate(camera.up, camRotateSpeed * yawRight);
        camera.orientation.rotate(camera.forward, camRotateSpeed * rollLeft);
        camera.orientation.rotate(camera.forward, -camRotateSpeed * rollRight);

        camera.forward = getForwardVector(viewMatrix);
        camera.up = getUpVector(viewMatrix);
        camera.right = getRightVector(viewMatrix);

        viewMatrix.setIdentity();
        viewMatrix.translate(camera.position);
        viewMatrix = multiply(quatToMat4(camera.orientation), viewMatrix);
        glUniformMatrix4fv(viewMatId, 1, false, viewMatrix.m[0]);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    return 0;
}
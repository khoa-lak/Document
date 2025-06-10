#include "config.h"

int main(){
    GLFWwindow* window;
    if(!glfwInit()){
        std::cout<<"error glfw"<<std::endl;
        return -1;
    }
    window = glfwCreateWindow(600,400,"MyApp",NULL,NULL);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
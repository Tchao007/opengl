#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_51.h"

#include <iostream>


const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height,"first 3d",NULL,NULL);
    if(window ==NULL){

        std::cout<<"failed create a window"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){

        std::cout<<"load glad failed"<<std::endl;
        return -1;
    }

    Shader ourShader("demo3d.vs","demo3d.fs");

    float vertices[] = {
        // positions          // texture coords
         0.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         0.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    float vertices2[] = {
		//位置            //纹理对应位置
		1.0f, 1.0f, 0.0f,		1.0f,1.0f,  //右上
		1.0f, -1.0f, 0.0f,	 	1.0f,0.0f,//右下
		0.0f, -1.0f, 0.0f,		0.0f,0.0f,//左下
		0.0f, 1.0f, 0.0f,		0.0f,1.0f//左上
	};
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO[2],VBO[2],EBO[2];
    glGenVertexArrays(2,VAO);
    glGenBuffers(2,VBO);
    glGenBuffers(2,EBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*(sizeof(float)),(void *)0);
    glEnableVertexAttribArray(0);
    //注意到，因为偏移量参数需要已float的大小为单位!!!缺失就没有纹理了！！！
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*(sizeof(float)),(void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*(sizeof(float)),(void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
#if 1
    unsigned int texture1;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("./timg.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {   
        std::cout<<"load picture"<<std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
#endif
#if 1   
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*(sizeof(float)),(void*)(0));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*(sizeof(float))));
    glEnableVertexAttribArray(1);

    unsigned int texture2;
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    data = stbi_load("../res/smell.jpg",&width,&height,&nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout<<"failed picture2"<<std::endl;
    }
    stbi_image_free(data);
#endif
    ourShader.use();
    glBindVertexArray(VAO[0]);
    //glUniform1i(glGetUniformLocation(ourShader.ID,"texture1"),0);
    glUniform1i(glGetUniformLocation(ourShader.ID,"texture1"),0);
    glBindVertexArray(VAO[1]);
    glUniform1i(glGetUniformLocation(ourShader.ID,"texture1"),0);


    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(VAO[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);


        glBindVertexArray(VAO[1]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture2);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteVertexArrays(2,VAO);
    glDeleteBuffers(2,VBO);
    glDeleteBuffers(2,EBO);

    glfwTerminate();
    return 0;

}
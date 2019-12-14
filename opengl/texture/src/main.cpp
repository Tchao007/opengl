#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
 
// 设置窗体宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
 
//主函数
int main()
{
	
	// glfw: 初始化和配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
 
	 // glfw 窗体生成
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "firstTriangle", NULL, NULL);//设置标题
	//判断窗体是否生成
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
	// glad: 加载所有的OpenGL功能指针
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// 建立并编译着色器
	// ------------------------------------
	Shader myShader("vertexSource.txt", "fragmentSource.txt");
#if 1 
	//设置点数据 (还有缓冲) 配置点的属性（包含点坐标等） 这里设置了4个，将以索引的方式选择点来画三角形
	float vertices[] = {
		//位置            //颜色
		0.0f,  1.0f, 0.0f,1.0f,0.0f,0.0f, 1.0f,1.0f,  //右上
		0.0f, -1.0f, 0.0f,0.0f,1.0f,0.0f,  1.0f,0.0f,//右下
		-1.0f, -1.0f, 0.0f,0.0f,0.0f,1.0f, 0.0f,0.0f,//左下
		-1.0f, 1.0f, 0.0f,1.0f,0.0f,1.0f , 0.0f,1.0f//左上
	};
//矩形
	// float vertices[] = {
	// 	//位置            //颜色
	// 	0.0f,  1.0f, 0.0f,1.0f,0.0f,0.0f,  //右上
	// 	0.0f, -1.0f, 0.0f,0.0f,1.0f,0.0f,  //右下
	// 	-1.0f, -1.0f, 0.0f,0.0f,0.0f,1.0f,//左下
	// 	-1.0f, 1.0f, 0.0f,1.0f,0.0f,1.0f //左上
	// };
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2, // 第一个三角形选择索引为 0 1 3的三个点
	    2, 3, 0  // 第一个三角形选择索引为 1 2 3的三个点
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);  //注意，这里使用EBO作为缓冲对象
							// 绑定顶点数组, 然后绑定并设置缓冲, 最后配置顶点属性.
	glBindVertexArray(VAO);
 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//修改属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//就因为在偏移量上使用的是（void*)3,而不是(void*)(3 * sizeof(float))造成颜色缺失
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
#endif
//	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)6);
//	glEnableVertexAttribArray(2);
#if 1
	//添加纹理
	unsigned int texture1;
	//stbi_set_flip_vertically_on_load(true);//y轴翻转
	//木箱部分
	glGenTextures(1, &texture1);
	//激活
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width,height,nrChannels;
	unsigned char* data = stbi_load("../res/back.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);//生成多级渐远纹理
	}
	else
	{
		std::cout << "加载纹理失败" << std::endl;
	}
	stbi_image_free(data);//释放

	myShader.use();
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
#endif 

#if 0//三角形的
float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
#endif
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		// -----
		processInput(window);
 
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture1);
		// 画第一个三角形
		//glUseProgram(shaderProgram);
		myShader.use();
		glBindVertexArray(VAO); //可以知道我们只有一个三角形VAO，没必要每次都绑定它，但是我们这么做会让代码有一点组织性
								//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES,0,3);
		// glBindVertexArray(0); //没必要每次都解绑 
 
		// 交换buffers和poll的IO事件 (按键按下/释放，鼠标移动等.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
 
	// optional: 一旦他们超出已有的资源，就取消所有资源的分配：
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
 
	// glfw:终止，清空之前所有的GLFW的预分配资源
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
 
// process all input:查询GLFW相关按键是否被按下/释放，根据情况作出反应
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
 
// glfw:无论窗口大小何时改变（由操作系统或用户自己）这个回调函数将会被执行
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//确定viewport与新的窗口尺寸匹配; 请注意，宽度和高度将明显大于显示器上指定的宽度和高度。
	glViewport(0, 0, width, height);
}
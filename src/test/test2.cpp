//纹理单元的使用
#include<iostream>

#include <glew/glew.h>
#include "../RenderSystem/Shader.h"
#include "../RenderSystem/TextureManager.h"
#include "../RenderSystem/Camera.h"
// GLFW
#include <glfw3.h>

#include<FreeImage.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace RenderSystem;
using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions 创造一个GLFWwindow object that can use for GLFW's
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions 设置回调函数
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a 
	//modern approach to retrieving function pointers and extensions 设置为true 用现在的方法恢复函数的指针和扩展
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers  初始化GLEW 去设置opengl的函数指针
	glewInit();

	// Define the viewport dimensions 定义视图尺寸
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


	// Build and compile our shader program    构建和编译我们的渲染程序
	glewInit();
	Shader ourShader("../../../src/Data/shader/test.fs", "../../../src/Data/shader/test.vs");


	// Set up vertex data (and buffer(s)) and attribute pointers  设置顶点数据和指针属性
	GLfloat vertices[] = {
		// Positions   位置       // Colors  颜色         // Texture Coords  纹理坐标
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0! 备注我们开始从0
		0, 1, 3, // First Triangle 第一个三角形
		1, 2, 3  // Second Triangle第二个三角形
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute  位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute  颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute   纹理坐标属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO 解绑VAO


						  // Load and create a texture   加载和创建纹理
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/2.jpg", "texture1");
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/1.png", "texture2");
	
	camera.setPerspectiveFovLHMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// Game loop
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		//如果有事件（键盘和鼠标等等）调用相应的响应函数
		glfwPollEvents();

		// Render 渲染
		// Clear the colorbuffer  清楚颜色缓存
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Bind Textures using texture units  绑定纹理 使用纹理单元

		glActiveTexture(GL_TEXTURE0);
		TextureManager::Inst()->bindTexture("texture1");		
		glActiveTexture(GL_TEXTURE1);
		TextureManager::Inst()->bindTexture("texture2");
		

		// Activate shader 激活渲染

		ourShader.setMat4("projection", camera.getProjectMatrix());

		// camera/view transformation
		glm::mat4 view = camera.getViewMatrix();
		ourShader.setMat4("view", view);
		// Create transformations 创建转变
		glm::mat4 model; //创建一个单位矩阵
		//transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));//移动
		//transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));//旋转

																										// Get matrix's uniform location and set matrix  得到矩阵uniform 位置并且设置矩阵，将我们设置的数据传给着色器
																										//首先请求uniform变量的地址，然后用glUniform函数把矩阵数据发送给着色器，这个函数有一个Matrix4fv的后缀
																										//第一个参数就比较熟悉了，它是uniform的地址，第二个参数告诉Opengl多少个矩阵被发送过去，目前是1
																										//第三个参数询问我们我们是否希望对矩阵进行置换，这样会把矩阵的行转换为列。penGL使用的内部的矩阵布局叫做以列为主顺序（column-major ordering）布局。
																										//GLM已经是用以列为主顺序定义了它的矩阵，所以并不需要置换矩阵，我们可以填为GL_FALSE、最后一个参数是实际的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望的那种，
																										//因此我们要先用GLM的内建函数value_ptr来变换这些数据。
		//GLint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		ourShader.setMat4("model", model);
		// Draw container  画集合
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers 交换屏幕的缓存
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose  适当的的释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.终止GLFW，清除所有的通过GLFW分配的资源
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
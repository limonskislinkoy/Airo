
#define GLEW_STATIC

#ifndef SHADER_H
#define SHADER_H


#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "SFML/Graphics.hpp"
#include <fstream>
#include <sstream>
float WINDOW_H = 1440;
float WINDOW_W = 2560;

class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif


class Camera {
public:
	float pos_x = 0.0;
	float pos_y = 1.0;
	float pos_z = 0.0;


	float dir_x = 0.0;
	float dir_y = 0.0;
	float dir_z = -1.0;

	float roll = 0.f;
	float pitch = 0.f;
	float yaw = 0.f;
	void move_world() {
		
	}
};



class Object {
public:
	int count_polygons = 0;

};

/*
void Load_Scene() {
	std::fstream objfile;
	objfile.open("test1.obj", std::ios::in);
	float obj_vertices[10000 * 6];
	float obj_textures[10000 * 2];
	float obj_normals[10000 * 3];
	int start_pos_for_readobj = 0;
	int start_pos_for_readobj_normals = 0;
	int count_of_vertex = 0;
	std::string line;


	while (std::getline(objfile, line))
	{
		if (line[0] == 'o') {
			//start_pos_for_readobj = 0;
			std::cout << "++++" << "\n";
		}
		//std::cout << line << "\n";
		if (line[0] == 'v' && line[1] == ' ') {
			count_of_vertex++;
			std::cout << "=========" << count_of_vertex << "\n";
			std::stringstream ss(line);
			std::string word;
			ss >> word;
			while (ss >> word) {
				//std::cout << "word:" << word << "\n";
				obj_vertices[start_pos_for_readobj] = std::stof(word);
				start_pos_for_readobj++;
			}
			obj_vertices[start_pos_for_readobj] = 0.3f;
			start_pos_for_readobj++;

			obj_vertices[start_pos_for_readobj] = 0.3f;
			start_pos_for_readobj++;

			obj_vertices[start_pos_for_readobj] = 0.3f;
			start_pos_for_readobj++;


		}

		if (line[0] == 'v' && line[1] == 'n') {
			std::stringstream ss(line);
			std::string word;
			ss >> word;
			while (ss >> word) {
				//std::cout << "word:" << word << "\n";
				obj_normals[start_pos_for_readobj_normals] = std::stof(word);
				start_pos_for_readobj_normals++;
			}
		}



		if (line[0] == 'f') {
			std::string delimiter = " ";

			line.erase(0, 2);
			size_t pos = 0;
			int token;
			int gc = 0;
			while (gc < 3) {
				//std::cout << line.substr(0, pos) << "\n";

				token = std::stoi(line.substr(0, line.find("/")));

				//std::cout << "token: " << token << "\n";
				vertices[start_pos_for_writeobj] = obj_vertices[(token - 1) * 6 + 0];
				vertices[start_pos_for_writeobj + 1] = obj_vertices[(token - 1) * 6 + 1];
				vertices[start_pos_for_writeobj + 2] = obj_vertices[(token - 1) * 6 + 2];

				vertices[start_pos_for_writeobj + 3] = obj_vertices[(token - 1) * 6 + 3];
				vertices[start_pos_for_writeobj + 4] = obj_vertices[(token - 1) * 6 + 4];
				vertices[start_pos_for_writeobj + 5] = obj_vertices[(token - 1) * 6 + 5];


				//std::cout << "old_str: " << line << "\n";
				line.erase(0, line.find("/") + 1);
				line.erase(0, line.find("/") + 1);
				//std::cout << "new_str: " << line << "\n";

				token = std::stoi(line.substr(0, line.find(" ")));
				//std::cout << "token normal: " << token << "\n";

				line.erase(0, line.find(" ") + 1);
				vertices[start_pos_for_writeobj + 6] = obj_normals[(token - 1) * 3 + 0];
				vertices[start_pos_for_writeobj + 7] = obj_normals[(token - 1) * 3 + 1];
				vertices[start_pos_for_writeobj + 8] = obj_normals[(token - 1) * 3 + 2];

				start_pos_for_writeobj += 9;

				gc++;
			}
		}

	}
	objfile.close();
}



*/
/*
float obj_vertices[10000 *6];
		float obj_textures[10000 * 2];
		float obj_normals[10000 * 3];
		int start_pos_for_readobj = 0;
		int start_pos_for_readobj_normals = 0;
		int count_of_vertex = 0;
		std::string line;


		while (std::getline(objfile, line))
		{
			if (line[0] == 'o') {
				//start_pos_for_readobj = 0;
				std::cout << "++++" << "\n";
			}
			//std::cout << line << "\n";
			if (line[0]=='v' && line[1]==' ') {
				count_of_vertex++;
				std::cout <<"========="<< count_of_vertex << "\n";
				std::stringstream ss(line);
				std::string word;
				ss >> word;
				while (ss >> word) {
					//std::cout << "word:" << word << "\n";
					obj_vertices[start_pos_for_readobj] = std::stof(word);
					start_pos_for_readobj++;
				}
				obj_vertices[start_pos_for_readobj] = 0.3f;
				start_pos_for_readobj++;

				obj_vertices[start_pos_for_readobj] = 0.3f;
				start_pos_for_readobj++;

				obj_vertices[start_pos_for_readobj] = 0.3f;
				start_pos_for_readobj++;


			}

			if (line[0] == 'v' && line[1] == 'n') {
				std::stringstream ss(line);
				std::string word;
				ss >> word;
				while (ss >> word) {
					//std::cout << "word:" << word << "\n";
					obj_normals[start_pos_for_readobj_normals] = std::stof(word);
					start_pos_for_readobj_normals++;
				}
			}



			if (line[0] == 'f') {
				std::string delimiter =" ";

				line.erase(0, 2);
				size_t pos = 0;
				int token;
				int gc = 0;
				while (gc<3) {
					//std::cout << line.substr(0, pos) << "\n";

					token = std::stoi(line.substr(0, line.find("/") ));

					//std::cout << "token: " << token << "\n";
					vertices[start_pos_for_writeobj] = obj_vertices[(token-1)*6+0];
					vertices[start_pos_for_writeobj+1] = obj_vertices[(token - 1) * 6 + 1];
					vertices[start_pos_for_writeobj+2] = obj_vertices[(token - 1) * 6 + 2];

					vertices[start_pos_for_writeobj+3] = obj_vertices[(token - 1) * 6 + 3];
					vertices[start_pos_for_writeobj+4] = obj_vertices[(token - 1) * 6 + 4];
					vertices[start_pos_for_writeobj+5] = obj_vertices[(token - 1) * 6 + 5];


					//std::cout << "old_str: " << line << "\n";
					line.erase(0,line.find("/")+1);
					line.erase(0, line.find("/") + 1);
					//std::cout << "new_str: " << line << "\n";

					token = std::stoi(line.substr(0, line.find(" ")));
					//std::cout << "token normal: " << token << "\n";

					line.erase(0, line.find(" ") + 1);
					vertices[start_pos_for_writeobj + 6] = obj_normals[(token - 1) * 3 + 0];
					vertices[start_pos_for_writeobj + 7] = obj_normals[(token - 1) * 3 + 1];
					vertices[start_pos_for_writeobj + 8] = obj_normals[(token - 1) * 3 + 2];

					start_pos_for_writeobj += 9;

					gc++;
				}
			}

		}
		objfile.close();
	}
*/

bool wasd[9] = { 0,0,0,0,  0  ,  0  , 0 ,0,0};
Camera camera;
glm::vec4 dir_camera = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
glm::vec4 dir_camera_up= glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
glm::vec4 dir_camera_right = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		std::cout << "Pos_x:" << camera.pos_x << std::endl;
		std::cout << "Pos_y:" << camera.pos_y << std::endl;
		std::cout << "Pos_z:" << camera.pos_z << std::endl;

		std::cout << "Camera_yaw:" << camera.yaw << std::endl;
		std::cout << "Camera_pitch:" << camera.pitch << std::endl;
		std::cout << "Camera_roll:" << camera.roll << std::endl;

		std::cout << "Dir_camera_x:" << dir_camera.x << std::endl;
		std::cout << " Dir_camera_y:" << dir_camera.y << std::endl;
		std::cout << " Dir_camera_z:" << dir_camera.z << std::endl;

		std::cout << "Dir_camera_up_x:" << dir_camera_up.x << std::endl;
		std::cout << " Dir_camera_up_y:" << dir_camera_up.y << std::endl;
		std::cout << " Dir_camera_up_z:" << dir_camera_up.z << std::endl;

		std::cout << "Dir_camera_right_x:" << dir_camera_right.x << std::endl;
		std::cout << " Dir_camera_right_y:" << dir_camera_right.y << std::endl;
		std::cout << " Dir_camera_right_z:" << dir_camera_right.z << std::endl;
		std::cout << "============" << std::endl;
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
		 camera.pos_x = 0.0;
		 camera.pos_y = 1.0;
		 camera.pos_z = 0.0;


		 camera.dir_x = 0.0;
		 camera.dir_y = 0.0;
		 camera.dir_z = -1.0;

		 camera.roll = 0.f;
		 camera.pitch = 0.f;
		 camera.yaw = 0.f;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) { wasd[0] = true; }
		if (key == GLFW_KEY_A) { wasd[1] = true; }
		if (key == GLFW_KEY_S) { wasd[2] = true; }
		if (key == GLFW_KEY_D) { wasd[3] = true; }

		if (key == GLFW_KEY_SPACE) { wasd[4] = true; }
		if (key == GLFW_KEY_LEFT_SHIFT) { wasd[5] = true; }
		if (key == GLFW_KEY_LEFT_CONTROL) { wasd[6] = true; }

		if (key == GLFW_KEY_Q) { wasd[7] = true; }
		if (key == GLFW_KEY_E) { wasd[8] = true; }
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) { wasd[0] = false; }
		if (key == GLFW_KEY_A) { wasd[1] = false; }
		if (key == GLFW_KEY_S) { wasd[2] = false; }
		if (key == GLFW_KEY_D) { wasd[3] = false; }

		if (key == GLFW_KEY_SPACE) { wasd[4] = false; }
		if (key == GLFW_KEY_LEFT_SHIFT) { wasd[5] = false; }
		if (key == GLFW_KEY_LEFT_CONTROL) { wasd[6] = false; }

		if (key == GLFW_KEY_Q) { wasd[7] = false; }
		if (key == GLFW_KEY_E) { wasd[8] = false; }
	}

	

}

float mouse_prev_x = 0;
float mouse_prev_y = 0;

float delta_x = 0;
float delta_y = 0;
glm::vec3 dir_of_camera;
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	delta_x = (xpos - mouse_prev_x);
	delta_y = (ypos - mouse_prev_y);
	mouse_prev_x = xpos;
	mouse_prev_y = ypos;
	camera.yaw -= delta_x/10.0;
	camera.pitch -= delta_y / 10.0;

	if (camera.yaw > 180.0f) { camera.yaw = -180.0f; }
	if (camera.yaw < -180.0f) { camera.yaw = 180.0f; }


	camera.pitch = std::max(-90.0f, std::min(90.0f, camera.pitch));
	//dir_of_camera = glm::vec3(camera.dir_x, camera.dir_y, camera.dir_z);

	



}
int main()
{

	



	GLfloat vertices[3*3 * 10000] = {
		// Позиции          // Цвета             // Текстурные координаты
		 //1.0f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,      // Верхний правый
		 //1.0f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,      // Нижний правый
		 //1.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,      // Нижний левый


		 // Позиции          // Цвета             // стрелки осей          -x
		 0.0f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 0.0f,1.0f,0.0f,      // Верхний правый
		 999.0f,0.0f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0,1.0,0.0,  // Нижний правый
		 0.0f,  0.001f, 0.0f,   1.0f, 0.0f, 0.0f,  0.0,1.0,0.0,    // Нижний левый

		 // Позиции          // Цвета             // стрелки осей          -y
		 0.0f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0,0.0,0.0,     // Верхний правый
		 0.0f,  999.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0,0.0,0.0,    // Нижний правый
		 0.001f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0,0.0,0.0,    // Нижний левый

		 // Позиции          // Цвета             // стрелки осей          -z
		 0.0f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0,1.0,0.0,    // Верхний правый
		 0.0f,  0.0f, 999.0f,   0.0f, 0.0f, 1.0f, 0.0,1.0,0.0,     // Нижний правый
		 0.001f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0,1.0,0.0  // Нижний левый

	};

	int start_pos_for_writeobj = 9 *3*3;

	//																													READ FROM .OBJ FILE
	std::fstream objfile;
	objfile.open("test1.obj", std::ios::in);
	

	float obj_vertices[10000 * 6];
	float obj_textures[10000 * 2];
	float obj_normals[10000 * 3];
	int start_pos_for_readobj = 0;
	int start_pos_for_readobj_normals = 0;
	int count_of_vertex = 0;
	std::string line;


	while (std::getline(objfile, line))
	{
		if (line[0] == 'o') {
			//start_pos_for_readobj = 0;
			std::cout << "++++" << "\n";
		}
		//std::cout << line << "\n";
		if (line[0] == 'v' && line[1] == ' ') {
			count_of_vertex++;
			std::cout << "=========" << count_of_vertex << "\n";
			std::stringstream ss(line);
			std::string word;
			ss >> word;
			while (ss >> word) {
				//std::cout << "word:" << word << "\n";
				obj_vertices[start_pos_for_readobj] = std::stof(word);
				start_pos_for_readobj++;
			}
			obj_vertices[start_pos_for_readobj] = 0.3f;
			start_pos_for_readobj++;

			obj_vertices[start_pos_for_readobj] = 0.3f;
			start_pos_for_readobj++;

			obj_vertices[start_pos_for_readobj] = 0.3f;
			start_pos_for_readobj++;


		}

		if (line[0] == 'v' && line[1] == 'n') {
			std::stringstream ss(line);
			std::string word;
			ss >> word;
			while (ss >> word) {
				//std::cout << "word:" << word << "\n";
				obj_normals[start_pos_for_readobj_normals] = std::stof(word);
				start_pos_for_readobj_normals++;
			}
		}



		if (line[0] == 'f') {
			std::string delimiter = " ";

			line.erase(0, 2);
			size_t pos = 0;
			int token;
			int gc = 0;
			while (gc < 3) {
				//std::cout << line.substr(0, pos) << "\n";

				token = std::stoi(line.substr(0, line.find("/")));

				//std::cout << "token: " << token << "\n";
				vertices[start_pos_for_writeobj] = obj_vertices[(token - 1) * 6 + 0];
				vertices[start_pos_for_writeobj + 1] = obj_vertices[(token - 1) * 6 + 1];
				vertices[start_pos_for_writeobj + 2] = obj_vertices[(token - 1) * 6 + 2];

				vertices[start_pos_for_writeobj + 3] = obj_vertices[(token - 1) * 6 + 3];
				vertices[start_pos_for_writeobj + 4] = obj_vertices[(token - 1) * 6 + 4];
				vertices[start_pos_for_writeobj + 5] = obj_vertices[(token - 1) * 6 + 5];


				//std::cout << "old_str: " << line << "\n";
				line.erase(0, line.find("/") + 1);
				line.erase(0, line.find("/") + 1);
				//std::cout << "new_str: " << line << "\n";

				token = std::stoi(line.substr(0, line.find(" ")));
				//std::cout << "token normal: " << token << "\n";

				line.erase(0, line.find(" ") + 1);
				vertices[start_pos_for_writeobj + 6] = obj_normals[(token - 1) * 3 + 0];
				vertices[start_pos_for_writeobj + 7] = obj_normals[(token - 1) * 3 + 1];
				vertices[start_pos_for_writeobj + 8] = obj_normals[(token - 1) * 3 + 2];

				start_pos_for_writeobj += 9;

				gc++;
			}
		}

	}
	objfile.close();

	//																													///////////////////

	//Инициализация GLFW
	glfwInit();
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);



	GLFWwindow* window = glfwCreateWindow(2560, 1440, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	
	

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	// Position attribute
	
	
	// TexCoord attribute
		


	/*unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps


	int widthtex, heighttex, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &widthtex, &heighttex, &nrChannels, 0);
	

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthtex, heighttex, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}


	stbi_image_free(data);
	
	
	*/
	float gc_time = 0;
	glEnable(GL_DEPTH_TEST);

	

	Shader ourShader("vertex.glsl", "fragment.glsl");
	
	ourShader.use();
	
	glm::vec3 Light_pos = glm::vec3(0.0, 3.0, 0.0);
	glm::vec3 Light_color = glm::vec3(1.0, 1.0, 1.0);

	
	
	glm::mat4 transmat;



	
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		gc_time += 0.001f;
		
		ourShader.setFloat("gc_time", gc_time);

		Light_pos = glm::vec3(camera.pos_x, camera.pos_y, camera.pos_z);
		//######################################################################################################################################################################################

		dir_camera = glm::vec4(0.0f,0.0f,-1.0f,0.0f);
		
		transmat = glm::mat4(1.0f);
		transmat = glm::rotate(transmat, glm::radians(camera.pitch), glm::vec3(-1.0, 0.0, 0.0));
		dir_camera = dir_camera * transmat;
		transmat = glm::mat4(1.0f);																						// I dont know WTF is happening, but y axis is reversed
		transmat = glm::rotate(transmat, glm::radians(camera.yaw), glm::vec3(0.0, -1.0, 0.0));
		dir_camera = dir_camera * transmat;
	
		dir_camera_up = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

		transmat = glm::mat4(1.0f);
		transmat = glm::rotate(transmat, glm::radians(camera.pitch+90.0f), glm::vec3(-1.0, 0.0, 0.0));
		dir_camera_up = dir_camera_up * transmat;
		transmat = glm::mat4(1.0f);																						// Repeat for calculate up-vector
		transmat = glm::rotate(transmat, glm::radians(camera.yaw), glm::vec3(0.0, -1.0, 0.0));
		dir_camera_up = dir_camera_up * transmat;


		dir_camera_right = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

		transmat = glm::mat4(1.0f);
		transmat = glm::rotate(transmat, glm::radians(camera.pitch), glm::vec3(-1.0, 0.0, 0.0));
		dir_camera_right = dir_camera_right * transmat;
		transmat = glm::mat4(1.0f);																						// Repeat for calculate right-vector
		transmat = glm::rotate(transmat, glm::radians(camera.yaw - 90.0f), glm::vec3(0.0, -1.0, 0.0));
		dir_camera_right = dir_camera_right * transmat;
		
		//######################################################################################################################################################################################


		if (wasd[0] && !wasd[2]) {
			
			camera.pos_x += dir_camera.x/100.0f;
			camera.pos_y += dir_camera.y / 100.0f;
			camera.pos_z += dir_camera.z / 100.0f;
			//std::cout << "forward" << camera.pos_x<< std::endl;
		}

		if (wasd[1] && !wasd[3]) {
			camera.pos_x -= dir_camera_right.x / 100.0f;
			camera.pos_y -= dir_camera_right.y / 100.0f;
			camera.pos_z -= dir_camera_right.z / 100.0f;
		}

		if (wasd[2] && !wasd[0]) {
			camera.pos_x -= dir_camera.x / 100.0f;
			camera.pos_y -= dir_camera.y / 100.0f;
			camera.pos_z -= dir_camera.z / 100.0f;
		}

		if (wasd[3] && !wasd[1]) {

			camera.pos_x += dir_camera_right.x / 100.0f;
			camera.pos_y += dir_camera_right.y / 100.0f;
			camera.pos_z += dir_camera_right.z / 100.0f;
		}

		if (wasd[6] && !wasd[4]) {
			camera.pos_x -= dir_camera_up.x / 100.0f;
			camera.pos_y -= dir_camera_up.y / 100.0f;
			camera.pos_z -= dir_camera_up.z / 100.0f;

		}

		if (wasd[4] && !wasd[6]) {
			camera.pos_x += dir_camera_up.x / 100.0f;
			camera.pos_y += dir_camera_up.y / 100.0f;
			camera.pos_z += dir_camera_up.z / 100.0f;

		}


		if (wasd[7] && !wasd[8]) {
			camera.roll += 0.01f;
		}

		if (wasd[8] && !wasd[7]) {
			camera.roll -= 0.01f;
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);




		GLuint transformLoc = glGetUniformLocation(ourShader.ID, "light_pos");
		glUniform3f(transformLoc, Light_pos.x, Light_pos.y, Light_pos.z);

		transformLoc = glGetUniformLocation(ourShader.ID, "light_color");
		glUniform3f(transformLoc, Light_color.x, Light_color.y, Light_color.z);

		transformLoc = glGetUniformLocation(ourShader.ID, "view_pos");
		glUniform3f(transformLoc, camera.pos_x, camera.pos_y, camera.pos_z);



		glm::mat4 proj = glm::perspective(70.0f, (float)width / (float)height,0.01f,100.0f);
		transformLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(proj));



		transmat = glm::mat4(1.0f);

		transmat = glm::rotate(transmat, glm::radians(-camera.yaw),glm::vec3(0.0f,1.0f,0.0f));
		transformLoc = glGetUniformLocation(ourShader.ID, "transmat_rotate_yaw");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transmat));



		transmat = glm::mat4(1.0f);

		transmat = glm::rotate(transmat, glm::radians(-camera.pitch) , glm::vec3(1.0f, 0.0f, 0.0f));
		transformLoc = glGetUniformLocation(ourShader.ID, "transmat_rotate_pitch");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transmat));



		transmat = glm::mat4(1.0f);

		transmat = glm::rotate(transmat, glm::radians(-camera.roll) , glm::vec3(dir_camera));
		transformLoc = glGetUniformLocation(ourShader.ID, "transmat_rotate_roll");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transmat));




		glm::vec3 transform_offset = glm::vec3(camera.pos_x, camera.pos_y, camera.pos_z);

		GLuint transform_offset_Loc = glGetUniformLocation(ourShader.ID, "transmat_offset");
		glUniform3f(transform_offset_Loc, transform_offset.x, transform_offset.y, transform_offset.z);
		//std::cout << gc_time<<std::endl;

		glDrawArrays(GL_TRIANGLES, 0, 3*10000);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
	}

	return 0;
}
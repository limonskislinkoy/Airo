#include "Header.h"

#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "SFML/Graphics.hpp"

#include <string>
#include <sstream>

//Global Variables
GLuint textures[MAX_OBJECTS];
int Current_number_of_objects=0;


//===Global Variables

glm::mat4 Create_Rotation_Matrix_by_Euler(float pitch,float yaw,float roll) {
	glm::mat4 result=glm::mat4(1.0);

	glm::vec4 direction = glm::vec4(0.0, 0.0, -1.0, 0.0);               //   -z  forward
	glm::vec4 up_direction = glm::vec4(0.0, 1.0, 0.0, 0.0);			 //   +y  up
	glm::vec4 right_direction = glm::vec4(1.0, 0.0, 0.0, 0.0);		 //   +x  right

	glm::mat4 trans = glm::mat4(1.0);

	glm::mat4 yaw_mat = glm::rotate(trans, glm::radians(yaw), glm::vec3(up_direction.x, up_direction.y, up_direction.z));
	direction = direction * yaw_mat;
	right_direction = right_direction * yaw_mat;


	glm::mat4 pitch_mat = glm::rotate(trans, glm::radians(pitch), glm::vec3(right_direction.x, right_direction.y, right_direction.z));
	direction = direction * pitch_mat;
	up_direction = up_direction * pitch_mat;

	return	yaw_mat * pitch_mat;
}


class Camera
{
public:
	glm::vec3 position = glm::vec3(7.0, 6.0, 4.0);
	glm::vec4 direction = glm::vec4(0.0, 0.0, -1.0, 0.0);               //   -z  forward
	glm::vec4 up_direction = glm::vec4(0.0, 1.0, 0.0, 0.0);			 //   +y  up
	glm::vec4 right_direction = glm::vec4(1.0, 0.0, 0.0, 0.0);		 //   +x  right
	float roll = 0.f;
	float pitch = -30.f;
	float yaw = 45.f;

	float fov = 90.f;

	glm::mat4 roll_mat;
	glm::mat4 pitch_mat;
	glm::mat4 yaw_mat;

	float speed = 0.1;


	//int look_at();
	void create_tranforming_matrixes();
};

void Camera::create_tranforming_matrixes() {
	direction = glm::vec4(0.0, 0.0, -1.0, 0.0);               //   -z  forward
	up_direction = glm::vec4(0.0, 1.0, 0.0, 0.0);			 //   +y  up
	right_direction = glm::vec4(1.0, 0.0, 0.0, 0.0);		 //   +x  right

	glm::mat4 trans = glm::mat4(1.0);
	
	yaw_mat = glm::rotate(trans, glm::radians(-yaw), glm::vec3(up_direction.x, up_direction.y, up_direction.z));
	direction = direction* yaw_mat;
	right_direction = right_direction* yaw_mat;

	pitch_mat = glm::rotate(trans, glm::radians(-pitch), glm::vec3(right_direction.x, right_direction.y, right_direction.z));
	direction = direction* pitch_mat;
	up_direction = up_direction* pitch_mat;

	//roll_mat = glm::rotate(trans, glm::radians(roll), glm::vec3(direction.x, direction.y, direction.z));
	//direction= direction* roll_mat;
	//right_direction = right_direction* roll_mat;

}

//Control variables
bool wasd[9] = { 0,0,0,0,  0  ,  0  , 0 ,0,0 };
bool restart_pos_of_camera=false;
double delta_x, delta_y, mouse_prev_x = -1, mouse_prev_y = -1;
bool out_debug_information=false;
//====Control variables

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		out_debug_information = true;
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
		restart_pos_of_camera = true;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); }

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
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	delta_x = (xpos - mouse_prev_x);
	delta_y = (ypos - mouse_prev_y);
	if (mouse_prev_x == -1) { delta_x = 0; delta_y = 0; }
	mouse_prev_x = xpos;
	mouse_prev_y = ypos;


}




class Object
{
public:
	std::string name;
	glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);

	glm::vec3  ambient;
	glm::vec3  diffuse;
	glm::vec3  specular;

	std::string path_to_texture;




	float* resulting_buff = new float[3000];
	int id_last_resulting_buff = 0;




	int load_object(std::string path);
	int load_scene(std::string path);

private:
	int load_material(std::string path);
};

int Object::load_material(std::string path) {

	std::fstream file(path);
	std::string s;
	std::string word;

	bool key_to_stop = false;
	while (true) {
		
		std::getline(file, s);
		std::stringstream ss(s);
		ss >> word;
		if (word == "Ka") {
			ss >> word; ambient.x = std::atof(word.c_str());
			ss >> word; ambient.y = std::atof(word.c_str());
			ss >> word; ambient.z = std::atof(word.c_str());
		}

		if (word == "Kd") {
			ss >> word; diffuse.x = std::atof(word.c_str());
			ss >> word; diffuse.y = std::atof(word.c_str());
			ss >> word; diffuse.z = std::atof(word.c_str());
		}

		if (word == "Ks") {
			ss >> word; specular.x = std::atof(word.c_str());
			ss >> word; specular.y = std::atof(word.c_str());
			ss >> word; specular.z = std::atof(word.c_str());
		}

		if (word == "map_Kd") { ss >> path_to_texture; key_to_stop = true; }
		if (word == "illum") { key_to_stop = true; }

		if (s.length() == 0 && key_to_stop==true) { break; }


	}

	// LOAD TEXTURE
	glBindTexture(GL_TEXTURE_2D, textures[Current_number_of_objects]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // maybe comment?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // maybe comment?
	int width, height, nrChannels;
	
	
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);


	return 0;

}


int Object::load_object(std::string path) {

	float* mas_vertexes = new float[10000];
	int id_last_vertex = 0;
	float* mas_textures = new float[10000];
	int id_last_texture = 0;
	float* mas_normals = new float[10000];
	int id_last_normal = 0;

	std::fstream file(path);
	std::string s;
	std::string word;
	std::string delimiter = "/";
	int token;
	while (true) {
		std::getline(file, s);
		std::stringstream ss(s);
		ss >> word;

		if (word == "#") { continue; }

		if (word == "mtllib") { ss >> word; load_material(word); }

		if (word == "o") { ss >> name; }

		if (word == "v") {
			for (int i = 0; i < 3; i++) {
				ss >> word;
				mas_vertexes[id_last_vertex] = std::atof(word.c_str());
				id_last_vertex++;
			}
		}
		if (word == "vt") {
			for (int i = 0; i < 2; i++) {
				ss >> word;
				mas_textures[id_last_texture] = std::atof(word.c_str());
				id_last_texture++;
			}
		}
		if (word == "vn") {
			for (int i = 0; i < 3; i++) {
				ss >> word;
				mas_normals[id_last_normal] = std::atof(word.c_str());
				id_last_normal++;
			}
		}


		if (word == "f") {
			size_t pos = 0;
			int token;
			int gc = 0;
			while (gc < 3) {
				ss >> word;
				token = std::stoi(word.substr(0, word.find("/")));
				resulting_buff[id_last_resulting_buff] = mas_vertexes[(token - 1)*3 + 0];
				id_last_resulting_buff++;
				resulting_buff[id_last_resulting_buff] = mas_vertexes[(token - 1) * 3 + 1];
				id_last_resulting_buff++;
				resulting_buff[id_last_resulting_buff] = mas_vertexes[(token - 1) * 3 + 2];
				id_last_resulting_buff++;
				
				word.erase(0, word.find("/") + 1);
				token = std::stoi(word.substr(0, word.find(" ")));
				resulting_buff[id_last_resulting_buff] = mas_textures[(token - 1) * 2 + 0];
				id_last_resulting_buff++;
				resulting_buff[id_last_resulting_buff] = mas_textures[(token - 1) * 2 + 1];
				id_last_resulting_buff++;
				

				word.erase(0, word.find("/") + 1);
				token = std::stoi(word.substr(0, word.find(" ")));
				resulting_buff[id_last_resulting_buff] = diffuse.x;
				id_last_resulting_buff++;
				resulting_buff[id_last_resulting_buff] = diffuse.y;
				id_last_resulting_buff++;
				resulting_buff[id_last_resulting_buff] = diffuse.z;
				id_last_resulting_buff++;
				gc++;
			}
		}

		if (s.length() == 0) { break; }
	}

	delete[] mas_normals;
	delete[] mas_textures;
	delete[] mas_vertexes;

	Current_number_of_objects++;
	return 0;


}


int Object::load_scene(std::string path) {}

class Engine
{
public:
	GLFWwindow* window;
	Object** mas_objects = new Object*[MAX_OBJECTS];
	unsigned int VBO, VAO;
	Camera* view_camera;
	int ID_SHADER=0;
	bool set_perspective_of_camera=true;

	double time=0.0;


	int start();
	void Move_event();
	void render();
	void Tick();
	void Event_checker();

};

int Engine::start() {
	
	for (int i = 0; i < MAX_OBJECTS; i++) {
		mas_objects[i] = nullptr;
	}


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Airo", nullptr, nullptr);
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

	glEnable(GL_DEPTH_TEST);

	glGenTextures(MAX_OBJECTS, textures);
	

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
	Shader ourShader("vertex.glsl", "fragment.glsl");
	ourShader.use();
	ID_SHADER = ourShader.ID;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	
	//glm::mat4 proj = glm::perspective(70.f, (float)width / (float)height, 0.1f, 100.0f);
	//GLuint transformLoc = glGetUniformLocation(ourShader.ID, "projection");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(proj));

	return 0;

}

void Engine::Move_event() {
	view_camera->pitch -= delta_y / 10;
	delta_y = 0;
	view_camera->yaw -= delta_x / 10;
	delta_x = 0;
	if (wasd[0] && !wasd[2]) {

		view_camera->position += glm::vec3(view_camera->direction.x, view_camera->direction.y, view_camera->direction.z) * view_camera->speed;
		//std::cout << "forward" << camera.pos_x<< std::endl;
	}

	if (wasd[1] && !wasd[3]) {
		view_camera->position -= glm::vec3(view_camera->right_direction.x, view_camera->right_direction.y, view_camera->right_direction.z) * view_camera->speed;
		
	}

	if (wasd[2] && !wasd[0]) {
		view_camera->position -= glm::vec3(view_camera->direction.x, view_camera->direction.y, view_camera->direction.z) * view_camera->speed;
		
	}

	if (wasd[3] && !wasd[1]) {

		view_camera->position += glm::vec3(view_camera->right_direction.x, view_camera->right_direction.y, view_camera->right_direction.z) * view_camera->speed;
	}

	if (wasd[6] && !wasd[4]) {
		view_camera->position -= glm::vec3(view_camera->up_direction.x, view_camera->up_direction.y, view_camera->up_direction.z) * view_camera->speed;

	}

	if (wasd[4] && !wasd[6]) {
		view_camera->position += glm::vec3(view_camera->up_direction.x, view_camera->up_direction.y, view_camera->up_direction.z) * view_camera->speed;

	}


	if (wasd[7] && !wasd[8]) {
		view_camera->roll += 0.01f;
	}

	if (wasd[8] && !wasd[7]) {
		view_camera->roll -= 0.01f;
	}
}

void Engine::render() {

	float gc_time = 0;
	int gc_circles = 0;


	if (glfwWindowShouldClose(window)) {
		glfwTerminate();
		return;
	}

	//while (!glfwWindowShouldClose(window))
	//{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gc_time += 0.001f;
		gc_circles += 1;
		glUniform1f(glGetUniformLocation(ID_SHADER, "gc_time"), gc_time);
		
		if (out_debug_information) {
			std::cout << "roll:" << view_camera->roll << "\n";
			std::cout << "pitch:" << view_camera->pitch << "\n";
			std::cout << "yaw:" << view_camera->yaw << "\n\n";

			std::cout << "coord x: " << view_camera->position.x << "\n";
			std::cout << "coord y: " << view_camera->position.y << "\n";
			std::cout << "coord z: " << view_camera->position.z << "\n\n";

			std::cout << "direction x: " << view_camera->direction.x << "\n";
			std::cout << "direction y: " << view_camera->direction.y << "\n";
			std::cout << "direction z: " << view_camera->direction.z << "\n--------------------------\n\n";


			out_debug_information = false;
		
		}

		view_camera->create_tranforming_matrixes();


		GLuint transformLoc = glGetUniformLocation(ID_SHADER, "transmat_yaw");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view_camera->yaw_mat));
		transformLoc = glGetUniformLocation(ID_SHADER, "transmat_pitch");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view_camera->pitch_mat));

		transformLoc = glGetUniformLocation(ID_SHADER, "camera_pos");
		glUniform3f(transformLoc, view_camera->position.x, view_camera->position.y, view_camera->position.z);




		int id = 0;
		while (true)
		{
			if (mas_objects[id] != nullptr) {

				transformLoc = glGetUniformLocation(ID_SHADER, "obj_position");
				glUniform3f(transformLoc, mas_objects[id]->position.x, mas_objects[id]->position.y,mas_objects[id]->position.z);
				
				transformLoc = glGetUniformLocation(ID_SHADER, "transmat_rotarion_of_object");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Create_Rotation_Matrix_by_Euler(mas_objects[id]->rotation.x, mas_objects[id]->rotation.y, mas_objects[id]->rotation.z)));


				glBindTexture(GL_TEXTURE_2D, textures[id]);

				glBufferData(GL_ARRAY_BUFFER, (mas_objects[id]->id_last_resulting_buff) *sizeof(GLfloat), mas_objects[id]->resulting_buff, GL_STREAM_DRAW);
				//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
				glDrawArrays(GL_TRIANGLES, 0, mas_objects[id]->id_last_resulting_buff/8 - 0);
			}
			else
			{
				break;
			}
			id += 1;
		}

		//std::cout << "11";


		glfwSwapBuffers(window);
		
		//}
	//return 0;
}

void Engine::Event_checker() {
	if (restart_pos_of_camera) {
		view_camera->position = glm::vec3(-2.0, 2.0, 2.0);
		view_camera->direction = glm::vec4(0.0, 0.0, -1.0, 0.0);               //   -z  forward
		view_camera->up_direction = glm::vec4(0.0, 1.0, 0.0, 0.0);			 //   +y  up
		view_camera->right_direction = glm::vec4(1.0, 0.0, 0.0, 0.0);		 //   +x  right
		view_camera->roll = 0.f;
		view_camera->pitch = -45.f;
		view_camera->yaw = 0.f;

		restart_pos_of_camera = false;
	}

	if (set_perspective_of_camera) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glm::mat4 proj = glm::perspective(glm::radians(view_camera->fov), (float)width / (float)height, 0.1f, 100.0f);
		GLuint transformLoc = glGetUniformLocation(ID_SHADER, "projection");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(proj));
		
		set_perspective_of_camera=false;
	}
}

void Engine::Tick() {
	time += 1;
	Move_event();
	Event_checker();
}



#include "funcs.cpp"
#include <iostream>
int main() {
	Engine engine;
	engine.start();

	Camera camera;
	engine.view_camera = &camera;

	Object object1;
	object1.load_object("testcubetex.obj");
	engine.mas_objects[0] = &object1;

	Object AsixX;
	AsixX.load_object("AsixX.obj");
	engine.mas_objects[1] = &AsixX;

	Object AsixZ;
	AsixZ.load_object("AsixZ.obj");
	engine.mas_objects[2] = &AsixZ;

	Object AsixY;
	AsixY.load_object("AsixY.obj");
	engine.mas_objects[3] = &AsixY;

	Object Floor;
	Floor.load_object("Floor.obj");
	engine.mas_objects[4] = &Floor;






	std::cout << "Loading object --- OK\n";
	while (true)
	{

		object1.position=glm::vec3(8.0+sin(engine.time/100)*6,5.0,-8.0+cos(engine.time/100)*6);
		object1.rotation += glm::vec3(0.1, 0.1, 0.0);
		engine.Tick();
		engine.render();
	}
	return 0;
}
// to compile in termianl:
// g++ main.cpp display.cpp mesh.cpp shader.cpp texture.cpp stb_image.c Celegans.c brain.c -lGL -lSDL2 -lGLEW -o celegans
#include <iostream>
#include "display.h"
#include <GL/glew.h>
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./libCelegans/main.h"

//sprite scale factor
float scaleFactor = 0.05f;

int main()
{
	Celegans* ce = Celegans_init();

	Display display(600,600,"hello Celegans!");

	// a rectangle using squared texture
	Vertex vertices[] = { 	Vertex(glm::vec3(-0.5, -0.5, 0),glm::vec2(1.0,1.0)), 
				Vertex(glm::vec3(-0.5,  0.5, 0),glm::vec2(1.0,0.0)),
				Vertex(glm::vec3( 0.5,  0.5, 0),glm::vec2(0.0,0.0)),
				Vertex(glm::vec3( 0.5,  0.5, 0),glm::vec2(0.0,0.0)), 
				Vertex(glm::vec3( 0.5, -0.5, 0),glm::vec2(0.0,1.0)),
				Vertex(glm::vec3(-0.5, -0.5, 0),glm::vec2(1.0,1.0)),};

	Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]));

	Shader shader("./res/basicShader");
	Texture texture("./res/texture.jpg");
	Transform transform;

	SDL_Event event;
	while(!display.IsClosed())
	{
		if(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_a:
					Celegans_brain_touchanterior(ce->soul);
					break;
				case SDLK_f:
					Celegans_brain_givefood(ce->soul);
					break;
				case SDLK_n:
					Celegans_brain_touchnose(ce->soul);
					break;
				case SDLK_p:
					Celegans_brain_touchposterior(ce->soul);
					break;
				case SDLK_ESCAPE:
					//
					break;
				default:
					break;
				}

				break;
			default:
				break;
			}
		}
		
		// calculate movement in main.h
		celegans_loop(ce);

		display.Clear(1.0f,1.0f,1.0f,1.0f);

		transform.GetPos().x = cele_getx(ce);//sinf(counter);
		transform.GetPos().y = cele_gety(ce);
		transform.GetRot().z = ce->body.rotation * 3.141592654f / 180.f;
		
		transform.SetScale(glm::vec3(scaleFactor,scaleFactor,scaleFactor));
 
		mesh.Draw();

		shader.Bind();
		shader.Update(transform);
		texture.Bind(0);
		display.Update();

	usleep(100000);
	}
	//Free brain
	Celegans_free(ce);
	return 0;
}

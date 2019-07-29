#include "Celegans.h"
#include "brain.h"
#include <unistd.h>

////////////////////////////////////////////////////////////
///	CONSTANTS
////////////////////////////////////////////////////////////
#define WINDOW_WIDTH 1100
#define WINDOW_HEIGHT 700
#define BODY_WIDTH 64
#define BODY_HEIGHT 32
#define RADIAS_TO_DEGREE 57.2957

#ifdef __cplusplus
extern "C" {
#endif
struct vec2d{
	float x;
	float y;
};
struct celebody
{
	vec2d position;
	vec2d size;
	vec2d origin;
	float rotation;
};
struct Celegans
{
	brain* soul;
	celebody body;
};

Celegans* Celegans_init()
{
	Celegans* ce = (Celegans*)malloc(sizeof(Celegans));
	ce->soul = Celegans_brain_init();
	ce->body.origin = {BODY_WIDTH/2, BODY_HEIGHT/2};
	ce->body.position = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
	ce->body.size = {BODY_WIDTH, BODY_HEIGHT};
	ce->body.rotation = 0;
	return ce;
}
void Celegans_free(Celegans* ce)
{
	Celegans_brain_free(ce->soul);
	free(ce);
}

void Celegans_update(Celegans* ce)
{
	ce->soul->neurons[MVULVA]->state = 0;
	
	//Calculate right side muscle contraction
	int rightAcc = 0;
	for (int i = MUSCLE_RIGHT_START; i < MUSCLE_RIGHT_END; i++) {
		rightAcc += ce->soul->neurons[i]->state; 
		ce->soul->neurons[i]->state *= 0.7; //Decrease the muscle contraction over time 
	}

	//Calculate left side muscle contraction
	int leftAcc = 0;
	for (int i = MUSCLE_LEFT_START; i < MUSCLE_LEFT_END; i++) {
		leftAcc += ce->soul->neurons[i]->state;
		ce->soul->neurons[i]->state *= 0.7; //Decrease the muscle contraction over time
	}
		
	//Calculate speed based on muscle contraction
	float speed = leftAcc + rightAcc;
	speed = speed/200.0 * 3.0;

	//Adjust orientation and position based on muscle contraction
	if (rightAcc > leftAcc) ce->body.rotation -= 1;
	else if (leftAcc > rightAcc) ce->body.rotation += 1;
	float changex = cosf(ce->body.rotation / RADIAS_TO_DEGREE) * speed;
	float changey = sinf(ce->body.rotation / RADIAS_TO_DEGREE) * speed;
	ce->body.position.x += changex;
	ce->body.position.y += changey;

	//Update brain
	Celegans_brain_update(ce->soul);
}
float cele_getx(Celegans* ce){
	return (ce->body.position.x - WINDOW_WIDTH/2)/WINDOW_WIDTH;
}
float cele_gety(Celegans* ce){
	return (ce->body.position.y - WINDOW_HEIGHT/2)/WINDOW_HEIGHT;
}
void celegans_loop(Celegans* ce)
{


		////////////////////////////////////////////////////////////
		///	UPDATE
		////////////////////////////////////////////////////////////

		//Update each neuron of the brain
		Celegans_update(ce);

		//Check window limits for collision detection
		int collision = 0;
		if (ce->body.position.x <= 0) {ce->body.position.x = 0; collision = 1;}
		if (ce->body.position.x >= WINDOW_WIDTH-BODY_HEIGHT) {ce->body.position.x = WINDOW_WIDTH-BODY_HEIGHT; collision = 1;}
		if (ce->body.position.y <= 0) {ce->body.position.y = 0; collision = 1;}
		if (ce->body.position.y >= WINDOW_HEIGHT-BODY_HEIGHT) {ce->body.position.y = WINDOW_HEIGHT-BODY_HEIGHT; collision = 1;}
		if (collision) Celegans_brain_touchnose(ce->soul); //Trigget nose neurons
		
		//Some manual controlls
		//if (vsKeyGetState(VS_KEY_F)) Celegans_brain_givefood(ce->soul);			//F = give food
		//if (vsKeyGetState(VS_KEY_N)) Celegans_brain_touchnose(ce->soul);		//N = touch nose
		//if (vsKeyGetState(VS_KEY_A)) Celegans_brain_touchanterior(ce->soul);	//A = touch anterior
		//if (vsKeyGetState(VS_KEY_P)) Celegans_brain_touchposterior(ce->soul);	//P = touch posterior

		

}

#ifdef __cplusplus
} //EXTERN C
#endif

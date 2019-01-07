#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>

/* OBRADA GRESAKA */
#include "ast.h"

/* OPEN_GL */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* GL_MATH */
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/transform2.hpp"

/* HELPERS */
#include "DataContainer.h"
#include "Config.h"

/* OBJECT */
#include "Object.h"
#include "DrawableObject.h"
#include "TexturedObject.h"
#include "MovableObject.h"
#include "User.h"
#include "math_objects.h"
#include "various_objects.h"
#include "objloader.h"
#include "Model.h"

#include "Texture.h"
#include "Material.h"
#include "Light.h"

/* CAMERA */
#include "Camera.h"

using namespace std;

/* Ovde se nalaze svi podaci koji
 * se dele izmedju funkcija
 * (umesto globalnih promenljivih) */
DataContainer globalData;

/* CALLBACK-ovi */
static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char c, int x, int y);
static void on_keyboard_release(unsigned char c, int x, int y);
static void on_mouse_move(int x, int y);

/* TAJMERI */
static void redisplay_timer(int value);
static void keyboard_timer(int value);
static void mouse_timer(int value);
static void gravity_timer(int value);
static void animation_timer(int value);
static void simulation_timer(int value);

/* Pomocne funkcije */
static void pauseTimers();
static void unpauseTimers();
// static void fx_changedir(std::string newDir);

int main(int argc, char * argv[])
{
	/* Argumenti - inicijalna provera */
	//string usage = "Usage: " + (string) argv[0] + " dirPath";
	//ast(argc == 2, usage.c_str());

	globalData.configs = Config::importAll("configs", "DEFAULT");
	Config & appConfig = globalData.configs["application"];

	glutInit(&argc, argv);
	int dflWidth = std::stoi(appConfig.getParameter("WIN_WIDTH"));
	int dflHeight = std::stoi(appConfig.getParameter("WIN_HEIGHT"));

	glutInitWindowSize(dflWidth, dflHeight);
	glutInitWindowPosition(0, 0);

	const char * title = appConfig.getParameter("TITLE").c_str();
	glutCreateWindow(title);
	glutFullScreen(); 

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glEnable(GL_DEPTH_TEST);

	float clearR = stof(appConfig.getParameter("CLEAR_COLOR_R"));
	float clearG = stof(appConfig.getParameter("CLEAR_COLOR_G"));
	float clearB = stof(appConfig.getParameter("CLEAR_COLOR_B"));
	glClearColor(clearR, clearG, clearB, 1);

	/* Callback-ovi */
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_release);
	glutMotionFunc(on_mouse_move);
	glutPassiveMotionFunc(on_mouse_move);
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Tajmeri */
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);
	glutTimerFunc(globalData.simulationTimerInterval, simulation_timer, globalData.simulationTimerId);

	/* Normalizacija normala */
	glEnable(GL_NORMALIZE);

	/* Uklucivanje osvetljenja */
	glEnable(GL_LIGHTING);

	/* Ukljucivanje tekstura */
	glEnable(GL_TEXTURE_2D);

	/* Ukljucivanje providnosti objekata */
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	/* Importovanje textura/modela/materijala i svetla */
	globalData.textures = Texture2D::importAll("resources/textures");
	globalData.models = Model::importAll("resources/models");
	globalData.materials = Material::importAll("resources/materials");
	globalData.lights = Light::importAll("resources/lights");

	
	User user;
	Texture2D skyTex = globalData.textures["sky"];
	skyTex.setRepeatingS(false);
	skyTex.setRepeatingT(false);

	TexturedObject sky(NULL, glm::vec4(0.3,0.3,0.3,1.0), globalData.models["cube"], globalData.materials["light"], skyTex);
	sky.scale(glm::vec3(10000.0f,10000.0f,10000.0f));
	globalData.objectsToDisplay.push_back(&sky);

	TexturedObject floor(NULL, glm::vec4(0.3,0.3,0.3,1.0), globalData.models["cube"], globalData.materials["normal"], globalData.textures["brick"]);
	floor.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	floor.translate(glm::vec3(0,-0.95,0));
	globalData.objectsToDisplay.push_back(&floor);
	globalData.startFloor = &floor;

	user.addToCheckColisionList(&floor);
	user.respawn();

	globalData.objectsToDisplay.push_back(&user);
	globalData.activeUser = &user;
	globalData.activeCamera = user.fpsViewCamera();
	globalData.objectsToKeyboard.push_back(&user);
	globalData.objectsToMouseMove.push_back(&user);
	globalData.objectsToGravity.push_back(&user);

	/* Solar systen */
	class Planet :public TexturedObject, public AnimatedObject {
		void animate() {
			rotate(0.35, glm::vec3(0,1,0));
		}
	};

	
	Planet sun;
	/* Ukljucivanje svetla (od sunca) */
	globalData.lights["sunlight"].enable();

	sun.texture = globalData.textures["sun"];
	sun.material = globalData.materials["normal"];
	sun.model = globalData.models["sphere"];
	sun.translate(glm::vec3(0, 50, 100));

	sun.startAnimation();
	globalData.objectsToAnimation.push_back(&sun);
	globalData.objectsToDisplay.push_back(&sun);

	Planet earth;
	earth.texture = globalData.textures["earth"];
	earth.material = globalData.materials["normal"];
	earth.model = globalData.models["sphere"];
	earth.translate(glm::vec3(-50, 0, 0));
	earth.scale(glm::vec3(0.5,0.5,0.5));
	sun.addChild(&earth);

	earth.startAnimation();
	globalData.objectsToAnimation.push_back(&earth);

	Planet moon;
	moon.texture = globalData.textures["moon"];
	moon.material = globalData.materials["normal"];
	moon.model = globalData.models["sphere"];
	moon.translate(glm::vec3(-50, 0, 0));
	moon.scale(glm::vec3(0.5,0.5,0.5));
	earth.addChild(&moon);

	user.addToCheckColisionList(&sun);
	user.addToCheckColisionList(&earth);
	user.addToCheckColisionList(&moon);


	class Spaceship : public TexturedObject, public AnimatedObject  {
		void animate() {
			translate(glm::vec3(0,0.4*sin(2*M_PI*animationParameter),1));
			rotate(0.5, glm::vec3(0,1,0));
		}
	};



	std::array<Spaceship, 5> spaceships;
	spaceships[1].translate(glm::vec3(0, -25, 10));
	spaceships[1].rotate(180, glm::vec3(0, 1, -5));
	spaceships[2].translate(glm::vec3(0, -50, 50));
	spaceships[2].rotate(90, glm::vec3(0, 1, 8));
	spaceships[3].translate(glm::vec3(0, -75, -100));
	spaceships[3].rotate(45, glm::vec3(0, 1, -17));
	spaceships[4].translate(glm::vec3(0, -100, -150));
	for (unsigned i = 0; i < spaceships.size(); i++) {

		user.addToCheckColisionList(&spaceships[i]);
		spaceships[i].setStep(0.0007);
		spaceships[i].model = globalData.models["spaceship"];
		spaceships[i].texture = globalData.textures["spaceship"];
		spaceships[i].material = globalData.materials["normal"];
		spaceships[i].startAnimation();
		globalData.objectsToDisplay.push_back(&spaceships[i]);
		globalData.objectsToAnimation.push_back(&spaceships[i]);
	}

	TexturedObject cubes[25];
	for (int i=0; i<25; i++) {
		cubes[i].texture = globalData.textures["box"];
		cubes[i].material = globalData.materials["normal"];
		cubes[i].model = globalData.models["cube"];
		cubes[i].rotate((float)i/20.0f*180, glm::vec3(0,1,0));
		cubes[i].translate(glm::vec3(i*0.15+5.3, i*0.3f, i*0.15+5.3));
		//cubes[i].scale(glm::vec3(1.0f, 0.21f, 1.0f));
		user.addToCheckColisionList(&cubes[i]);
		globalData.objectsToDisplay.push_back(&cubes[i]);
	}

	TexturedObject floor1(NULL, glm::vec4(0.3,0.3,0.3,1.0), globalData.models["cube"], globalData.materials["normal"], globalData.textures["granite"]);
	floor1.translate(glm::vec3(-17, 10, -17));
	floor1.scale(glm::vec3(5, 5, 5));
	globalData.objectsToDisplay.push_back(&floor1);
	user.addToCheckColisionList(&floor1);

	class Lift : public TexturedObject, public AnimatedObject {
		void animate() {
			glm::vec3 trvec = glm::vec3(0, 0.1*sin(animationParameter*2*M_PI), 0);
			translate(trvec);
			if (globalData.activeUser->isColiding(this)) {
				globalData.activeUser->translate(globalData.activeUser->vecToObjectSys(trvec));
			}
		}
	};

	std::array<Lift, 4> liftovi;
	float coef_t_lift = 12;
	liftovi[0].translate(glm::vec3(-coef_t_lift-3.5,10,0));
	liftovi[1].translate(glm::vec3(-coef_t_lift-7.0,10,-3));
	liftovi[2].translate(glm::vec3(-coef_t_lift-9.5,10,-6));
	liftovi[3].translate(glm::vec3(-coef_t_lift-12.0,10,-9));

	for (unsigned i = 0; i < liftovi.size(); i++) {
		liftovi[i].translate(glm::vec3(0,-5,0));
		liftovi[i].model = globalData.models["cube"];
		liftovi[i].texture = globalData.textures["box"];
		liftovi[i].material = globalData.materials["normal"];
		liftovi[i].setStep(0.001);
		liftovi[i].startAnimation();
		globalData.objectsToAnimation.push_back(&liftovi[i]);
		globalData.objectsToDisplay.push_back(&liftovi[i]);
		user.addToCheckColisionList(&liftovi[i]);
	}

	class Slider : public TexturedObject, public AnimatedObject {
		void animate() {
			glm::vec3 trvec = glm::vec3(0.1*sin(animationParameter*2*M_PI), 0, 0);
			translate(trvec);
			if (globalData.activeUser->isColiding(this)) {
				globalData.activeUser->translate(globalData.activeUser->vecToObjectSys(trvec));
			}
		}
	};

	Slider slider;
	slider.translate(glm::vec3(-10,13,-17));
		slider.model = globalData.models["cube"];
		slider.texture = globalData.textures["box"];
		slider.material = globalData.materials["normal"];
		slider.setStep(0.001);
		slider.startAnimation();
		globalData.objectsToAnimation.push_back(&slider);
		globalData.objectsToDisplay.push_back(&slider);
		user.addToCheckColisionList(&slider);

	TexturedObject cubes1[10];
	for (int i=0; i<10; i++) {
		cubes1[i].texture = globalData.textures["box"];
		cubes1[i].material = globalData.materials["normal"];
		cubes1[i].model = globalData.models["cube"];
		cubes1[i].translate(glm::vec3(24, 12, -17));
		cubes1[i].translate(glm::vec3(2.5*i, i*0.4f, i*0.15));
		//cubes[i].scale(glm::vec3(1.0f, 0.21f, 1.0f));
		user.addToCheckColisionList(&cubes1[i]);
		globalData.objectsToDisplay.push_back(&cubes1[i]);
	}

	class Rotator : public TexturedObject, public AnimatedObject {

		void animate() {
			//glm::vec3 trvec = glm::vec3(0.1*sin(animationParameter*2*M_PI), 0.1*sin(animationParameter*2*M_PI), 0);
			rotate(0.5, glm::vec3(0,1,0));
			//translate(trvec);

			if (globalData.activeUser->isColiding(this)) {
				//globalData.activeUser->translate(globalData.activeUser->vecToObjectSys(trvec));
				globalData.activeUser->rotate(0.5, glm::vec3(0,1,0));
			}
		}

	};

	std::array<Rotator, 10> rotators;
	for (int i = 0; i < 10; i++) {
		rotators[i].translate(glm::vec3(47,16 + 0.3 * i,-13+3*i));
		rotators[i].model = globalData.models["cube"];
		rotators[i].texture = globalData.textures["box"];
		rotators[i].material = globalData.materials["normal"];
		rotators[i].setStep(0.001);
		rotators[i].startAnimation();
		globalData.objectsToAnimation.push_back(&rotators[i]);
		globalData.objectsToDisplay.push_back(&rotators[i]);
		user.addToCheckColisionList(&rotators[i]);
	}

	Slider slider1;
	slider1.translate(glm::vec3(12,17,14));
	slider1.model = globalData.models["cube"];
	slider1.texture = globalData.textures["box"];
	slider1.material = globalData.materials["normal"];
	slider1.setStep(0.001);
	slider1.startAnimation();
	globalData.objectsToAnimation.push_back(&slider1);
	globalData.objectsToDisplay.push_back(&slider1);
	user.addToCheckColisionList(&slider1);


	TexturedObject floor2(NULL, glm::vec4(0.3,0.3,0.3,1.0), globalData.models["cube"], globalData.materials["normal"], globalData.textures["asteroid"]);
	floor2.translate(glm::vec3(12,10,13));
	floor2.scale(glm::vec3(3.0f, 3.0f, 3.0f));
	globalData.objectsToDisplay.push_back(&floor2);
	user.addToCheckColisionList(&floor2);

	std::array<Lift, 4> liftovi1;
	float coef_t_lift1 = 0;

	liftovi1[0].translate(glm::vec3(10-coef_t_lift1-3.5,11,16));
	liftovi1[1].translate(glm::vec3(10-coef_t_lift1-7.0,11,16 - 3));
	liftovi1[2].translate(glm::vec3(10-coef_t_lift1-9.5,11,16 - 6));
	liftovi1[3].translate(glm::vec3(10-coef_t_lift1-12.0,11,16 - 9));

	for (unsigned i = 0; i < liftovi1.size(); i++) {
		liftovi1[i].model = globalData.models["cube"];
		liftovi1[i].texture = globalData.textures["box"];
		liftovi1[i].material = globalData.materials["normal"];
		liftovi1[i].setStep(0.001);
		liftovi1[i].startAnimation();
		globalData.objectsToAnimation.push_back(&liftovi1[i]);
		globalData.objectsToDisplay.push_back(&liftovi1[i]);
		user.addToCheckColisionList(&liftovi1[i]);
	}


	TexturedObject floor3(NULL, glm::vec4(0.3,0.3,0.3,1.0), globalData.models["cube"], globalData.materials["normal"], globalData.textures["finish"]);
	floor3.translate(glm::vec3(-7,40,7));
	floor3.scale(glm::vec3(3.0f, 3.0f, 3.0f));
	globalData.objectsToDisplay.push_back(&floor3);
	user.addToCheckColisionList(&floor3);
	globalData.endFloor = &floor3;



	//Camera cam(&spaceships[0]);
	//cam.matrix = glm::translate(cam.matrix, glm::vec3(0, 0, -25));
	//globalData.activeCamera = &cam;

	glutMainLoop();

	return 0;
}


/* CALLBACK-ovi */
static void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Postavljanje tacke gledista */
	if (globalData.activeCamera != NULL)
		glLoadMatrixf(glm::value_ptr(globalData.activeCamera->viewMatrix()));

	/* Sortiranje po daljini od user-a  Posto se iz vektora cita unazad,
	 * sortiramo u obrnutom poretku, na pocetak idu najdalji - ovo je 
	 * bitno zbog ispravne obrade providnosti objekata!! */
	//std::sort (globalData.objectsToDisplay.begin(), globalData.objectsToDisplay.end(), [] (Object * a, Object * b) -> bool {

	//	/* TODO: Ovo da bi bilo generalnije treba da bude daljina od kamere a ne od user-a 
	//	 * -> Od user-a je okej ako je kamera vezana za user-a, a ako se veze za neki drugi 
	//	 *  objekat ili neki drugi tip kamere bude (ne First Person) ovo nece valjati */
	//	float aDistanceFromUser = glm::length(globalData.activeUser->pointToObjectSys(a, glm::vec3(0,0,0)));
	//	float bDistanceFromUser = glm::length(globalData.activeUser->pointToObjectSys(b, glm::vec3(0,0,0)));

	//	if (aDistanceFromUser > bDistanceFromUser)
	//		return true;
	//	else
	//		return false;
	//});

	/* Iscrtavanje objekata */
	for_each (globalData.objectsToDisplay.begin(), globalData.objectsToDisplay.end(), [] (Object * o) {

		if(DrawableObject * d_o = dynamic_cast<DrawableObject*>(o))
			d_o->draw();

	});

       // /* Tekst za ime trenutnog (exploring) direktorijum */
	//glm::vec3 userPosition = globalData.activeUser->pointToWorldSys(glm::vec3(0,0,0));

	//std::stringstream ss; 
	//ss << "(" << userPosition.x << "," << userPosition.y << "," << userPosition.z << ")";
	std::ostringstream ss;
	ss << globalData.time_elapsed;
	std::string time_elapsed(ss.str());
	//ss << globalData.time_elapsed;
	//ss >> time_elapsed;

        Text userTimeText(glm::vec2(15.0, globalData.screenSize.y - 25.0), glm::vec3(1,0,0), "Time: " + time_elapsed + "s");
	globalData.textToScreenVec.push_back(userTimeText);

	if (globalData.activeUser->isColiding(globalData.endFloor)) {
		std::ostringstream sb;
		sb << globalData.time_elapsed;
		Text userTimeText(glm::vec2(globalData.screenSize.x/2-120, globalData.screenSize.y/2), glm::vec3(0,1,0), "Cestitamo, vase vreme je: " + time_elapsed + "s");
		globalData.textToScreenVec.push_back(userTimeText);
	}

       // /* Ovde se obradjuju prosledjeni tekstovi na ekran */
       
       
       for (auto e : globalData.textToScreenVec)
        	e.print(globalData.screenSize);

       // /* Ispisani tekst se brise */
       globalData.textToScreenVec.clear();

	glutSwapBuffers();
}

static void on_reshape(int width, int height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Config & appConfig = globalData.configs["application"];
	float viewMin = stof(appConfig.getParameter("VIEW_DISTANCE_MIN"));
	float viewMax = stof(appConfig.getParameter("VIEW_DISTANCE_MAX"));

	gluPerspective(60, (float) width/(float) height, viewMin, viewMax);

	/* Cuvamo velicinu ekrana jer nam treba na drugim mestima */
	globalData.screenSize = glm::vec2(width,height);
}

static void on_keyboard(unsigned char c, int x, int y)
{
	/* U switch-u se obradjuju komande koje ne uticu na
	 * pojedinacne objekte vec na ceo program */
	switch (c) {

		/* TODO: Pause key iz config-a - mozda, za sada problem constexpr */
		case 'p': 
			if (globalData.timersActive)
				pauseTimers();
			else
				unpauseTimers();

			break;

		/* 27 -> ESC_KEY */
		case 27: 
			exit(EXIT_SUCCESS);

	}
	
	/* Belezimo key koji je pritisnut tj. belezimo 
	 * u nase globalne podatke da je pritisnut i
	 * na kojoj poziciji je pritisnut (x,y)*/
	globalData.pressedKeys[c] = true;

	/* TODO: Iskoristiti za poziciju glm::vec2 */
	globalData.keyPressedPositionX = x;
	globalData.keyPressedPositionY = y;
}

static void on_keyboard_release(unsigned char c, int x, int y)
{
	/* Belezimo key koji je pusten tj. belezimo
	 * u nase globalne podatke da je pusten i
	 * na kojoj poziciji je pusten (x,y)*/
	globalData.pressedKeys[c] = false;

	/* TODO: Iskoristiti za poziciju glm::vec2 */
	globalData.keyReleasedPositionX = x;
	globalData.keyReleasedPositionY = y;
}

static void on_mouse_move(int x, int y) 
{
	/* Belezimo poziciju misa u globalne podatke */
	globalData.mousePosition = glm::vec2(x,y);
}


/* TAJMERI */ 
static void redisplay_timer(int value)
{
	if (value != globalData.redisplayTimerId || !globalData.timersActive)
		return;

	glutPostRedisplay();
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
}

static void keyboard_timer(int value) 
{
	if (value != globalData.keyboardTimerId || !globalData.timersActive)
		return;

	/* Obradjivanje zahteva za tastatru */
	for_each (globalData.objectsToKeyboard.begin(), globalData.objectsToKeyboard.end(), [] (Object * o) {

		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o))
			m_o->processKeyboardInput(globalData.pressedKeys, globalData.keyPressedPositionX, globalData.keyPressedPositionY);

	});

	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
}

static void mouse_timer(int value) 
{
	if (value != globalData.mouseTimerId || !globalData.timersActive)
		return;

	/* TODO: Ovde lepse resenje !!!! */
	GLint m_viewport[4];
	glGetIntegerv( GL_VIEWPORT, m_viewport );

	glm::vec2 center(m_viewport[2]/2, m_viewport[3]/2);
	glm::vec2 mousePosition = globalData.mousePosition;

	glm::vec2 delta = center-mousePosition;

	for_each (globalData.objectsToMouseMove.begin(), globalData.objectsToMouseMove.end(), [delta] (Object * o) {

		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o)) 
			m_o->processMouseMove(delta);

	});

	glutWarpPointer( center.x , center.y );

	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
}

static void gravity_timer(int value)
{
	if (value != globalData.gravityTimerId || !globalData.timersActive)
		return;

	/* Pad objekata kojima je pridruzena gravitacija i nisu na podu */
	for_each (globalData.objectsToGravity.begin(), globalData.objectsToGravity.end(), [] (Object * o) {

		if(MovableObject* m_o = dynamic_cast<MovableObject*>(o)) {
			float gravity = -stof(globalData.configs["gravity"].getParameter("GRAVITY"));
			m_o->addToVelocity(glm::vec3(0.0, gravity, 0.0));
			m_o->move(m_o->getVelocity());
			glm::vec3 position = m_o->pointToWorldSys(glm::vec3(0,0,0));
			if (position.y < -300) {
				m_o->setVelocity(glm::vec3(0,0,0));
				m_o->respawn();
			}
		}

	});

	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
}


static void animation_timer(int value) 
{
	/* Ovaj tajmer moze da se shvati kao tajmer za sve poslove file explorer simulacije,
	 * ovde je ubacen za razresavanje kolizije sa objektima, za neku drugu simulaciju 
	 * ovaj tajmer postoji samo za file explorer */

	if (value != globalData.animationTimerId || !globalData.timersActive)
		return;
	
	for_each (globalData.objectsToAnimation.begin(), globalData.objectsToAnimation.end(), [] (Object * o) {

		if(AnimatedObject* a_o = dynamic_cast<AnimatedObject*>(o)) {
			if (a_o->isAnimationOngoing()) {
				a_o->animate();
				a_o->inctementForStep();
			}
		}

	});

	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);

}

static void pauseTimers() 
{
	globalData.timersActive = false;

	/* Prikazivanje kursor-a */
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	// TODO: Uncomment kada se pause key implementira
	//string pauseKey = globalData.configs["keyboard"].getParameter("PAUSE");
	// + Ovaj pause text da postoji neki config tipa messages, pa da se iz toga citaju poruke 
	
	Text pauseMsg(glm::vec2(globalData.screenSize.x/2.0 - 170.0, globalData.screenSize.y/2.0), glm::vec3(1,0,0), "Pause is set, to unpause press 'p' key");
	globalData.textToScreenVec.push_back(pauseMsg);


	glutPostRedisplay();
}

static void unpauseTimers() 
{
	globalData.timersActive = true;

	/* Sakrivanje kursor-a */
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Tajmeri */
	glutTimerFunc(globalData.redisplayTimerInterval, redisplay_timer, globalData.redisplayTimerId);
	glutTimerFunc(globalData.mouseTimerInterval, mouse_timer, globalData.mouseTimerId);
	glutTimerFunc(globalData.keyboardTimerInterval, keyboard_timer, globalData.keyboardTimerId);
	glutTimerFunc(globalData.gravityTimerInterval, gravity_timer, globalData.gravityTimerId);
	glutTimerFunc(globalData.animationTimerInterval, animation_timer, globalData.animationTimerId);

	/* Simulation timer */
	glutTimerFunc(globalData.simulationTimerInterval, simulation_timer, globalData.simulationTimerId);
}


static void simulation_timer(int value)
{

	if (value != globalData.simulationTimerId || !globalData.timersActive)
		return;

	if (globalData.activeUser->isColiding(globalData.startFloor)) {
		globalData.time_elapsed = 0;
	} else if (globalData.activeUser->isColiding(globalData.endFloor)) {
	} else {
		globalData.time_elapsed += 1;
	}


	glutTimerFunc(globalData.simulationTimerInterval, simulation_timer, globalData.simulationTimerId);
}

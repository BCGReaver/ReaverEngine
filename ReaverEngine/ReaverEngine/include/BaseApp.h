#pragma once
#include <Prerequisites.h>
#include "Window.h"

class
	BaseApp {
public:
	BaseApp() = default;
	~BaseApp();

	//Función encargada de ejecutar la aplicación en main
	int
		run();

	//Función de inicialización
	bool
		init();

	//Función que se actualiza por frame
	void
		update();

	//Función de renderizado
	void
		render();

	void
		destroy();

private:
	//Son punteros globales para poder gestionar los recursos que utilizan
	//Inician con m_ porque son miembros de una clase. Si estuvieran en main de forma global serían g_
	//sf::RenderWindow* m_window; -> Se creo una clase a parte para no depender de SFMLN
	Window* m_window;
	sf::CircleShape* m_circle;
};
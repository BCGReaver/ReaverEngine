#pragma once
#include "Prerequisites.h"

class BaseApp{

public:
	BaseApp() = default;
	~BaseApp();

    // Funcion encargada de ejecutar la aplicación en main
	int 
	run();

	// Funcion inicializadora
	bool 
	init();

	// Funcion que se acytualiza por frame
	void 
	update();

	// Funcion renderizado
	void 
	render();

	void 
	destroy();

	void 
	handleEvents();

private: 
	sf::RenderWindow* m_window;
	sf::CircleShape*  m_circle;
};
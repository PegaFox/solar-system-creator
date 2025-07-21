#define _USE_MATH_DEFINES

#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <random>
#include "utils.hpp"
#include "menu_utils.hpp"
#include <glm/gtc/matrix_transform.hpp>

std::default_random_engine randomGenerator(time(nullptr));

//const double BIG_G = 0.0000000000674;
const double BIG_G = 1.0;

#include "body.hpp"

/*
mass measured in 10**25 kg

gravity equation:
gravity constant = g
attraction force = f
distance between the center of planets = d
mass of object 1 = m1
mass of object 2 = m2
f=g*((m1*m2)/(d*d))
*/

std::vector<Body> bodies;

sf::View cam(sf::FloatRect(0.0f, 0.0f, 512.0f, 512.0f));

uint32_t speed = 1U;
bool paused = true;

bool focus = true;

#include "gui.hpp"

GUI gui;

#include "lookahead.hpp"

int main() {
  pf::FPS fpsClock;

  Lookahead lookahead;

  sf::RenderWindow SCREEN(sf::VideoMode(512, 512), "planets", sf::Style::Close);

  SCREEN.setFramerateLimit(60U);
  while (SCREEN.isOpen()) {
    {
      sf::Event event;
      while (SCREEN.pollEvent(event)) {
        gui.getEvent(event, SCREEN, cam);
        switch (event.type) {
          case sf::Event::Closed:
            SCREEN.close();
            break;
          case sf::Event::LostFocus:
            focus = false;
            break;
          case sf::Event::GainedFocus:
            focus = true;
            break;
          case sf::Event::MouseWheelScrolled:
            if (!focus) break;
            cam.setSize(cam.getSize().x * (1.0 - event.mouseWheelScroll.delta*0.1f), cam.getSize().y * (1.0 - event.mouseWheelScroll.delta*0.1f));
            break;
          case sf::Event::KeyPressed:
            if (!focus) break;
            if (event.key.code == sf::Keyboard::Up) {
              speed++;
            }
            if (event.key.code == sf::Keyboard::Down && speed > 1) {
              speed--;
            }
            break;
          case sf::Event::KeyReleased:
            if (!focus) break;
            if (event.key.code == sf::Keyboard::Space) {
              paused = !paused;
            }
            if (event.key.code == sf::Keyboard::C) {
              for (uint32_t b = 0; b < bodies.size(); b++) {
                if (!bodies[b].active || bodies[b].lowDis > 10000) {

                  if (gui.selectedBody != -1) {
                    if (gui.selectedBody > b) {
                      gui.selectedBody--;
                    } else if (gui.selectedBody == b) {
                      gui.selectedBody = -1;
                    }
                  }

                  if (gui.following != -1) {
                    if (gui.following > b) {
                      gui.following--;
                    } else if (gui.following == b) {
                      gui.following = -1;
                    }
                  }

                  bodies.erase(bodies.begin()+b);
                  b--;
                }
              }
            }
            if (event.key.code == sf::Keyboard::R) {
              bodies.clear();
              gui.selectedBody = -1U;
              gui.following = -1U;
            }
            break;
        }
      }
    }

    if (!paused && gui.following != -1) {
      sf::Vector2f center = cam.getCenter();
      cam.setCenter(sf::Vector2f(center.x + bodies[gui.following].vel.x*speed, center.y + bodies[gui.following].vel.z*speed));
    }

    sf::Vector2f moveSpeed = cam.getSize() * 0.01f;
    if (focus) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        cam.setCenter(cam.getCenter().x, cam.getCenter().y - moveSpeed.y);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        cam.setCenter(cam.getCenter().x, cam.getCenter().y + moveSpeed.y);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        cam.setCenter(cam.getCenter().x - moveSpeed.x, cam.getCenter().y);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        cam.setCenter(cam.getCenter().x + moveSpeed.x, cam.getCenter().y);
      }
    }
    SCREEN.setView(cam);

    SCREEN.setTitle(std::to_string(fpsClock.get_fps())+", "+std::to_string(speed));

    if (!paused) {
      for (uint32_t i = 0; i < speed; i++) {
        for (Body& body : bodies) {
          body.updateVel(bodies);
        }
        for (Body& body : bodies) {
          body.updatePos();
        }
      }
      
    }

    SCREEN.clear();

    for (Body& body : bodies) {
      body.draw(SCREEN);
    }

    lookahead.update(SCREEN, fpsClock.fps);

    SCREEN.setView(SCREEN.getDefaultView());
    
    gui.update(SCREEN);

    SCREEN.display();
  }
  return 0;
}

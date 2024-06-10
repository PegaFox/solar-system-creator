class GUI {
  public:
    uint32_t lookahead = 0;

    uint32_t selectedBody = -1U;
    uint32_t following = -1;

    GUI() {
      up.loadFromFile("../increase.png");
      upIcon.setTexture(up);
      upIcon.setScale(1.0f, 1.0f);

      down.loadFromFile("../decrease.png");
      downIcon.setTexture(down);
      downIcon.setScale(1.0f, 1.0f);

      font.loadFromFile("../pico-8-mono.ttf");
      text.setFont(font);
      text.setCharacterSize(16U);

      lookaheadDownBtn.setPosition(sf::Vector2f(6.0f, 30.0f));
      lookaheadUpBtn.setPosition(sf::Vector2f(138.0f, 30.0f));
      massDownBtn.setPosition(sf::Vector2f(6.0f, 74.0f));
      massUpBtn.setPosition(sf::Vector2f(250.0f, 74.0f));
      radiusDownBtn.setPosition(sf::Vector2f(6.0f, 118.0f));
      radiusUpBtn.setPosition(sf::Vector2f(250.0f, 118.0f));
      lenDownBtn.setPosition(sf::Vector2f(272.0f, 162.0f));
      lenUpBtn.setPosition(sf::Vector2f(490.0f, 162.0f));

      angSlider.setFont("../pico-8-mono.ttf");
      angSlider.caption = "ANG: ";
      angSlider.range = sf::Vector2f(0.0, M_PI*2.0);
      angSlider.setPosition(sf::Vector2f(10.0f, 164.0f));

      followBtn.setFont("../pico-8-mono.ttf");
      followBtn.setPosition(sf::Vector2f(160.0f, 32.0f));
      followBtn.setSize(sf::Vector2f(220.0f, 16.0f));

      saveBtn.setFont("../pico-8-mono.ttf");
      saveBtn.setCaption("SAVE");
      saveBtn.setPosition(sf::Vector2f(160.0f, 10.0f));
      saveBtn.setSize(sf::Vector2f(60.0f, 16.0f));
      loadBtn.setFont("../pico-8-mono.ttf");
      loadBtn.setCaption("LOAD");
      loadBtn.setPosition(sf::Vector2f(230.0f, 10.0f));
      loadBtn.setSize(sf::Vector2f(60.0f, 16.0f));
    }

    void getEvent(const sf::Event& event, const sf::RenderWindow& SCREEN, const sf::View& cam)
    {
      if (event.type == sf::Event::MouseButtonPressed)
      {
        glm::vec2 mousePos = pf::vector2fToVec2(SCREEN.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), cam));
        switch (event.mouseButton.button)
        {
          case sf::Mouse::Left:
            if (follow) {
              if (following == selectedBody) {
                following = -1;
              } else {
                following = selectedBody;
              }
            }

            if (save) {
              saveSystem();
            }
            if (load) {
              loadSystem();
            }

            for (uint32_t b = 0; b < bodies.size(); b++) {
              if (glm::distance(mousePos, glm::vec2(bodies[b].pos.x, bodies[b].pos.z)) <= bodies[b].radius) {
                held = b;
              }
            }
            break;

          case sf::Mouse::Middle:
            selectedBody = -1;
            for (uint32_t b = 0; b < bodies.size(); b++) {
              if (glm::distance(mousePos, glm::vec2(bodies[b].pos.x, bodies[b].pos.z)) <= bodies[b].radius) {
                selectedBody = b;
              }
            }
          break;

          case sf::Mouse::Right:
            bodies.push_back(Body());
            bodies.back().pos.x = mousePos.x;
            bodies.back().pos.z = mousePos.y;
            bodies.back().color = glm::vec3((float)randomGenerator()/randomGenerator.max(), (float)randomGenerator()/randomGenerator.max(), (float)randomGenerator()/randomGenerator.max());
            bodies.back().mass = 1;
            bodies.back().radius = 1;
            break;
        }
      }

      angSlider.getEvent(event);
    }

    void update(sf::RenderWindow& SCREEN) {

      if (paused) {
        text.setString("LOOKAHEAD");
        text.setPosition(10.0f, 10.0f);
        SCREEN.draw(text);

        text.setString(std::to_string(lookahead));
        text.setPosition(26.0f, 32.0f);
        SCREEN.draw(text);

        lookaheadDownBtn.update(SCREEN, SCREEN);

        downIcon.setPosition(6.0f, 30.0f);
        SCREEN.draw(downIcon);

        lookaheadUpBtn.update(SCREEN, SCREEN);

        upIcon.setPosition(138.0f, 30.0f);
        SCREEN.draw(upIcon);

        saveBtn.update(SCREEN, SCREEN);
        loadBtn.update(SCREEN, SCREEN);
      }

      if (selectedBody != -1) {
        if (paused) {
          if (following == selectedBody) {
            followBtn.setCaption("STOP FOLLOWING");
          } else {
            followBtn.setCaption("    FOLLOW    ");
          }
          followBtn.update(SCREEN, SCREEN);

          text.setString("MASS");
          text.setPosition(106.0f, 54.0f);
          SCREEN.draw(text);

          text.setString(std::to_string(bodies[selectedBody].mass));
          text.setPosition(26.0f, 76.0f);
          SCREEN.draw(text);

          massDownBtn.update(SCREEN, SCREEN);

          downIcon.setPosition(6.0f, 74.0f);
          SCREEN.draw(downIcon);

          massUpBtn.update(SCREEN, SCREEN);

          upIcon.setPosition(250.0f, 74.0f);
          SCREEN.draw(upIcon);

          text.setString("RADIUS");
          text.setPosition(90.0f, 98.0f);
          SCREEN.draw(text);

          text.setString(std::to_string(bodies[selectedBody].radius));
          text.setPosition(26.0f, 120.0f);
          SCREEN.draw(text);

          radiusDownBtn.update(SCREEN, SCREEN);

          downIcon.setPosition(6.0f, 118.0f);
          SCREEN.draw(downIcon);

          radiusUpBtn.update(SCREEN, SCREEN);

          upIcon.setPosition(250.0f, 118.0f);
          SCREEN.draw(upIcon);

          text.setString("VELOCITY: "+std::to_string(bodies[selectedBody].vel.x)+", "+std::to_string(bodies[selectedBody].vel.z));
          text.setPosition(10.0f, 142.0f);
          SCREEN.draw(text);

          // velocity settings
          if (following == -1) {
            ang = pf::getAngle(bodies[selectedBody].vel.x, bodies[selectedBody].vel.z, 0, 0);
            len = glm::length(bodies[selectedBody].vel);
          } else {
            glm::dvec3 relVel = bodies[selectedBody].vel - bodies[following].vel;

            ang = pf::getAngle(relVel.x, relVel.z, 0, 0);
            len = glm::length(relVel);
          }
          float startAng = ang;
          float startLen = len;

          angSlider.update(SCREEN, SCREEN);

          text.setString(std::to_string(len));
          text.setPosition(292.0f, 164.0f);
          SCREEN.draw(text);

          lenDownBtn.update(SCREEN, SCREEN);

          downIcon.setPosition(272.0f, 162.0f);
          SCREEN.draw(downIcon);

          lenUpBtn.update(SCREEN, SCREEN);

          upIcon.setPosition(490.0f, 162.0f);
          SCREEN.draw(upIcon);

          if (lenUp) {
            len += changeValue*0.01f;
          }
          if (lenDown) {
            len -= changeValue*0.01f;
          }

          if (startAng != ang || startLen != len) {
            if (following == -1) {
              bodies[selectedBody].vel.x = glm::cos(ang)*len;
              bodies[selectedBody].vel.z = glm::sin(ang)*len;
            } else {
              bodies[selectedBody].vel.x = bodies[following].vel.x + glm::cos(ang)*len;
              bodies[selectedBody].vel.z = bodies[following].vel.z + glm::sin(ang)*len;
            }
          }
        }

        if (following == -1) {
          sf::Color arrowColor(255.0f - bodies[selectedBody].color[0]*255.0f, 255.0f - bodies[selectedBody].color[1]*255.0f, 255.0f - bodies[selectedBody].color[2]*255.0f);
          velArrow[0] = sf::Vertex(sf::Vector2f(SCREEN.mapCoordsToPixel(sf::Vector2f(bodies[selectedBody].pos.x, bodies[selectedBody].pos.z), cam)), arrowColor);
          velArrow[1] = sf::Vertex(sf::Vector2f(SCREEN.mapCoordsToPixel(sf::Vector2f(bodies[selectedBody].pos.x + bodies[selectedBody].vel.x*60.0f, bodies[selectedBody].pos.z + bodies[selectedBody].vel.z*60.0f), cam)), arrowColor);
        } else {
          glm::dvec3 relVel = bodies[selectedBody].vel - bodies[following].vel;

          sf::Color arrowColor(255.0f - bodies[selectedBody].color[0]*255.0f, 255.0f - bodies[selectedBody].color[1]*255.0f, 255.0f - bodies[selectedBody].color[2]*255.0f);
          velArrow[0] = sf::Vertex(sf::Vector2f(SCREEN.mapCoordsToPixel(sf::Vector2f(bodies[selectedBody].pos.x, bodies[selectedBody].pos.z), cam)), arrowColor);
          velArrow[1] = sf::Vertex(sf::Vector2f(SCREEN.mapCoordsToPixel(sf::Vector2f(bodies[selectedBody].pos.x + relVel.x*60.0f, bodies[selectedBody].pos.z + relVel.z*60.0f), cam)), arrowColor);
        }

        SCREEN.draw(velArrow);
      }

      if (paused) {
        if (focus && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          if (changeSpeed >= 1.0f) {
            changeSpeed += 1.0f;
            changeValue++;
          } else if (changeTick > 1.0f / changeSpeed) {
            changeSpeed += 0.1f;
            changeValue++;
            changeTick = 0U;
          } else {
            changeValue = 0U;
            changeTick++;
          }
        } else {
          changeSpeed = 0.1f;
          changeValue = 0U;
          changeTick = 0U;
        }

        if (lookaheadUp) {
          lookahead += changeValue;
        }
        if (lookaheadDown) {
          lookahead -= changeValue;
        }
        if (lookahead > 9999999) {
          lookahead = 0U;
        }

        if (selectedBody != -1) {
          if (focus && sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
            bodies.erase(bodies.begin()+selectedBody);
            selectedBody = -1;
          }

          if (massUp) {
            bodies[selectedBody].mass += changeValue;
          }
          if (massDown) {
            bodies[selectedBody].mass -= changeValue;
          }

          if (radiusUp) {
            bodies[selectedBody].radius += changeValue;
          }
          if (radiusDown) {
            bodies[selectedBody].radius -= changeValue;
          }

          if (focus && held != -1 && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f m = SCREEN.mapPixelToCoords(sf::Mouse::getPosition(SCREEN), cam);
            bodies[held].pos.x = m.x;
            bodies[held].pos.z = m.y;
          } else {
            held = -1;
          }
        }
      }
    }
  private:
    sf::Texture up;
    sf::Sprite upIcon;
    
    sf::Texture down;
    sf::Sprite downIcon;

    sf::VertexArray velArrow = sf::VertexArray(sf::Lines, 2);

    sf::Font font;
    sf::Text text;

    float changeSpeed = 0.1f;
    uint32_t changeValue = 0U;
    uint32_t changeTick = 0U;

    uint32_t held = -1;

    float ang = 0.0f;
    pf::Slider<float> angSlider = pf::Slider<float>(&ang);
    float len = 0.0f;

    bool lenUp;
    pf::Button lenUpBtn = pf::Button(&lenUp);
    bool lenDown;
    pf::Button lenDownBtn = pf::Button(&lenDown);

    bool lookaheadUp;
    pf::Button lookaheadUpBtn = pf::Button(&lookaheadUp);
    bool lookaheadDown;
    pf::Button lookaheadDownBtn = pf::Button(&lookaheadDown);

    bool massUp;
    pf::Button massUpBtn = pf::Button(&massUp);
    bool massDown;
    pf::Button massDownBtn = pf::Button(&massDown);

    bool radiusUp;
    pf::Button radiusUpBtn = pf::Button(&radiusUp);
    bool radiusDown;
    pf::Button radiusDownBtn = pf::Button(&radiusDown);

    bool follow;
    pf::Button followBtn = pf::Button(&follow);

    bool save;
    pf::Button saveBtn = pf::Button(&save);
    bool load;
    pf::Button loadBtn = pf::Button(&load);

    void saveSystem() {
      std::ofstream saveFile("save.txt");

      saveFile << BIG_G << "\n\n";

      for (uint32_t b = 0; b < bodies.size(); b++) {
        if (!bodies[b].active) continue;

        glm::dvec3 relPos(bodies[b].pos.x - (double)cam.getCenter().x, bodies[b].pos.y, bodies[b].pos.z - (double)cam.getCenter().y);

        saveFile << relPos.x << ' ' << relPos.y << ' ' << relPos.z << '\n';
        saveFile << bodies[b].vel.x << ' ' << bodies[b].vel.y << ' ' << bodies[b].vel.z << '\n';
        saveFile << bodies[b].color[0] << ' ' << bodies[b].color[1] << ' ' << bodies[b].color[2] << '\n';
        saveFile << bodies[b].radius << '\n';
        saveFile << bodies[b].mass << "\n\n";
      }

      saveFile.close();
    }

    void loadSystem() {
      std::ifstream saveFile("save.txt");

      double savedBigG;
      saveFile >> savedBigG;
      if (savedBigG != BIG_G) {
        std::clog << "WARNING: configured gravity is different from saved gravity\n";
      }

      while (!saveFile.eof()) {
        bodies.emplace_back(Body());
        Body* currentBody = &bodies.back();

        saveFile >> currentBody->pos.x >> currentBody->pos.y >> currentBody->pos.z;
        currentBody->pos.x += cam.getCenter().x;
        currentBody->pos.z += cam.getCenter().y;

        saveFile >> currentBody->vel.x >> currentBody->vel.y >> currentBody->vel.z;
        saveFile >> currentBody->color[0] >> currentBody->color[1] >> currentBody->color[2];
        saveFile >> currentBody->radius;
        saveFile >> currentBody->mass;
      }

      saveFile.close();

      bodies.pop_back();
    }
};
class Lookahead {
  public:

    std::vector<Body> futureBodies;
    std::vector<sf::VertexArray> sprites;
    float precision = 1.0f;

    Lookahead() {

    };

    void update(sf::RenderTarget& SCREEN, float fps) {
      //adjustPrecision(fps);
      setup();

      for (uint32_t i = 0; i < iterations; i++) {
        for (uint32_t b = 0; b < bodies.size(); b++) {
          futureBodies[b].updateVel(futureBodies);
        }

        for (uint32_t b = 0; b < bodies.size(); b++) {
          if (futureBodies[b].color == glm::vec3(1.0f, 0.0f, 0.0f)) continue;

          if (!futureBodies[b].active) {
            futureBodies[b].color = glm::vec3(1.0f, 0.0f, 0.0f);

            if (gui.following != -1) {
              glm::dvec3 shift = futureBodies[gui.following].pos - bodies[gui.following].pos;
              futureBodies[b].pos -= shift;
            }
            futureBodies[b].draw(SCREEN);
            //futureBodies.erase(futureBodies.begin()+b);

            sprites[b].resize(i);
            SCREEN.draw(sprites[b]);
            //sprites.erase(sprites.begin()+b);
            //b--;
            continue;
          }

          futureBodies[b].updatePos();

          sprites[b][i+1].color = sf::Color(futureBodies[b].color[0]*128.0f, futureBodies[b].color[1]*128.0f, futureBodies[b].color[2]*128.0f);
          if (gui.following != -1) {
            glm::dvec3 shift = futureBodies[gui.following].pos - bodies[gui.following].pos;
            sprites[b][i+1].position = sf::Vector2f(futureBodies[b].pos.x - shift.x, futureBodies[b].pos.z - shift.z);
          } else {
            sprites[b][i+1].position = sf::Vector2f(futureBodies[b].pos.x, futureBodies[b].pos.z);
          }
        }
      }

      for (auto sprite : sprites)
        SCREEN.draw(sprite);
    }

  private:
    uint32_t iterations;
  
    void adjustPrecision(float fps) {
      // setup lookaheadPrecision
      precision += float(40 - fps) * 0.01f;

      if (precision < 1.0f) {
        precision = 1.0f;
      }
      if (precision > gui.lookahead) {
        precision = gui.lookahead;
      }
    }

    void setup() {
      iterations = gui.lookahead / precision;

      futureBodies.resize(bodies.size());
      sprites.clear();
      sprites.reserve(bodies.size());
      for (uint32_t b = 0; b < bodies.size(); b++) {
        futureBodies[b] = bodies[b];
        futureBodies[b].precision = precision;

        sprites.emplace_back(sf::LineStrip, iterations+1);
        sprites[b][0].position = sf::Vector2f(bodies[b].pos.x, bodies[b].pos.z);
        sprites[b][0].color = sf::Color(bodies[b].color[0]*128.0f, bodies[b].color[1]*128.0f, bodies[b].color[2]*128.0f);
      }
    }
};

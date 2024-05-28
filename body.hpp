class Body {
  public:
    glm::dvec3 pos = glm::dvec3(0.0);
    glm::dvec3 vel = glm::dvec3(0.0);

    double precision = 1.0;

    double mass = 1.0;
    double radius = 1.0;

    float shatterThreshold = 2.0f;

    glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f);

    bool active = true;

    double lowDis = INFINITY;

    Body(double radius = 1.0) {
      this->radius = radius;

      sprite.setRadius(radius);
    }

    void updateVel(std::vector<Body>& bodies) {
      if (!active) return;

      lowDis = INFINITY;
      for (Body& body : bodies) {
        if (!body.active || body.pos == pos) {
          continue;
        }

        double dis = glm::distance(body.pos, pos);
        if (dis < lowDis)
          lowDis = dis;

        if (dis < body.radius+radius) {
          double hitForce = glm::distance(vel, body.vel);
          if (false && hitForce > shatterThreshold) {
            uint32_t pieces = hitForce - shatterThreshold;
            glm::dvec3 hitPoint = (pos*body.mass+body.pos*mass) / (mass+body.mass);

            for (uint32_t p = 0; p < pieces; p++) {
              bodies.emplace_back((mass+body.mass) / pieces);
              bodies.back().radius = (radius+body.radius) / pieces;
              bodies.back().pos = hitPoint+glm::dvec3(bodies.back().radius);
            }

            active = false;
            body.active = false;
          } else {
            body.active = false;
            pos = (pos*mass + body.pos*body.mass) / (mass+body.mass);
            vel = (vel*mass + body.vel*body.mass) / (mass+body.mass); 
            radius += body.radius;
            mass += body.mass;
          }
        } else
        {
          glm::dvec3 dir = glm::normalize(body.pos - pos);

          double gravityPower = BIG_G * ((body.mass*mass)/(dis*dis));
          glm::dvec3 force = dir * gravityPower;

	        force *= glm::pow(mass, -1.0f);

          vel += force * precision;
        }
      }
    }

    void updatePos() {
      if (!active) return;
      pos += vel * precision;
    }

    void draw(sf::RenderTarget& SCREEN) {
      if (active) {
        sprite.setFillColor(sf::Color(color[0]*255.0f, color[1]*255.0f, color[2]*255.0f));
      } else {
        sprite.setFillColor(sf::Color(255, 0, 0, 128));
      }

      sf::View cam = SCREEN.getView();
      if (SCREEN.getViewport(cam).contains(SCREEN.mapCoordsToPixel(sf::Vector2f(pos.x, pos.z)))) {
        sprite.setRadius(radius);
        sprite.setPointCount(30UL);
        sprite.setPosition(sf::Vector2f(pos.x, pos.z));
      } else {
        sf::Vector2f center = cam.getCenter();
        glm::vec3 dir = glm::normalize(pos - glm::dvec3(center.x, 0.0f, center.y));

        sprite.setRadius(cam.getSize().x/128.0);
        sprite.setPointCount(4UL);
        sprite.setPosition(SCREEN.mapPixelToCoords(sf::Vector2i(256 + dir.x*250, 256 + dir.z*250)));
      }

      sprite.setOrigin(sf::Vector2f(sprite.getRadius(), sprite.getRadius()));

      SCREEN.draw(sprite);
    }
  private:
    sf::CircleShape sprite;
};

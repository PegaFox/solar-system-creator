#ifndef PEGAFOX_UTILS_HPP
#define PEGAFOX_UTILS_HPP

#include "../glm/glm/glm.hpp"
#include <chrono>

namespace pf {

  std::string floatToStr(double value, std::size_t decimalPlaces = -1) {

    if (decimalPlaces < 20UL) {
      std::size_t sizeMult = glm::pow(10, decimalPlaces);

      value = glm::floor(value*sizeMult) / sizeMult;
    }

    std::string str = std::to_string(value);
    if (str.find('.') != std::string::npos) {
      str.erase(str.find_last_not_of('0') + 1);
      str.erase(str.find_last_not_of('.') + 1);
    }

    return str;
  }

  class FPS {
    public:
      float fps = 0.0f;
      int frameCount = 0;
      float deltaTime = 0.0f;

      float get_fps() {
        auto frameTime = std::chrono::high_resolution_clock::now();
        auto time = frameTime - lastFrameTime;
        lastFrameTime = frameTime;

        fps = 1000000000.0f/std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();

        deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()/1000000000.0f;

        frameCount++;
        return fps;
      }

      // NOTE: must be run once every loop cycle with get_fps() alongside it
      void cap_fps(double maxFps) {
        std::chrono::high_resolution_clock::time_point step;
        std::chrono::milliseconds currentTime;
        do {
          step = std::chrono::high_resolution_clock::now();
          currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(step - lastFrameTime);
        } while (currentTime.count() <= 1000.0 / maxFps);
      }
    private:
      std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
  };

  double radAdjust(double radians) {
    while (radians > M_PI*2)
      radians -= M_PI*2;
    while (radians < 0)
      radians += M_PI*2;
    return radians;
  }

  double getAngle(double x1, double y1, double x2, double y2) {
    return radAdjust(atan2(y1-y2, x1-x2));
  }

  double adjustRange(double value, double min1, double max1, double min2, double max2) {
    double x = (value - min1) / (max1 - min1);
    return min2 + (max2 - min2) * x;
  }

  inline glm::vec2 vector2fToVec2(sf::Vector2f vec) {
    return glm::vec2(vec.x, vec.y);
  }
}

#endif // PEGAFOX_UTILS_HPP
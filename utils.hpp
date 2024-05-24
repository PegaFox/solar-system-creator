#ifndef PEGAFOX_UTILS_HPP
#define PEGAFOX_UTILS_HPP

#include <string>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <iterator>
#include <cstddef>

#ifdef __linux__

#include <unistd.h>

#endif // __linux__
#ifdef __FreeBSD__

#endif // __FreeBSD__
#ifdef __ANDROID__

#endif // __ANDROID__
#ifdef __APPLE__

#endif // __APPLE__
#ifdef _WIN32
#ifdef _WIN64
  
#else

#endif // _WIN64

#endif // _WIN32

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

  glm::vec2 lineClosestPoint(const glm::vec2& line1, const glm::vec2& line2, const glm::vec2& point) {
    glm::vec2 ap = point - line1;
    glm::vec2 ab_dir = line2 - line1;
    float dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
    if (dot < 0.0f)
        return line1;
    float ab_len_sqr = ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y;
    if (dot > ab_len_sqr)
        return line2;
    return line1 + ab_dir * dot / ab_len_sqr;
  }

  glm::vec2 lineClosestPoint(float line1X, float line1Y, float line2X, float line2Y, float pointX, float pointY) {
    return lineClosestPoint({line1X, line1Y}, {line2X, line2Y}, {pointX, pointY}); 
  }

  std::string intToHexStr(uint8_t in, const std::string& prefix = "0x") {
    std::string result = prefix;

    for (uint8_t nibble = 0; nibble < 2; nibble++) {
      uint8_t digit = (in & 0xF0) >> 4;
      if (digit < 10) {
        result.append(1, digit + 48);
      } else {
        result.append(1, digit + 55);
      }
      in <<= 4;
    }

    return result;
  }

  std::string intToHexStr(uint16_t in, const std::string& prefix = "0x") {
    std::string result = prefix;

    for (uint8_t nibble = 0; nibble < 4; nibble++) {
      uint8_t digit = (in & 0xF000) >> 12;
      if (digit < 10) {
        result.append(1, digit + 48);
      } else {
        result.append(1, digit + 55);
      }
      in <<= 4;
    }

    return result;
  }

  std::string intToHexStr(uint32_t in, const std::string& prefix = "0x") {
    std::string result = prefix;

    for (uint8_t nibble = 0; nibble < 8; nibble++) {
      uint8_t digit = (in & 0xF0000000) >> 28;
      if (digit < 10) {
        result.append(1, digit + 48);
      } else {
        result.append(1, digit + 55);
      }
      in <<= 4;
    }

    return result;
  }

  std::string intToHexStr(uint64_t in, const std::string& prefix = "0x") {
    std::string result = prefix;

    for (uint8_t nibble = 0; nibble < 16; nibble++) {
      uint8_t digit = (in & 0xF000000000000000) >> 60;
      if (digit < 10) {
        result.append(1, digit + 48);
      } else {
        result.append(1, digit + 55);
      }
      in <<= 4;
    }

    return result;
  }

  uint64_t hexStrToInt(std::string hex) {
    if (hex[1] == 'x' || hex[1] == 'X') {
      hex.erase(0UL, 2);
    }

    uint64_t result = 0;
    uint64_t digit = 1;

    while (!hex.empty()) {
      if (hex.back() > 47 && hex.back() < 58) {
        result += (hex.back()-48) * digit;
      } else if (hex.back() > 64 && hex.back() < 71) {
        result += (hex.back()-55) * digit;
      }

      digit *= 16;
      hex.pop_back();
    }

    return result;
  }

  uint64_t octStrToInt(std::string oct) {
    uint64_t result = 0;
    uint64_t digit = 1;

    while (!oct.empty()) {
      result += (oct.back() - 48) * digit;

      digit *= 8;
      oct.pop_back();
    }

    return result;
  }

  uint64_t binStrToInt(std::string bin) {
    if (bin[1] == 'b' || bin[1] == 'B') {
      bin.erase(0UL, 2);
    }

    uint64_t result = 0;
    uint64_t digit = 1;

    while (!bin.empty()) {
      if (bin.back() == '1') {
        result += digit;
      }

      digit *= 2;
      bin.pop_back();
    }

    return result;
  }
  
  double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
  }

  double distance(glm::vec2 pos1, glm::vec2 pos2) {
    return distance(pos1.x, pos1.y, pos2.x, pos2.y);
  }

  double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2)+pow(z1-z2, 2));
  }

  double distance(glm::vec3 pos1, glm::vec3 pos2) {
    return distance(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
  }

  glm::vec2 normalize(double x, double y) {
    double dis = distance(0, 0, x, y);
    if (dis == 0.0) {
      return {0.0, 0.0};
    }
    return {x / dis, y / dis};
  }

  glm::vec2 normalize(glm::vec2 pos) {
    return normalize(pos.x, pos.y);
  }

  glm::vec3 normalize(double x, double y, double z) {
    double dis = distance(0, 0, 0, x, y, z);
    if (dis == 0.0) {
      return {0.0, 0.0, 0.0};
    }
    return {x / dis, y / dis, z / dis};
  }

  glm::vec3 normalize(glm::vec3 pos) {
    return normalize(pos.x, pos.y, pos.z);
  }

  double dotProduct(double x1, double y1, double x2, double y2) {
    return x1*x2 + y1*y2;
  }

  double dotProduct(glm::vec2 pos1, glm::vec2 pos2) {
    return dotProduct(pos1.x, pos1.y, pos2.x, pos2.y);
  }

  double dotProduct(double x1, double y1, double z1, double x2, double y2, double z2) {
    return x1*x2 + y1*y2 + z1*z2;
  }

  double dotProduct(glm::vec3 pos1, glm::vec3 pos2) {
    return dotProduct(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
  }

  glm::vec3 crossProduct(double x1, double y1, double z1, double x2, double y2, double z2) {
    return {y1*z2-z1*y2, z1*x2-x1*z2, x1*y2-y1*x2};
  }

  glm::vec3 crossProduct(glm::vec3 pos1, glm::vec3 pos2) {
    return crossProduct(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
  }

  glm::vec2 reflect(double dirX, double dirY, double normX, double normY) {
    double doubled = 2*dotProduct(dirX, dirY, normX, normY);

    return {dirX - doubled*normX, dirY - doubled*normY};
  }

  glm::vec2 reflect(glm::vec2 dir, glm::vec2 norm) {
    return reflect(dir.x, dir.y, norm.x, norm.y);
  }

  double logisticFunc(double num, double tilt) {
    return (2/(1+exp(-tilt*num)))-1;
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

  /*
    Records how long the struct is on the stack.
    If result is NULL, the time taken is printed to stderr
  */
  struct Timing {
    std::string name;
    std::chrono::system_clock::time_point start;

    Timing(const std::string& name, double* result = nullptr) {
      begin(name, result);
    }

    ~Timing() {
      end();
    }

    void begin(const std::string& name, double* result = nullptr)
    {
      this->name = name;
      this->result = result;
      start = std::chrono::high_resolution_clock::now();
    }

    double end()
    {
      std::chrono::system_clock::time_point end = std::chrono::high_resolution_clock::now();
      std::chrono::nanoseconds elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
      double elapsedMilliseconds = elapsed.count() * 0.000001;
      if (result == nullptr) {
        std::clog << "code " << name << " took " << elapsedMilliseconds << " milliseconds\n";
      } else {
        *result = elapsedMilliseconds;
      }
      return elapsedMilliseconds;
    }
    private:
      double* result;
  };

  std::string getFileText(const std::string& filename) {
    std::ifstream file(filename);

    std::stringstream fileText;

    fileText << file.rdbuf();

    file.close();

    return fileText.str();
  }

  uint64_t splitMix64Hash(uint64_t seed) {
    uint64_t hashed = (seed + UINT64_C(0x9E3779B97F4A7C15));
    hashed = (hashed ^ (hashed >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    hashed = (hashed ^ (hashed >> 27)) * UINT64_C(0x94D049BB133111EB);
    return hashed ^ (hashed >> 31);
  }

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

  double getAngle(glm::vec2 pos1, glm::vec2 pos2) {
    return getAngle(pos1.x, pos1.y, pos2.x, pos2.y);
  }

  double adjustRange(double value, double min1, double max1, double min2, double max2) {
    double x = (value - min1) / (max1 - min1);
    return min2 + (max2 - min2) * x;
  }

  double adjustRange(double value, glm::vec2 range1, glm::vec2 range2) {
    return adjustRange(value, range1.x, range1.y, range2.x, range2.y);
  }

  uint32_t HSLtoRGB(int hue, int saturation, int lightness) {
    float r, g, b, h, s, l; //this function works with floats between 0 and 1
    float temp1, temp2, tempr, tempg, tempb;
    h = (float)hue / 256.0;
    s = (float)saturation / 256.0;
    l = (float)lightness / 256.0;

    //If saturation is 0, the color is a shade of gray
    if(s == 0) r = g = b = l;
    //If saturation > 0, more complex calculations are needed
    else {
      //Set the temporary values
      if(l < 0.5) temp2 = l * (1 + s);
      else temp2 = (l + s) - (l * s);
      temp1 = 2 * l - temp2;
      tempr = h + 1.0 / 3.0;
      if(tempr > 1) tempr--;
      tempg = h;
      tempb = h - 1.0 / 3.0;
      if(tempb < 0) tempb++;

      //Red
      if(tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
      else if(tempr < 0.5) r = temp2;
      else if(tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
      else r = temp1;

      //Green
      if(tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
      else if(tempg < 0.5) g = temp2;
      else if(tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
      else g = temp1;

      //Blue
      if(tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
      else if(tempb < 0.5) b = temp2;
      else if(tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
      else b = temp1;
    }
    return (uint32_t(r * 255.0) << 16) | (uint32_t(g * 255.0) << 8) | uint32_t(b * 255.0);
  }

  int max(std::vector<double> input) {
    int largestInd;
    double largestNum;
    for (int i = 0; i < input.size(); i++) {
      if (input[i] > largestNum) {
        largestNum = input[i];
        largestInd = i;
      }
    }
    return largestInd;
  }

  template <typename num>
  std::vector<std::vector<num>> matrixMult(
    std::vector<std::vector<num>> matrix1, std::vector<std::vector<num>> matrix2) {
    std::vector<std::vector<num>> result;
    int size = matrix2.size();
    int width = matrix2[0].size();
    int height = matrix1.size();
    result.resize(height);
    for (int i = 0; i < height; i++) result[i].resize(width);

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        for (int w = 0; w < size; w++) {
          result[y][x] += matrix1[y][w]*matrix2[w][x];
        }
      }
    }

    return result;
  }

  void sleep(double seconds) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start).count() < seconds * 1000000000.0) {

    }
  }

  bool keyPressed();

  #ifdef __linux__

  bool keyPressed() {
    /*uint8_t data[256];
    read(fileno(stdin), data, 256);
    select();
    std::cout << data << '\n';*/
    return false;
  }

  #endif // __linux__
  #ifdef __FreeBSD__
  bool keyPressed() {
    return false;
  }
  #endif // __FreeBSD__
  #ifdef __ANDROID__
  bool keyPressed() {
    return false;
  }
  #endif // __ANDROID__
  #ifdef __APPLE__
  bool keyPressed() {
    return false;
  }
  #endif // __APPLE__
  #ifdef _WIN32
  #ifdef _WIN64

  #else

  #endif // _WIN64
  bool keyPressed() {
    return false;
  }
  #endif // _WIN32

  // stl-like vector class that allocates to the stack
  template <typename _Tp, std::size_t _Nm>
  class StackVector {
    public:
  
      struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = _Tp;
        using pointer = _Tp*;
        using reference = _Tp&;
  
        iterator(pointer ptr) : elem{ptr} {
          
        }
  
        reference operator*() const { return *elem; }
        pointer operator->() { return elem; }
  
        iterator& operator++() { elem++; return *this; }  
  
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        bool operator<(const iterator& i) const {
          return elem < i.elem;
        }

        bool operator<=(const iterator& i) const {
          return elem <= i.elem;
        }

        bool operator>(const iterator& i) const {
          return elem > i.elem;
        }

        bool operator>=(const iterator& i) const {
          return elem >= i.elem;
        }

        bool operator==(const iterator& i) const {
          return elem == i.elem;
        }

        bool operator!=(const iterator& i) const {
          return elem != i.elem;
        }

        iterator operator-(const iterator& i) const {
          return elem - i.elem;
        }
  
        private:
          pointer elem;
      };
  
      struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = _Tp;
        using pointer = const _Tp*;
        using reference = const _Tp&;
  
        const_iterator(pointer ptr) : elem{ptr} {
          
        }
  
        reference operator*() const { return *elem; }
        pointer operator->() { return elem; }
  
        const_iterator& operator++() { elem++; return *this; }  
  
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }

        bool operator<(const const_iterator& i) const {
          return elem < i.elem;
        }

        bool operator<=(const const_iterator& i) const {
          return elem <= i.elem;
        }

        bool operator>(const const_iterator& i) const {
          return elem > i.elem;
        }

        bool operator>=(const const_iterator& i) const {
          return elem >= i.elem;
        }

        bool operator==(const const_iterator& i) const {
          return elem == i.elem;
        }

        bool operator!=(const const_iterator& i) const {
          return elem != i.elem;
        }

        const_iterator operator-(const const_iterator& i) const {
          return elem - i.elem;
        }
  
        private:
          pointer elem;
      };
  
      struct reverse_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = _Tp;
        using pointer = _Tp*;
        using reference = _Tp&;
  
        reverse_iterator(pointer ptr) : elem{ptr} {
          
        }
  
        reference operator*() const { return *elem; }
        pointer operator->() { return elem; }
  
        reverse_iterator& operator++() { elem++; return *this; }  
  
        reverse_iterator operator++(int) { reverse_iterator tmp = *this; ++(*this); return tmp; }
  
        reverse_iterator& operator--() { elem--; return *this; }  
  
        reverse_iterator operator--(int) { reverse_iterator tmp = *this; --(*this); return tmp; }

        bool operator<(const reverse_iterator& i) const {
          return elem < i.elem;
        }

        bool operator<=(const reverse_iterator& i) const {
          return elem <= i.elem;
        }

        bool operator>(const reverse_iterator& i) const {
          return elem > i.elem;
        }

        bool operator>=(const reverse_iterator& i) const {
          return elem >= i.elem;
        }

        bool operator==(const reverse_iterator& i) const {
          return elem == i.elem;
        }

        bool operator!=(const reverse_iterator& i) const {
          return elem != i.elem;
        }

        reverse_iterator operator-(const reverse_iterator& i) const {
          return elem - i.elem;
        }
  
        private:
          pointer elem;
      };
  
      struct const_reverse_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = _Tp;
        using pointer = const _Tp*;
        using reference = const _Tp&;
  
        const_reverse_iterator(pointer ptr) : elem{ptr} {
          
        }
  
        reference operator*() const { return *elem; }
        pointer operator->() { return elem; }
  
        const_reverse_iterator& operator++() { elem++; return *this; }  
  
        const_reverse_iterator operator++(int) { const_reverse_iterator tmp = *this; ++(*this); return tmp; }
  
        const_reverse_iterator& operator--() { elem--; return *this; }  
  
        const_reverse_iterator operator--(int) { const_reverse_iterator tmp = *this; --(*this); return tmp; }

        bool operator<(const const_reverse_iterator& i) const {
          return elem < i.elem;
        }

        bool operator<=(const const_reverse_iterator& i) const {
          return elem <= i.elem;
        }

        bool operator>(const const_reverse_iterator& i) const {
          return elem > i.elem;
        }

        bool operator>=(const const_reverse_iterator& i) const {
          return elem >= i.elem;
        }

        bool operator==(const const_reverse_iterator& i) const {
          return elem == i.elem;
        }

        bool operator!=(const const_reverse_iterator& i) const {
          return elem != i.elem;
        }

        const_reverse_iterator operator-(const const_reverse_iterator& i) const {
          return elem - i.elem;
        }
  
        private:
          pointer elem;
      };
  
      StackVector() {
      
      }
      
      StackVector(std::size_t count, const _Tp& value) : count{count} {
        std::fill(values, values+this->count, value);
      }
  
      StackVector(std::size_t count) : count{count} {
        
      }
  
      template<class input_iterator>
      StackVector(input_iterator first, input_iterator last) {
        count = last-first;
        std::copy(first, last, values);
      }
  
      StackVector(const StackVector& other) {
        count = other.size();
  
        if (count > _Nm) {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        std::copy(other.cbegin(), other.cend(), values);
      }
  
      StackVector(StackVector&& other) {
        count = other.size();
  
        if (count > _Nm) {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
        
        std::copy(other.cbegin(), other.cend(), values);
    
      }
  
      StackVector(std::initializer_list<_Tp> init) {
        count = init.size();
  
        if (count > _Nm) {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        std::copy(init.begin(), init.end(), values);
      }

      StackVector& operator=(const StackVector& other) {
        if (this!= &other) {
          count = other.size();

          if (count > _Nm) {
            throw std::out_of_range("pf::StackVector, size exceeded maximum length");
          }

          std::copy(other.cbegin(), other.cend(), values);
        }

        return *this;
      }

      _Tp& at(std::size_t pos) {
        if (pos >= 0 && pos < count) {
          return values[pos];
        }
        
        throw std::out_of_range("pf::StackVector, size exceeded maximum length");
      }
  
      const _Tp& at(std::size_t pos) const {
        if (pos >= 0 && pos < count) {
          return values[pos];
        }
        
        throw std::out_of_range("pf::StackVector, size exceeded maximum length");
      }
  
      _Tp& operator[](std::size_t pos) {
        return values[pos];
      }
  
      const _Tp& operator[](std::size_t pos) const {
        return values[pos];
      }
  
      _Tp& front() {
        return values[0];
      }
  
      const _Tp& front() const {
        return values[0];
      }
  
      _Tp& back() {
        return values[count-1];
      }
  
      const _Tp& back() const {
        return values[count-1];
      }
  
      _Tp* data() {
        return values;
      }
  
      const _Tp* data() const {
        return values;
      }
  
      iterator begin() {
        return iterator(values);
      }
  
      const_iterator cbegin() const noexcept {
        return const_iterator(values);
      }
  
      iterator end() {
        return iterator(values+count);
      }
  
      const_iterator cend() const noexcept {
        return const_iterator(values+count);
      }
  
      reverse_iterator rbegin() {
        return reverse_iterator(values);
      }
  
      const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(values);
      }
  
      reverse_iterator rend() {
        return reverse_iterator(values+count);
      }
  
      const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(values+count);
      }
  
      bool empty() const {
        return count == 0;
      }
  
      std::size_t size() const {
        return count;
      }
  
      std::size_t max_size() const {
        return _Nm;
      }
  
      std::size_t capacity() const {
        return _Nm;
      }
  
      void clear() {
        count = 0;
      }
  
      iterator insert(const_iterator pos, const _Tp& value) {
        if (count < _Nm) {
          std::move_backward(&*pos, values+count, values+count+1);
          count++;
          *(&*pos) = value;
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
        
        return iterator(&*pos);
      }
  
      iterator insert(const_iterator pos, _Tp&& value) {
        if (count < _Nm) {
          std::move_backward(&*pos, values+count, values+count+1);
          count++;
          *(&*pos) = value;
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        return iterator(&*pos);
      }
  
      iterator insert(const_iterator pos, std::size_t count, const _Tp& value) {
        if (this->count+count <= _Nm) {
          std::move_backward(&*pos, values+this->count, values+this->count+count);
          this->count += count;
          std::fill(&*pos, ((_Tp*)(&*pos))+count, value);
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        return iterator(&*pos);
      }
  
      template<class input_iterator>
      iterator insert(const_iterator pos, input_iterator first, input_iterator last) {
        std::size_t count = ((_Tp*)(&*last)) - ((_Tp*)(&*first));
        if (this->count+count <= _Nm) {
          std::move_backward(&*pos, values+this->count, values+this->count+count);
          this->count += count;
          std::copy(first, last, &*pos);
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        return iterator(&*pos);
      }
  
      iterator insert(const_iterator pos, std::initializer_list<_Tp> values) {
        if (count+values.size() <= _Nm) {
          std::move_backward(&*pos, this->values+count, this->values+count+values.size());
          count += values.size();
          std::copy(values.begin(), values.end(), &*pos);
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        return iterator(&*pos);
      }
  
      // not yet implemented
      template<class... Args>
      iterator emplace(const_iterator pos, Args&&... args) {
        iterator item((&*begin()) + ((&*pos) - (&*begin())));

        if (count < _Nm) {
          std::move_backward(&*item, values+count, values+count+1);
          count++;
          *(&*item) = _Tp(args...);
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        return item;
      }
  
      iterator erase(iterator pos) {
        if (count > 0) {
          (*pos).~_Tp();
          std::move(((_Tp*)(&*pos))+1, values+count, pos);
          count--;
        }
  
        return iterator(&*pos);
      }
  
      iterator erase(const_iterator pos) {
        if (count > 0) {
          (*pos).~_Tp();
          std::move(((_Tp*)(&*pos))+1, values+count, pos);
          count--;
        }
  
        return iterator(&*pos);
      }
  
      iterator erase(iterator first, iterator last) {
      
        if (&*first < values || &*last > values+count) {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        for (iterator value = first; value < last; value++) {
          (*value).~_Tp();
        }
  
        std::size_t count = ((_Tp*)(&*last)) - ((_Tp*)(&*first));
        std::move(last, values+count, first);
        count -= count;
  
        return iterator(&*last);
      }
  
      iterator erase(const_iterator first, const_iterator last) {
        if (&*first < values || &*last > values+count) {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        for (iterator value = first; value < last; value++) {
          (*value).~_Tp();;
        }
  
        std::size_t count = ((_Tp*)(&*last)) - ((_Tp*)(&*first));
        std::move(last, values+count, first);
        count -= count;
  
        return iterator(&*last);
      }
  
      void push_back(const _Tp& value) {
        if (count < _Nm) {
          values[count] = value;
          count++;
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
      }
  
      void push_back(_Tp&& value) {
        if (count < _Nm) {
          values[count] = value;
          count++;
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
      }
  
      // not yet implemented
      template<class... Args>
      _Tp& emplace_back(Args&&... args) {
        return *emplace(cend(), args...);
      }
  
      void pop_back() {
        count--;
        (values[count]).~_Tp();
      }
  
      void resize(std::size_t count) {
        if (count < _Nm) {
          this->count = count;
        } else {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
      }
  
      // not yet implemented
      void swap(StackVector& other) {
        count = other.size();
  
        if (count > _Nm) {
          throw std::out_of_range("pf::StackVector, size exceeded maximum length");
        }
  
        std::copy(other.cbegin(), other.cend(), values);
      }
  
    private:
  
      _Tp values[_Nm];
      std::size_t count = 0;
  };

}

#endif // PEGAFOX_UTILS_HPP
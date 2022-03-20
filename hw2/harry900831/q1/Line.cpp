#include "Line.h"
#include <stdexcept>


Line::Line(Line const& other) : p(other.p) {}

Line::Line(Line&& other) {
  p = std::move(other.p);
}

Line& Line::operator=(Line const& other) {
  if (this == &other) return *this;
  p = other.p;
  return *this;
}

Line& Line::operator=(Line&& other) {
  if (this == &other) return *this;
  p = std::move(other.p);
  return *this;
}

Line::Line(size_t size) : p(size) {}

size_t Line::size() const {
  return p.size();
}

const float& Line::x(size_t it) const {
  check_range(it);
  return p[it].x;
}

float& Line::x(size_t it) {
  check_range(it);
  return p[it].x;
}

const float& Line::y(size_t it) const {
  check_range(it);
  return p[it].y;
}

float& Line::y(size_t it) {
  check_range(it);
  return p[it].y;
}

void Line::check_range(size_t it) const {
  if (it >= p.size()) {
    throw std::out_of_range("Line index out of range");
  }
}

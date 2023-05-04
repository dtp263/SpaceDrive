#ifndef UTILS_H
#define UTILS_H

int ConvertToScale(int inputScale, int outputScale, int value) {
  float conversion = float(value) / float(inputScale);
  return conversion * outputScale;
}

#endif

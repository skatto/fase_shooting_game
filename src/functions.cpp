

#define FASE_USE_ADD_FUNCTION_BUILDER_MACRO
#include <fase/fase.h>

#include "game.h"

FaseAutoAddingFunctionBuilder(Square, void Square(const float& in, float& dst) {
  dst = in * in;
});

FaseAutoAddingFunctionBuilder(Max, void Max(const float& in1, const float& in2,
                                            float& dst) {
  dst = in1 > in2 ? in1 : in2;
});

FaseAutoAddingFunctionBuilder(Min, void Min(const float& in1, const float& in2,
                                            float& dst) {
  dst = in1 < in2 ? in1 : in2;
});

FaseAutoAddingFunctionBuilder(Cross,
                              void Cross(const float& in1, const float& in2,
                                         float& dst) { dst = in1 * in2; });

FaseAutoAddingFunctionBuilder(Add, void Add(const float& in1, const float& in2,
                                            float& dst) { dst = in1 + in2; });

FaseAutoAddingFunctionBuilder(Diff,
                              void Diff(const float& in1, const float& in2,
                                        float& dst) { dst = in1 - in2; });

FaseAutoAddingFunctionBuilder(KeyPressTime,
                              void KeyPressTime(int key, float& time,
                                                const KeyPressStorer& storer) {
                                if (storer.pressed_map.count(key)) {
                                  using namespace std::chrono;
                                  time = duration_cast<milliseconds>(
                                             system_clock::now() -
                                             storer.pressed_map.at(key))
                                             .count() /
                                         1000.f;
                                } else {
                                  time = 0.f;
                                }
                              });

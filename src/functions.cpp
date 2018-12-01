
#define FASE_USE_ADD_FUNCTION_BUILDER_MACRO
#include <fase/fase.h>

FaseAutoAddingFunctionBuilder(Square, void Square(const int& in, int& dst) {
  dst = in * in;
});

/**
 * Goal: Describe the different possible method signatures.
 */
class Vector {};

class VectorMethods {
  // Standard function.
  int foo1(Vector v);

  // Standard pass by reference.
  int foo2(Vector& v);

  // Standard pass by pointer.
  int foo3(Vector* v);

  /* v is a Vector passed by value and cannot be modified. */
  int foo4(const Vector v);
  /* v is a Vector passed by reference and cannot be modified. */
  int foo5(const Vector& v);
  /** 
   * v is a Vector passed by pointer and the pointer itself cannot be modified.
   * However, the data point to by the pointer CAN be changed. See the
   * double-const below.
   */
  int foo6(const Vector* v);

  /* Pointer and the data pointed to cannot be changed */
  int foo7(const Vector* const v);

  // NOT valid, cant apply the inner const to a reference type, only pointer.
  // int foo(const Vector& const v);

  /**
   * When the return type is by value, const declares that the return type
   * cannot be used as an lvalue (left-hand side of an expression). Marking a
   * return type as const makes no sense for primitive built-in types, because
   * built-int types cannot appear on the LHS of an expression anyway.
   *
   * This example would not be allowed:
   * foo2() = ...
   *
   * Note, method name is different because the outter const does not change
   * the overload signature.
   */
  const int foo8(Vector v);

  /** 
   * This const declares that the method cannot change any private members of
   * the class. 
   */
  int foo9(Vector v) const;

  // All together now!
  const int foo10(const Vector* const v) const;
};

int main() {
  return 0;
}
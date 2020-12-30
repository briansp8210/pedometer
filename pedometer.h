#include <array>
#include <vector>

struct IirCoeff {
  std::array<double, 3> alpha;
  std::array<double, 3> beta;
};

class IirFilter {
 public:
  std::vector<double> Filter(const std::vector<double> &data,
                             const struct IirCoeff &coeff);

  static const struct IirCoeff coeffLowPass0HZ;
};

class Pedometer {
 public:
  // The constructor converts the given data set to User and Gravity
  // acceleration. It may be overloaded for input of different App.

  // Combined (User and Gravity) acceleration, with 3 dimensions separated.
  Pedometer(const std::array<std::vector<double>, 3> &data);

  std::array<std::vector<double>, 3> accUser, accGravity;
  // User acceleration component at gravity direction
  std::vector<double> accOrigin;

 private:
  void IsolateUserComponentAtGravityDirection(
      const std::array<std::vector<double>, 3> &accUser,
      const std::array<std::vector<double>, 3> &accGravity);

  IirFilter filter;
};

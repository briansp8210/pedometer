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
  static const struct IirCoeff coeffLowPass5HZ;
  static const struct IirCoeff coeffHighPass1HZ;
};

class Processor {
 public:
  void EliminateJumpyPeaks(std::vector<double> &acc);
  void EliminateSlowPeaks(std::vector<double> &acc);

 private:
  IirFilter filter;
};

class Pedometer {
 public:
  // The constructor converts the given data set to User and Gravity
  // acceleration. It may be overloaded for input of different App.

  // Combined (User and Gravity) acceleration, with 3 dimensions separated.
  Pedometer(const std::array<std::vector<double>, 3> &data);

  void Process();
  size_t CountSteps();

  // The frontend will uses these membersto showcase the intermediate results
  std::array<std::vector<double>, 3> accUser, accGravity;
  std::vector<double> accOrigin, accNoJumpyPeaks, accNoSlowPeaks;

  std::vector<double> acc;

 private:
  // Used to eliminate short peaks. This could happen when user using their
  // device while walking.
  static constexpr double kStepAccelerationHighThreShold = 0.1;
  // Used to eliminate bumpy peaks.
  static constexpr double kStepAccelerationLowThreShold = 0;

  void IsolateUserComponentAtGravityDirection(
      const std::array<std::vector<double>, 3> &accUser,
      const std::array<std::vector<double>, 3> &accGravity);

  IirFilter filter;
  Processor processor;
};

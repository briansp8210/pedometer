#include "pedometer.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <complex>
#include <numeric>

namespace py = pybind11;

const struct IirCoeff IirFilter::coeffLowPass0HZ = {
    {1, -1.979133761292768, 0.979521463540373},
    {0.000086384997973502, 0.000172769995947004, 0.000086384997973502}};

std::vector<double> IirFilter::Filter(const std::vector<double> &data,
                                      const struct IirCoeff &coeff) {
  std::vector<double> result(data.size());
  result[0] = result[1] = 0.0;

  for (size_t i = 2; i < data.size(); ++i) {
    result[i] = coeff.alpha[0] *
                (data[i] * coeff.beta[0] + data[i - 1] * coeff.beta[1] +
                 data[i - 2] * coeff.beta[2] - result[i - 1] * coeff.alpha[1] -
                 result[i - 2] * coeff.alpha[2]);
  }
  return result;
}

// Combined (User and Gravity) acceleration, with 3 dimensions separated.
Pedometer::Pedometer(const std::array<std::vector<double>, 3> &data) {
  size_t sampleNums = data[0].size();
  accUser.fill(std::vector<double>(sampleNums));
  accGravity.fill(std::vector<double>(sampleNums));

  for (int dim = 0; dim < 3; ++dim) {
    accGravity[dim] = filter.Filter(data[dim], IirFilter::coeffLowPass0HZ);
    for (size_t i = 0; i < sampleNums; ++i) {
      accUser[dim][i] = data[dim][i] - accGravity[dim][i];
    }
  }
  IsolateUserComponentAtGravityDirection(accUser, accGravity);
}

void Pedometer::IsolateUserComponentAtGravityDirection(
    const std::array<std::vector<double>, 3> &accUser,
    const std::array<std::vector<double>, 3> &accGravity) {
  size_t sampleNums = accUser[0].size();
  accOrigin.reserve(sampleNums);
  for (size_t i = 0; i < sampleNums; ++i) {
    // auto gravity = std::sqrt(std::pow(accGravity[0][i], 2) +
    //                          std::pow(accGravity[1][i], 2) +
    //                          std::pow(accGravity[2][i], 2));
    accOrigin.push_back(accUser[0][i] * accGravity[0][i] +
                        accUser[1][i] * accGravity[1][i] +
                        accUser[2][i] * accGravity[2][i]);
  }
}

PYBIND11_MODULE(_pedometer, m) {
  py::class_<Pedometer>(m, "Pedometer")
      .def(py::init<const std::array<std::vector<double>, 3> &>())
      .def_readonly("accUser", &Pedometer::accUser)
      .def_readonly("accGravity", &Pedometer::accGravity)
      .def_readonly("accOrigin", &Pedometer::accOrigin);
}
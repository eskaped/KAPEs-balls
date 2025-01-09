#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include "doctest.h"
#include <cstring> //for strcmp()
#include <iostream>

TEST_CASE("Testing ParticleType")
{
  SUBCASE("Testing Getter methods")
  {
    kape::ParticleType a{"a", 0.5, 1};
    CHECK(std::strcmp(a.GetName(), "a") == 0);
    CHECK(a.GetMass() == 0.5);
    CHECK(a.GetCharge() == 1);
  }

  SUBCASE("Testing Print method")
  {
    kape::ParticleType a{"a", 0.5, 1};

    std::cout << "\nPlease check that the 2 printed outputs are the same: \n\n";
    std::cout << "Name:\ta\n";
    std::cout << "Mass:\t0.5\n";
    std::cout << "Charge:\t1\n\n";
    a.Print();
  }
}

TEST_CASE("Testing ResonanceType")
{
  SUBCASE("Testing Getter methods")
  {
    kape::ResonanceType b{"b", 0.5, 1, 1.};
    CHECK(std::strcmp(b.GetName(), "b") == 0);
    CHECK(b.GetMass() == 0.5);
    CHECK(b.GetCharge() == 1);
    CHECK(b.GetWidth() == 1.);
  }

  SUBCASE("Testing Print method")
  {
    kape::ResonanceType b{"b", 0.5, 1, 1.};

    std::cout << "\nPlease check that the 2 printed outputs are the same: \n\n";
    std::cout << "Name:\tb\n";
    std::cout << "Mass:\t0.5\n";
    std::cout << "Charge:\t1\n";
    std::cout << "Width:\t1\n\n";
    b.Print();
  }

  SUBCASE("Testing Print override")
  {
    kape::ParticleType* particles[2];
    particles[0] = new kape::ParticleType("ParticleType", 0.5, 1);
    particles[1] = new kape::ResonanceType("ResonanceType", 1., -1, 1.);

    std::cout << "\nPlease check that the 2 printed outputs are the same: \n\n";
    std::cout << "Name:\tParticleType\n";
    std::cout << "Mass:\t0.5\n";
    std::cout << "Charge:\t1\n";

    std::cout << "Name:\tResonanceType\n";
    std::cout << "Mass:\t1\n";
    std::cout << "Charge:\t-1\n";
    std::cout << "Width:\t1\n\n";

    for (int i = 0; i < 2; i++) {
      particles[i]->Print();
    }
  }
}
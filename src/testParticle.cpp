#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Particle.hpp"

#include "doctest.h"

TEST_CASE("Testing Particle class")
{
  SUBCASE("Testing AddParticleType()")
  {
    CHECK(kape::Particle::GetNParticleType() == 0);
    kape::Particle::AddParticleType("kape", 70., -1);
    CHECK(kape::Particle::GetNParticleType() == 1);
    kape::Particle::AddParticleType("samu", 63., 4, 1.);
    CHECK(kape::Particle::GetNParticleType() == 2);
    kape::Particle::AddParticleType("lele", 56., 18);
    CHECK(kape::Particle::GetNParticleType() == 3);
    kape::Particle::AddParticleType("nick", 79., -100, 0.40);
    CHECK(kape::Particle::GetNParticleType() == 4);

    kape::Particle gebbi{"kape", 10., -3., 0.};
    CHECK(gebbi.GetIndex() == 0);
    CHECK(gebbi.GetMass() == 70.);
    gebbi.Print();
    gebbi.SetIndex(1);
    CHECK(gebbi.GetIndex() == 1);
    CHECK(gebbi.GetMass() == 63.);
    gebbi.Print();
  
    kape::Particle::AddParticleType("nick", 79., +100, 0.40);
    CHECK(kape::Particle::GetNParticleType() == 4);

    kape::Particle::PrintParticleType();
  }
}
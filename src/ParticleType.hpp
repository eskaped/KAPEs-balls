#ifndef PARTICLE_TYPE_HPP
#define PARTICLE_TYPE_HPP

class ParticleType
{
public:
    ParticleType();
    ParticleType(const char * name, double mass, int charge);

    const char* GetName() const;
    double GetMass() const;
    int GetCharge() const;
private:
    const char *fName;
    const double fMass;
    const int fCharge;
};
#endif
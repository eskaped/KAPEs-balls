#ifndef PARTICLE_TYPE_HPP
#define PARTICLE_TYPE_HPP

namespace kape
{
    class ParticleType
    {
    public:
        ParticleType(const char *name, double mass, int charge);
        ParticleType();
        virtual ~ParticleType();

        const char *GetName() const;
        double GetMass() const;
        int GetCharge() const;

        virtual void Print() const;


    private:
        const char *fName;
        const double fMass;
        const int fCharge;
    };
}

#endif
#ifndef RESONANCE_TYPE_HPP
#define RESONANCE_TYPE_HPP
#include "ParticleType.hpp"
namespace kape
{
    class ResonanceType : public ParticleType
    {
    public:
    ResonanceType(const char *name, double mass, int charge, double width);
    ResonanceType(); 
    
    double GetWidth() const;
    void Print() const override;

    private:
    const double fWidth;
    };
}

#endif
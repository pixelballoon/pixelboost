#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace pb
{
        
    class Easing
    {
    public:
        Easing();
        
        float Evaluate(float t);
        
        enum EaseType
        {
            kEaseTypeIn,
            kEaseTypeOut,
            kEaseTypeInOut,
        };
        
    public:
        static Easing Sin(EaseType type);
        static Easing Polynomial(EaseType type, int exponent);
        static Easing Bounce(EaseType type, int bounce = 15, float decayW = 0.3f, float delayH = 0.1f);
        static Easing Linear();
        
    private:
        enum EaseFunction
        {
            kEaseFunctionSin,
            kEaseFunctionPolynomial,
            kEaseFunctionBounce,
            kEaseFunctionLinear,
        };
        
        Easing(EaseFunction function, EaseType type = kEaseTypeInOut, float paramA = 0.f, float paramB = 0.f, float paramC = 0.f, float paramD = 0.f);
        
        float EvaluateSin(EaseType type, float t);
        float EvaluatePolynomial(EaseType type, float t);
        float EvaluateBounce(EaseType type, float t);
        float EvaluateElastic(EaseType type, float t);

        EaseFunction _Function;
        EaseType _Type;
        float _ParamA;
        float _ParamB;
        float _ParamC;
        float _ParamD;
    };
}

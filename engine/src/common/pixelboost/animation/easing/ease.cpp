#include "pixelboost/animation/easing/ease.h"

using namespace pb;

Easing::Easing()
    : Easing(kEaseFunctionLinear)
{
    
}

float Easing::Evaluate(float t)
{
    switch (_Function)
    {
        case kEaseFunctionLinear:
        {
            return t;
        }
        case kEaseFunctionSin:
        {
            return EvaluateSin(_Type, t);
        }
        case kEaseFunctionPolynomial:
        {
            return EvaluatePolynomial(_Type, t);
        }
        case kEaseFunctionBounce:
        {
            return EvaluateBounce(_Type, t);
        }
    }
}

Easing Easing::Sin(EaseType type)
{
    return Easing(kEaseFunctionSin, type);
}

Easing Easing::Polynomial(EaseType type, int exponent)
{
    return Easing(kEaseFunctionPolynomial, type, exponent);
}

Easing Easing::Bounce(EaseType type, int bounce, float decayW, float decayH)
{
    float coeff = 0.5f;
    
    for (int i = 1; i < bounce; i++)
    {
        coeff += glm::pow(decayW, (float)i);
    }
    
    return Easing(kEaseFunctionBounce, type, bounce, decayW, decayH, 1.0 / coeff);
}

Easing Easing::Linear()
{
    return Easing(kEaseFunctionLinear);
}

Easing::Easing(EaseFunction function, EaseType type, float paramA, float paramB, float paramC, float paramD)
{
    _Function = function;
    _Type = type;
    _ParamA = paramA;
    _ParamB = paramB;
    _ParamC = paramC;
    _ParamD = paramD;
}

float Easing::EvaluateSin(EaseType type, float t)
{
    switch (type)
    {
        case kEaseTypeIn:
        {
            return 1.f-glm::cos(t * M_PI_2);
        }
        case kEaseTypeOut:
        {
            return glm::sin(t * M_PI_2);
        }
        case kEaseTypeInOut:
        {
            if (t < 0.5f)
            {
                return (0.5f * glm::sin(M_PI * t));
            } else
            {
                return 1.f - (0.5f * sin(M_PI * t));
            }
        }
    }
}

float Easing::EvaluatePolynomial(EaseType type, float t)
{
    switch (type)
    {
        case kEaseTypeIn:
        {
            return glm::pow(t, _ParamA);
        }
        case kEaseTypeOut:
        {
            return 1.f - glm::pow(1.f - t, _ParamA);
        }
        case kEaseTypeInOut:
        {
            float p = t * 2.f;
            
            if (p < 1.0)
            {
                return glm::pow(p, _ParamA) * 0.5f;
            } else
            {
                p -= 2.0;
                
                if ((int)_ParamA % 2 == 0)
                {
                    return (glm::pow(p, _ParamA) - 2.f) * -0.5f;
                } else
                {
                    return (glm::pow(p, _ParamA) + 2.f) * 0.5f;
                }
            }
        }
    }
    
    return t;
}

float Easing::EvaluateBounce(EaseType type, float t)
{
    switch (type)
    {
        case kEaseTypeIn:
        {
            return 1.f - EvaluateBounce(kEaseTypeOut, 1.f - t);
        }
        case kEaseTypeOut:
        {
            float currentT = _ParamD * 0.5f;
            float prevT = currentT;
            float width = currentT;
            
            if (t <= currentT)
            {
                return glm::sin(-M_PI_2 + t * 1.0f/width * M_PI_2) + 1.0f;
            } else
            {
                for (int i = 1; i < _ParamA; i++)
                {
                    width = glm::pow(_ParamB, (float)i) * _ParamD;
                    currentT += width;
                    
                    if (t >= prevT && t < currentT)
                    {
                        float elastic = glm::sin((t-prevT) * (1.0f/width) * M_PI - M_PI) + 1.0f;
                        return 1.0f - ((1.0f - elastic) * glm::pow(_ParamC, (float)i));
                    }
                    
                    prevT = currentT;
                }
            }
            
            return 1.f;
        }
        case kEaseTypeInOut:
        {
            if (t < 0.5f)
            {
                return 0.5f * EvaluateBounce(kEaseTypeOut, t * 2.0f);
            } else
            {
                return 0.5f + 0.5f * EvaluateBounce(kEaseTypeIn, (t - 0.5f) * 2.0f);
            }
        }
    }
    
    return t;
}

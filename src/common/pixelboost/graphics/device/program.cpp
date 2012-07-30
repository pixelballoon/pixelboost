#include "pixelboost/debug/assert.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/graphics/device/program.h"

#ifndef PIXELBOOST_DISABLE_GRAPHICS

using namespace pb;

ShaderProgram::ShaderProgram()
{
    
}

ShaderProgram::~ShaderProgram()
{
    
}

void ShaderProgram::OnContextLost()
{
    PbAssert(!"This shader program does not support the loss of the graphics context");
}

#endif

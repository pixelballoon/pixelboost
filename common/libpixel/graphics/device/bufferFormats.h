#pragma once

namespace libpixel
{
    enum BufferFormat
    {
        kBufferFormatDynamic,
        kBufferFormatStatic,
    };
    
    enum VertexFormat
    {
        kVertexFormat_P_XY_RGBA,
        kVertexFormat_P_XYZ_UV,
        kVertexFormat_P_XYZ_RGBA,
        kVertexFormat_P_XYZ_RGBA_UV,
        kVertexFormat_NP_XYZ_UV,
    };
}

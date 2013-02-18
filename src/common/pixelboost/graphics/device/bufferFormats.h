#pragma once

namespace pb
{
    enum BufferFormat
    {
        kBufferFormatDynamic,
        kBufferFormatStatic,
    };
    
    enum VertexFormat
    {
        kVertexFormat_P3,
        kVertexFormat_P3_UV,
        kVertexFormat_P3_C4,
        kVertexFormat_P3_C4_UV,
        kVertexFormat_P3_N3_UV,
        kVertexFormat_P3_N3_UV_BW,
    };
}

//
//  Shader.fsh
//  test
//
//  Created by Jamie Hales on 03/06/2012.
//  Copyright (c) 2012 Pixel Balloon. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}

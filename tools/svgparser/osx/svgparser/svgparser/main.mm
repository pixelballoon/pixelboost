#import <AppKit/AppKit.h>

#include "svgParser.h"

int main (int argc, const char * argv[])
{
    [NSApplication sharedApplication];
    
    if (argc < 2)
        return 1;

    SvgParser parser;
    
    parser.Load(argv[1]);

    parser.ParseAll();
    
    parser.Export(argv[2]);
    
    return 0;
}

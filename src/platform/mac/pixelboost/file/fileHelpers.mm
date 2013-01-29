#ifdef PIXELBOOST_PLATFORM_OSX

#include <Foundation/Foundation.h>

#include "pixelboost/file/fileHelpers.h"

namespace pb
{
    
namespace FileHelpers
{
        
std::string GetRootPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] resourcePath] UTF8String];
    [pool release];
    return rootPath;
}
    
std::string GetUserPath()
{
    return "";
}
    
bool CreateDirectory(const std::string& directory)
{
    NSString* dirToCreate = [NSString stringWithUTF8String:directory.c_str()];
    NSError* error = nil;
    BOOL isDir=YES;
    
    if([[NSFileManager defaultManager] fileExistsAtPath:dirToCreate isDirectory:&isDir] || !isDir)
        return false;
    
    if(![[NSFileManager defaultManager] createDirectoryAtPath:dirToCreate withIntermediateDirectories:YES attributes:nil error:&error])
    {
        return false;
    }
    
    return true;
}

int GetTimestamp(const std::string& filename)
{
    NSDictionary* attrs = [[NSFileManager defaultManager] attributesOfItemAtPath:[NSString stringWithUTF8String:filename.c_str()] error:nil];
    
    if (attrs != nil)
    {
        NSDate *date = (NSDate*)[attrs objectForKey: NSFileModificationDate];
        return (int)[date timeIntervalSince1970];
    } 
    else {
        return 0;
    }
}
        
}
    
}

#endif
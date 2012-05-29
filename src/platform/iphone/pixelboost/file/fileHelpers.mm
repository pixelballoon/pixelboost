#ifdef PIXELBOOST_PLATFORM_IOS

#include "pixelboost/file/fileHelpers.h"

using namespace pb;

std::string pb::FileHelpers::GetRootPath()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    std::string rootPath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    [pool release];
    return rootPath;
}
    
std::string pb::FileHelpers::GetUserPath()
{
    NSArray* dirPaths;
    dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    
    return std::string([[dirPaths objectAtIndex:0] UTF8String]);
}

bool pb::FileHelpers::CreateDirectory(const std::string& directory)
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

int pb::FileHelpers::GetTimestamp(const std::string& filename)
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

#endif

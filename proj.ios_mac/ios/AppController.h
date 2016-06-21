#import <UIKit/UIKit.h>
#import "GameKitHelper.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate,GameKitHelperProtocol> {
    UIWindow *window;
}

//

@property (nonatomic, strong) UINavigationController *rootNavigationController;

@property (nonatomic, retain) UIWindow *window;

@end


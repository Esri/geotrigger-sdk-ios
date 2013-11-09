//
//  GTAppDelegate.h
//  ios-geotrigger-sample
//
//  Created by Ryan Arana on 8/31/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GTViewController;

@interface GTAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) GTViewController *viewController;

@end

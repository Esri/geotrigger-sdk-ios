//
//  GTAppDelegate.m
//  ios-geotrigger-sample
//
//  Created by Ryan Arana on 8/31/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import "GTAppDelegate.h"

#import "GTViewController.h"
#import <GeotriggerSDK/GeotriggerSDK.h>

@implementation GTAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.viewController = [[GTViewController alloc] initWithNibName:@"GTViewController" bundle:nil];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];

    // If we were launched from a push notification, send the payload to the Geotrigger Manager
    if (launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey] != nil) {
        [[AGSGTGeotriggerManager sharedManager] handlePushNotification:launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey]];
    }

    return YES;
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    NSLog(@"Failed to register for remote notifications with Apple: %@", [error debugDescription]);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"registerForRemoteNotificationsFailure" object:nil];
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    // Register the device token with this device on the Geotrigger Service.
    [[AGSGTGeotriggerManager sharedManager] registerAPNSDeviceToken:deviceToken
                                                      forProduction:NO
                                                         completion:^(NSDictionary *dictionary, NSError *error) {
                                                             if (error == nil) {
                                                                 [[NSNotificationCenter defaultCenter] postNotificationName:@"registerForRemoteNotificationsSuccess" object:nil];
                                                             } else {
                                                                 NSLog(@"Failed to register APNS device token: %@", error.userInfo);
                                                                 [[NSNotificationCenter defaultCenter] postNotificationName:@"registerForRemoteNotificationsFailure" object:nil];
                                                             }
                                                         }];
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    // Forward the push notification payload on to the manager.
    [[AGSGTGeotriggerManager sharedManager] handlePushNotification:userInfo];
    if ([userInfo[@"aps"][@"alert"] isEqualToString:@"Trigger fired!"]) {
        [[NSNotificationCenter defaultCenter] postNotificationName:@"pushNotificationReceived" object:nil];
    }
}
@end

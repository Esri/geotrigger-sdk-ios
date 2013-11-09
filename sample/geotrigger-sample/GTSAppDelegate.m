//
//  GTSAppDelegate.m
//  geotrigger-sample
//
//  Created by Ryan Arana on 11/8/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import "GTSAppDelegate.h"
#import <GeotriggerSDK/GeotriggerSDK.h>

@implementation GTSAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
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

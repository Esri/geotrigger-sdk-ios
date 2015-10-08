//
//  GTSAppDelegate.m
//  geotrigger-sample
//
//  Created by Ryan Arana on 11/8/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import "GTSAppDelegate.h"
#import <GeotriggerSDK/GeotriggerSDK.h>
#import <CoreLocation/CoreLocation.h>
#import "Constants.h"

@implementation GTSAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [[NSUserDefaults standardUserDefaults] setObject:@"No - not setup yet" forKey:@"managerReadyText"];
    // Enable debug logs to the console. This spits out a lot of logs so you probably don't want to do this in a release
    // build, but it is good for helping track down any problems you may encounter.
    [AGSGTGeotriggerManager setLogLevel:AGSGTLogLevelDebug];

    // Set up the manager singleton with your clientId and any tracking profile you pass in here
    // will be used to start the manager when it is ready. You can also pass in UIRemoteNotificationTypes
    // to have the manager register your application for with Apple when it is ready to receive an APNS
    // device token. Note, you'll have to implement application:didRegisterForRemoteNotificationsWithDeviceToken:
    // in your UIApplicationDelegate. The readyBlock will be run once the manager is ready to send updates
    // to the server.
    [AGSGTGeotriggerManager setupWithClientId:kClientId
                                 isProduction:NO
                                    isOffline:NO
                                   completion:^(NSError *error) {
                                        [AGSGTGeotriggerManager sharedManager].trackingProfile = kAGSGTTrackingProfileAdaptive;
                                        NSString *managerReadyText = error == nil ? @"Yes" : @"No - error!";
                                       if(error) {
                                           NSLog(@"Error: %@", error);
                                       }
                                        [[NSUserDefaults standardUserDefaults] setObject:managerReadyText forKey:@"managerReadyText"];
    }];
    // If we were launched from a push notification, send the payload to the Geotrigger Manager
    if (launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey] != nil) {
        [AGSGTGeotriggerManager handlePushNotification:launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey] showAlert:NO];
    }
    return YES;
}
							
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    NSLog(@"Failed to register for remote notifications with Apple: %@", [error debugDescription]);
    [[NSNotificationCenter defaultCenter] postNotificationName:@"registerForRemoteNotificationsFailure" object:nil];
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    [[NSNotificationCenter defaultCenter] postNotificationName:@"registerForRemoteNotificationsSuccess" object:nil];
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    if ([userInfo[@"aps"][@"alert"] isEqualToString:@"Trigger fired!"]) {
        [[NSNotificationCenter defaultCenter] postNotificationName:@"pushNotificationReceived" object:nil];
    }
}

@end

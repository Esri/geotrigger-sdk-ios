//
//  GTSViewController.m
//  geotrigger-sample
//
//  Created by Ryan Arana on 11/8/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import "GTSViewController.h"
#import "Constants.h"
#import <GeotriggerSDK/GeotriggerSDK.h>

@interface GTSViewController ()

@property (assign, nonatomic) BOOL triggerCreated;
@end

@implementation GTSViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Set up the manager singleton with your clientId and any tracking profile you pass in here
    // will be used to start the manager when it is ready. You can also pass in UIRemoteNotificationTypes
    // to have the manager register your application for with Apple when it is ready to receive an APNS
    // device token. Note, you'll have to implement application:didRegisterForRemoteNotificationsWithDeviceToken:
    // in your UIApplicationDelegate. The readyBlock will be run once the manager is ready to send updates
    // to the server.
    [AGSGTGeotriggerManager setupWithClientId:kClientId
                              trackingProfile:kAGSGTTrackingProfileFine
               registerForRemoteNotifications:UIRemoteNotificationTypeAlert
                                   completion:^(NSError *error) {
                                       if (error == nil) {
                                           // Update our UI
                                           self.managerReadyLabel.text = @"Yes";
                                           
                                           // This is handy to test that push notifications are all set up correctly and
                                           // can work outside of the trigger process.
                                           //[[AGSGTApiClient sharedClient] postPath:@"device/notify"
                                           //                             parameters:@{ @"text": @"This came from device/notify", @"url": @"http://pdx.esri.com" }
                                           //                             success:^(AFHTTPRequestOperation *req, id res) {
                                           //                                 NSLog(@"device/notify success: %@", res);
                                           //                             }
                                           //                             failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                                           //                                 NSLog(@"device/notify failed: %@", error.userInfo);
                                           //                             }];
                                       } else {
                                           self.managerReadyLabel.text = @"No - Error!";
                                       }
                                   }];

    // The didReceiveLocationUpdates block is called every time the manager receive a CLLocation from the CLLocationManager.
    // You can use this block to get access to all location updates from the OS without implementing your own
    // CLLocationManagerDelegate. Here we use it to create a trigger around the first location we receive which will
    // fire on leaving the trigger region. On all subsequent location updates we update the UI to show the latest location
    // we've received.
    [AGSGTGeotriggerManager sharedManager].didReceiveLocationUpdates = ^(NSArray *locations) {
        CLLocation *location = locations[0];

        // Update our UI
        self.locationUpdateReceivedLabel.text = [NSString stringWithFormat:@"lat: %3.6f, long: %3.6f", location.coordinate.latitude, location.coordinate.longitude];

        if (!self.triggerCreated) {
            self.triggerCreated = YES;

            // Create a geotrigger builder object and set its properties as desired.
            AGSGTTriggerBuilder *builder = [AGSGTTriggerBuilder new];
            builder.triggerId = @"sampleTrigger";
            builder.tags = @[[AGSGTGeotriggerManager sharedManager].deviceDefaultTag];
            builder.direction = @"leave";
            [builder setGeoWithLocation:location distance:100];
            builder.notificationText = @"Trigger fired!";
            NSDictionary *params = [builder build];
            [[AGSGTApiClient sharedClient] postPath:@"trigger/create"
                                         parameters:params
                                            success:^(id res) {
                                                NSLog(@"Trigger created: %@", res);
                                                self.triggerCreatedLabel.text = @"Yes";
                                            }
                                            failure:^(NSError *error) {
                                                NSLog(@"Trigger create error: %@", error.userInfo);
                                                self.triggerCreatedLabel.text = @"Error";
                                                self.triggerCreated = NO;
                                            }];
        }
    };

    // The didUploadLocations block is called every time we send locations to the Geotrigger Service. Here we are just
    // using this information to update our UI.
    [AGSGTGeotriggerManager sharedManager].didUploadLocations = ^(int count, NSError *error) {
        if (error == nil) {
            AGSGTLocationFix *locationFix = [AGSGTGeotriggerManager sharedManager].lastSyncedLocation;
            self.locationUpdateSentLabel.text = [NSString stringWithFormat:@"lat: %3.6f, long: %3.6f", locationFix.location.coordinate.latitude, locationFix.location.coordinate.longitude];
        } else {
            NSLog(@"Location upload error: %@", error.userInfo);
        }
    };

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(registerForRemoteNotificationsFailure)
                                                 name:@"registerForRemoteNotificationsFailure" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(registerForRemoteNotificationsSuccess)
                                                 name:@"registerForRemoteNotificationsSuccess" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(pushNotificationReceived)
                                                 name:@"pushNotificationReceived" object:nil];
}

- (void)pushNotificationReceived {
    self.triggerFiredLabel.text = @"Yes";
}

- (void)registerForRemoteNotificationsFailure {
    self.registeredForPushNotificationsLabel.text = @"Error";
}

- (void)registerForRemoteNotificationsSuccess {
    self.registeredForPushNotificationsLabel.text = @"Yes";
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end

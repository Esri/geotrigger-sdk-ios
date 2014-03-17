//
//  GTSViewController.m
//  geotrigger-sample
//
//  Created by Ryan Arana on 11/8/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import "GTSViewController.h"
#import "GTSAppDelegate.h"
#import <GeotriggerSDK/GeotriggerSDK.h>

@interface GTSViewController ()

@property (assign, nonatomic) BOOL triggerCreated;
@property (strong, nonatomic) NSMutableOrderedSet *locations;
@end

@implementation GTSViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.locations = [NSMutableOrderedSet new];

    // The didReceiveLocationUpdates block is called every time the manager receive a CLLocation from the CLLocationManager.
    // You can use this block to get access to all location updates from the OS without implementing your own
    // CLLocationManagerDelegate. Here we use it to create a trigger around the first location we receive which will
    // fire on leaving the trigger region. On all subsequent location updates we update the UI to show the latest location
    // we've received.
    [AGSGTGeotriggerManager sharedManager].didReceiveLocationUpdates = ^(NSArray *locations) {
        [self.locations insertObjects:locations atIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, locations.count)]];
        CLLocation *location = locations[0];

        // Update our UI
        self.locationUpdateReceivedLabel.text = [NSString stringWithFormat:@"lat: %3.6f, long: %3.6f", location.coordinate.latitude, location.coordinate.longitude];
        self.managerReadyLabel.text = [[NSUserDefaults standardUserDefaults] stringForKey:@"managerReadyText"];

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
    [AGSGTGeotriggerManager sharedManager].didUploadLocations = ^(NSUInteger count, NSError *error) {
        if (error == nil) {
            CLLocation *location = self.locations[0];
            self.locationUpdateSentLabel.text = [NSString stringWithFormat:@"lat: %3.6f, long: %3.6f", location.coordinate.latitude, location.coordinate.longitude];
            NSRange locationsToRemove;
            if (count > self.locations.count) {
                locationsToRemove = NSMakeRange(0, self.locations.count);
            } else {
                locationsToRemove = NSMakeRange(self.locations.count-count, count);
            }
            [self.locations removeObjectsInRange:locationsToRemove];
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

- (IBAction)notifyClicked:(id)sender {
    [[AGSGTApiClient sharedClient] postPath:@"device/notify"
                                 parameters:@{ @"text": @"This came from device/notify", @"url": @"http://pdx.esri.com" }
                                    success:^(id res) {
                                        NSLog(@"device/notify success: %@", res);
                                    }
                                    failure:^(NSError *error) {
                                        NSLog(@"device/notify failed: %@", error.userInfo);
                                    }];
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

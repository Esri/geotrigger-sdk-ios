//
//  GTSViewController.h
//  geotrigger-sample
//
//  Created by Ryan Arana on 11/8/13.
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GTSViewController : UIViewController

@property (strong, nonatomic) IBOutlet UILabel *managerReadyLabel;
@property (strong, nonatomic) IBOutlet UILabel *registeredForPushNotificationsLabel;
@property (strong, nonatomic) IBOutlet UILabel *triggerCreatedLabel;
@property (strong, nonatomic) IBOutlet UILabel *triggerFiredLabel;
@property (strong, nonatomic) IBOutlet UILabel *locationUpdateReceivedLabel;
@property (strong, nonatomic) IBOutlet UILabel *locationUpdateSentLabel;
@property (weak, nonatomic) IBOutlet UIButton *notifyButton;

@end

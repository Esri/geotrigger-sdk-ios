# iOS Geotrigger SDK

## Overview

The iOS Geotrigger SDK is a set of tools to help your app communicate with the
Geotrigger Service and provide it with accurate location data without using any
more of the device's battery power than absolutely necessary.

At the core of the iOS Geotrigger SDK is the `AGSGTGeotriggerManager`, a wrapper around iOS's native location services, which provides a set of tracking
profiles to support a variety of use cases while preserving battery life as much
as possible. The `AGSGTGeotriggerManager` also handles uploading device
locations to the Geotrigger API.

In addition to the manager there is the `AGSGTApiClient` which can be used to call any of the server's [API methods][geotrigger-api-ref] using the `postPath:parameters:success:failure:` method.

Below you'll find the steps required to get your app (new or existing) up and running with the Geotrigger SDK. You also may find the [sample project](sample) helpful to see a project with the steps outlined here already completed.

## Add the Geotrigger SDK

There are two ways to add the Geotrigger SDK to your project. Using [CocoaPods], or manually.

**NOTE**: If you want your app to be able to receive location updates and send them to the server while your app is not
in the foreground you will need to enable the following "Background Modes":

* Location Updates
* Background Fetch
* Remote Notifications

These can be found in the "Capabilities" tab when you have your app's target selected in the Project Settings window.

### With Cocoapods

If you are using [CocoaPods] adding the Geotrigger SDK is extra easy!

1. Add `pod 'GeotriggerSDK', :git => "https://github.com/Esri/geotrigger-sdk-ios.git"` to your `Podfile`.
2. Run `pod install`.

This will grab the latest version of the Geotrigger SDK from github and set your project (workspace) up with all of the necessary dependencies and you're ready to go.

If you aren't using [CocoaPods] yet you should [check it out](http://docs.cocoapods.org/guides/installing_cocoapods.html)!

### Without Cocoapods

The Geotrigger SDK relies on the following Apple system libraries, which you'll need to make sure your project links:

* CoreLocation
* Security
* SystemConfiguration
* MobileCoreServices

Once you have all of the prerequisite dependencies added to your project the only thing left to do is copy the `GeotriggerSDK.framework` file from the [GitHub repo](https://github.com/Esri/geotrigger-sdk-ios) into your project and link against it. The easiest way to do this is to simply drag and drop it from Finder into the Frameworks folder in XCode.

## Set up the GeotriggerManager

Before you can start tracking the device's location or communicating with the Geotrigger API you'll need to set up the `AGSGTGeotriggerManager` singleton by calling `setupWithClientId:trackingProfile:registerForRemoteNotifications:completion:`.
It is recommended you do this in your `UIApplicationDelegate`'s `application:didFinishLaunchingWithOptions:` so that the manager can be ready to go as soon as possible.

If you want the manager to start sending location updates immediately, pass any `trackingProfile` value other than `kAGSGTTrackingProfileOff` to the set up method.

The `registerForRemoteNotifications:` parameter can be used to tell the manager to call `[UIApplication registerForRemoteNotificationTypes]` with the `UIRemoteNotificationType` you specify once this device has been registered with the Geotrigger Service and is ready to receive the APNS Device Token. You will still have to implement a few `UIApplicationDelegate` methods to send things along to the `AGSGTGeotriggerManager`. See the "Handling Push Notification Registration" section below for more details.

Here is a common way to start the manager.

```objectivec
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    ...
    [AGSGTGeotriggerManager setupWithClientId:@"[your clientId goes here]"
                              trackingProfile:kAGSGTTrackingProfileAdaptive
               registerForRemoteNotifications:UIRemoteNotificationTypeAlert
                                   completion:^(NSError *error) {
                                       if (error != nil) {
                                          NSLog(@"Geotrigger Service setup encountered error: %@", error);
                                       } else {
                                          NSLog(@"Geotrigger Service ready to go!");
                                       }
                                   }];
    ...
}
```

## Handling Push Notification Registration

After registering for remote notifications (either by calling the UIApplication method yourself or via the `AGSGTGeotriggerManager`)
you'll need to send the `deviceToken` that Apple sends to your `UIApplicationDelegate` to the Geotrigger Manager so that
we can attach it to the device on the server.

The `AGSGTGeotriggerManager` also provides a default handler for push notifications which does the following when it receives the notification:

* If the app is in the foreground - Displays the notification text in a `UIAlertView` with a "Close" button and a "View" button which will take the user
to the URL in the payload if there is one. The View button only shows up if there is a URL in the payload.
* If the app is in the background - Passing the userInfo dictionary to this handler when your app is launched in response to the user
tapping on the push notification will send the user straight to the URL instead.

```objectivec
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    [[AGSGTGeotriggerManager sharedManager] registerAPNSDeviceToken:deviceToken forProduction:NO completion:nil];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"Failed to register for remote notifications: %@", error);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    [[AGSGTGeotriggerManager sharedManager] handlePushNotification:userInfo];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  ...

    // If we were launched from a push notification, send the payload to the Geotrigger Manager
    if (launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey] != nil) {
        [[AGSGTGeotriggerManager sharedManager] handlePushNotification:launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey]];
    }

  ...
}
```

## Further Reading

Guides on various topics are available as part of the [Geotrigger Documentation][geotrigger-docs], including interacting with the `AGSGTGeotriggerManager`, working with the Geotrigger API, and configuring push notifications. Information about other platforms and the API itself are also available.

The SDK's [reference doc](Docs) is also available in this repo.

[esri-site]: http://www.esri.com
[arcgis-dev-site]: https://developers.arcgis.com
[geotrigger-docs]: https://developers.arcgis.com/geotrigger-service
[geotrigger-api-ref]: https://developers.arcgis.com/geotrigger-service/api-reference
[CocoaPods]: http://cocoapods.org/

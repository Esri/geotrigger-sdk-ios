# Geotrigger SDK for iOS

## Overview

The Geotrigger SDK for iOS is a set of tools to help your app communicate with the
Geotrigger Service and provide it with accurate location data without using any
more of the device's battery power than absolutely necessary.

At the core of the Geotrigger SDK for iOS is the `AGSGTGeotriggerManager`, a wrapper around iOS's native location services, which provides a set of tracking
profiles to support a variety of use cases while preserving battery life as much
as possible. The `AGSGTGeotriggerManager` also handles uploading device
locations to the Geotrigger API.

In addition to the manager there is the `AGSGTApiClient` which can be used to call any of the server's [API methods][geotrigger-api-ref] using the `postPath:parameters:success:failure:` method.

Below you'll find the steps required to get your app (new or existing) up and running with the Geotrigger SDK. You also may find the [sample project](sample) helpful to see a project with the steps outlined here already completed.

## Add the Geotrigger SDK

### Prerequisites

1. In order to enable location services in iOS 8 you will need to enter a message describing what your app does with the location
information in the `NSLocationAlwaysUsageDescription` in your `Info.plist` file. This message will be shown to the user when
the operating system first prompts them for permission to use their location. See the [Apple Documentation](https://developer.apple.com/library/ios/documentation/CoreLocation/Reference/CLLocationManager_Class/index.html#//apple_ref/doc/uid/TP40007125-CH3-SW62) on the subject
for more details.
2. In order to receive location updates and send them to the server while your app is not in the foreground you will need to
enable the following "Background Modes" which can be found in the "Capabilities" tab when you have your app's target selected in the Project Settings window:
  * Location Updates
  * Background Fetch

There are two ways to add the Geotrigger SDK for iOS to your project -- manually or using [CocoaPods](http://cocoapods.org/).

### With Cocoapods

If you are using [CocoaPods] adding the Geotrigger SDK is extra easy!

1. Add `pod 'GeotriggerSDK'` to your `Podfile`.
2. Run `pod install`.

This will grab the latest version of the Geotrigger SDK from github and set your project (workspace) up with all of the necessary dependencies and you're ready to go.

If you aren't using [CocoaPods] yet you should [check it out](http://docs.cocoapods.org/guides/installing_cocoapods.html)!

### Without Cocoapods

1. Copy the `GeotriggerSDK.framework` file from the [GitHub repo](https://github.com/Esri/geotrigger-sdk-ios) into your project and link against it. The easiest way to do this is to simply drag and drop it from Finder into the Frameworks folder in XCode.
2. Link in our dependencies. This can be done either of the following ways:
  1. Setting the "Enable Modules (C and Objective-C)" and "Link Frameworks Automatically" flags to `YES` in the
    "Apple LLVM 5.0 - Language - Modules" section of the Build Settings. These values are the defaults in XCode 5 and above.
  1. If for some reason you can't link frameworks automatically, these are the libraries that will be required to be linked in your project:
    * CoreLocation
    * Security
    * SystemConfiguration
    * MobileCoreServices


## Set up the GeotriggerManager

Before you can start tracking the device's location or communicating with the Geotrigger API
you'll need to set up the `AGSGTGeotriggerManager` singleton by calling one of the `setupWithClientId:...` methods.
It is recommended you do this in your `UIApplicationDelegate`'s `application:didFinishLaunchingWithOptions:`
so that the manager can be ready to go as soon as possible.

The minimum required parameters to these methods are:

* `clientId`: Your ArcGIS Online Application's ClientId
* `isProduction`: A boolean value signifying whether the app is built using a Production or Sandbox Provisioning Profile. This is used to tell the
Geotrigger Server where to send push notifications.
* `completion`: A block that will be executed once the `AGSGTGeotriggerManager` is done initializing. The `error` parameter will be `nil` if setup
was completed successfully, or the error that was encountered otherwise.

Other optional parameters include:

* `tags`: Passing this parameter will cause the `AGSGTGeotriggerManager` to add the specified tags to the device automaticallly once it is ready.
* `trackingProfile`: Passing this parameter will cause the `AGSGTGeotriggerManager` to turn on location updates automatically when it is ready using
this profile.

Here is a common way to start the manager.

```objectivec
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    ...
    [AGSGTGeotriggerManager setupWithClientId:kClientId isProduction:NO tags:@[@"test"] completion:^(NSError *error) {
         if (error != nil) {
             NSLog(@"Geotrigger Service setup encountered error: %@", error);
         } else {
             NSLog(@"Geotrigger Service ready to go!");

             // Turn on location tracking in adaptive mode
             [AGSGTGeotriggerManager sharedManager].trackingProfile = kAGSGGTTrackingProfileAdaptive;
         }
    }];
    ...
}
```

## Handling Push Notification Registration

By default when you call one of the `setupWithClientId:...` methods, `AGSGTGeotriggerManager` will set itself up to receive and handle push notifications
automatically by becoming the primary `UIApplicationDelegate` and forwarding all `UIApplicationDelegate` methods to your `UIApplicationDelegate`. This sets it up
so that if your app is in the foreground when a notification is received this will display the notification text in a `UIAlertView` with a "Close"
button and a "View" button which will take the user to the URL in the payload if there is one. The View button only shows up if there is a URL in the payload.
If you want to send the user straight to the URL when they open your app in response to a push notification you will need to add the following to your
`application:didFinishLaunchingWithOptions:`

```objectivec
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  ...

    // If we were launched from a push notification, send the payload to the Geotrigger Manager
    if (launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey] != nil) {
        [AGSGTGeotriggerManager handlePushNotification:launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey] showAlert:NO];
    }

  ...
}
```

If you need to implement custom handling of push notifications you can do so using the normal `UIApplicationDelegate` method
(`application:didReceiveRemoteNotification:`). Be sure *not* to call the `handlePushNotification:...` methods on `AGSGTGeotriggerManager` in this
method though, or your app will notify your user twice!

If you prefer to set up push notifications manually put the following in before your call to `setupWithClientId:...`:

```objectivec
    [AGSGTGeotriggerManager sharedManager].autoSetupEnabled = NO;
```

After registering for remote notifications (either by calling the `UIApplication` method yourself or via the `AGSGTGeotriggerManager`) you’ll need to
send the deviceToken that Apple sends to your UIApplicationDelegate to the Geotrigger Manager so that we can attach it to the device on the server.

```objectivec
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    [[AGSGTGeotriggerManager sharedManager] registerAPNSDeviceToken:deviceToken completion:nil];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"Failed to register for remote notifications: %@", error);
}
```

## Further Reading

Guides on various topics are available as part of the [Geotrigger Documentation][geotrigger-docs], including interacting with the `AGSGTGeotriggerManager`, working with the Geotrigger API, and configuring push notifications. Information about other platforms and the API itself are also available.

The SDK's [reference doc](Docs) is also available in this repo.

[esri-site]: http://www.esri.com
[arcgis-dev-site]: https://developers.arcgis.com
[geotrigger-docs]: https://developers.arcgis.com/en/geotrigger-service
[geotrigger-api-ref]: https://developers.arcgis.com/en/geotrigger-service/api-reference
[CocoaPods]: http://cocoapods.org/

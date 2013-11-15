# iOS Geotrigger SDK Sample App
This app is a self contained skeleton app made to show the bare minimum of what an app would need to do to get up and running with the Geotrigger Service and SDK.
It aims to be a reference point and test environment to verify that you have all of the non-code pieces set up correctly (i.e. Push Notifications, ArcGIS Online, etc).

The app creates a trigger at its current location with a 100m radius which will fire when the device leaves that location. The trigger is configured to send a Push Notification when it fires, but you'll need to make sure you're set up to send/receive push notifications first. See our [Push Notifications Guide][push-notifications-docs] for details on that.

## Set up

1. Copy `Constants.h.dist` to `Constants.h`
2. Edit `Constants.h` and replace "Your client id here" with your AGO Client ID.
3. If you want to test Push Notifications you'll need all of the following (See our [push notifications guide][push-notifications-docs] for details):
  1. A fully qualified Bundle Identifier
  2. A Provisioning Profile and Certificate for that Bundle Identifier that has Push Notifications enabled on it.
  3. Add your Certificate public key to your AGO application's push certificates page.

## GTSViewController.m
In [GTSViewController.m](geotrigger-sample/GTSViewController.m) in `viewDidLoad` we set up the manager passing in a block that will be run once all of the registration (or loading from cache) has been finished and the SDK has all of the info it needs to communicate with the server. In this block we just update a piece of our UI to display this update to the user.

We also add a block to the manager singleton that allows us to update our UI whenever we send location updates to the Geotrigger Service. And another block so that when we receive our first location update from the device we can create a trigger at the current location.

Then we listen for some NSNotifications that we send from the AppDelegate when we register for Push Notifications and when we receive them.

## GTSAppDelegate.m
In [GTSAppdelegate.m](geotrigger-sample/GTSAppDelegate.m) we implement the necessary delegate methods for handling Push Notification registration and forwarding on the notifications to the Geotrigger SDK as we receive them.

[push-notifications-docs]:https://developers.arcgis.com/geotriggers/ios-push-notifications

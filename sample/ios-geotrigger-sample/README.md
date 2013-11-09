# Overview
This sample app shows the most basic set of Geotrigger Service functionality and is designed to work as a starting point for you to
create new apps from, as well as a minimal set of functionality to test that all of the necessary external configuration pieces are
correctly in place.

## Build Instructions
1. Copy Constants.h.dist to Constants.h and put a valid clientId in it.
2. In order to receive the push notifications generated from the sample, you will need to set up your provisioning profile
   and certificates correctly and register them with AGO. See [here](http://esri.github.io/geotrigger-docs/ios/push-notifications/)
   for details on setting this up with Apple and AGO.

After doing these two steps you should be able to build the app to a device and walk about 100m away and get a push message! If not,
there is a commented out chunk of code in the `GTViewController`'s `viewDidLoad` method which you can uncomment and rerun the app.
You should then get a push notification that says "This came from device/notify". If you don't, the problem is somewhere in your
push notification set up, check the console logs for any errors about registering for remote notifications, [this guide](http://esri.github.io/geotrigger-docs/ios/push-notifications/)
could also be helpful.

If you aren't able to get push messages from the sample app, please [contact support](mailto:geotriggerdev@esri.com).


/*
 COPYRIGHT 2013 ESRI

 TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
 Unpublished material - all rights reserved under the
 Copyright Laws of the United States and applicable international
 laws, treaties, and conventions.

 For additional information, contact:
 Environmental Systems Research Institute, Inc.
 Attn: Contracts and Legal Services Department
 380 New York Street
 Redlands, California, 92373
 USA

 email: contracts@esri.com
 */

#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

/** @constant AGSGTGeotriggerManager kAGSGTTrackingProfile Do not track location at all. */
static NSString * const kAGSGTTrackingProfileOff = @"off";
/** Adaptive mode intelligently turns GPS monitoring on and off based on many factors and feedback from the server in order to get the best location accuracy to battery usage ratio. This is the mode most apps will want to be in most of the time. */
static NSString * const kAGSGTTrackingProfileAdaptive = @"adaptive";
/** Rough mode will use `CoreLocation`'s `Significant Location Change` mode to track user location. This mode will use the least amount of battery, but at the cost of location accuracy. */
static NSString * const kAGSGTTrackingProfileRough = @"rough";
/** Fine mode receives the most accurate and up to date location information at the cost of battery usage. */
static NSString * const kAGSGTTrackingProfileFine = @"fine";

@class AGSGTGeotriggerManager;

@protocol AGSGTGeotriggerManagerDelegate <NSObject>
@optional

/** Tells the delegate that new location data is available.

  @param manager The `AGSGTGeotriggerManager` object that generated the event.
  @param locations The `NSArray` that the `CLLocationManager` passed to the `AGSGTGeotriggerManager`.
  @param isOnDemand A `BOOL` that indicates whether or not the locations were obtained as a result of calling `requestOnDemandUpdate:`.
 */
- (void)manager:(AGSGTGeotriggerManager *)manager didReceiveLocationUpdates:(NSArray *)locations isOnDemand:(BOOL)isOnDemand;

/** Tells the delegate that locations have been uploaded to the Geotrigger Service servers.

  @param manager The `AGSGTGeotriggerManager` object that generated the event.
  @param count The number of location updates that were sent to the server.
*/
- (void)manager:(AGSGTGeotriggerManager *)manager didUploadLocations:(NSUInteger)count;

/** Tells the delegate that locations have failed to upload to the Geotrigger Service servers.

  @param manager The `AGSGTGeotriggerManager` object that generated the event.
  @param error The error that was encountered when trying to upload the locations.
*/
- (void)manager:(AGSGTGeotriggerManager *)manager didFailToUploadLocations:(NSError *)error;

/** Tells the delegate that the Tracking Profile has been changed.

  @param manager The `AGSGTGeotriggerManager` object that generated the event.
  @param newProfile The new Tracking Profile that the `AGSGTGeotriggerManager` is using.
  @param oldProfile The Tracking Profile that the `AGSGTGeotriggerManager` was using previously.
*/
- (void)manager:(AGSGTGeotriggerManager *)manager didChangeTrackingProfileTo:(NSString *)newProfile from:(NSString *)oldProfile;

@end

/**
* The log levels available in the Geotrigger SDK.
*/
typedef NS_ENUM(int, AGSGTLogLevel) {
    /** Do not log anything. */
    AGSGTLogLevelOff = 0,
    /** Only log errors. */
    AGSGTLogLevelError = 1,
    /** Only log warnings and errors. */
    AGSGTLogLevelWarn = 3,
    /** Include some additional informative logs. */
    AGSGTLogLevelInfo = 7,
    /** Enable debug logging. Be warned, this is very chatty, and should be used for debugging only! */
    AGSGTLogLevelDebug = 15,
    /** Enable all of the logs. Be warned, this is very chatty, and should be used for debugging only! */
    AGSGTLogLevelVerbose = 31
};

/** <code>CLLocationManagerDelegate</code> implementation which handles the receiving and uploading of location updates to the Geotrigger Service.

 This object also provides hooks for you to respond to location updates and locations being sent to the server.
 */
@interface AGSGTGeotriggerManager : NSObject <CLLocationManagerDelegate, UIAlertViewDelegate>

/**
* The `AGSGTGeotriggerManagerDelegate` implementation that will handle all callbacks from the `AGSGTGeotriggerManager`.
*/
@property(nonatomic, weak) id<AGSGTGeotriggerManagerDelegate> delegate;

/** The tracking profile the AGSGTGeotriggerManager is currently using.

 This property is used to tell the AGSGTGeotriggerManager to optimize for accuracy vs battery life, or to turn off location updates completely.
 */
@property(nonatomic, copy) NSString *trackingProfile;

/** The default tag of this device.
*/
@property(nonatomic, strong, readonly) NSString *deviceDefaultTag;

/** The deviceId of this device.
*/
@property(nonatomic, strong, readonly) NSString *deviceId;

/** AutoSetup makes the `AGSGTGeotriggerManager` set itself as the UIApplicationDelegate for your app and forward
* all calls to your application's `UIApplicationDelegate`. This allows the Manager to handle push notifications and
* traversal to and from running in the background automatically.
*/
@property(nonatomic, assign, readwrite, getter=isAutoSetupEnabled) BOOL autoSetupEnabled;

/** Block to be called every time the `CLLocationManager` calls `locationManager:didUpdateLocations:`

 The `NSArray` that it is called with will be the same one that the `CLLocationManager` passes to its delegate.
 */
@property(copy) void(^didReceiveLocationUpdates)(NSArray *locations);

/** Block to be called every time the `CLLocationManager` calls `locationManager:didUpdateLocations:`

 The block will be called with the `CLLocation` object obtained as a result of a call to `requestOnDemandUpdate:`.
 */
@property(copy) void(^didReceiveOnDemandUpdate)(CLLocation *location);

/** Block to be called every time location updates are sent to the server.

 The block will be called with two parameters, the number of locations uploaded
 and an NSError (which will be nil on success).
 */
@property(copy) void(^didUploadLocations)(NSUInteger count, NSError *error);

/** Block to be called every time the tracking profile is changed.
 
 The block will be called with two parameters, the previous profile and the new current profile.
 */
@property(copy) void(^didChangeTrackingProfile)(NSString *old, NSString *new);

/** Setup the manager with the given clientId.

  @param clientId The Client ID to set the manager up with.
  @param isProduction A flag determining whether the application is signed with a Production/Distribution certificate
         or a Sandbox certificate for push notifications.
  @param completion This block will be called once the manager has finished setting itself up and is ready to upload locations. Unless
         there is an error encountered during that process, in which case the error parameter will be non-nil.
 */
+ (void)setupWithClientId:(NSString *)clientId isProduction:(BOOL)isProduction completion:(void (^)(NSError *error))completion;

/** Setup the manager with the given clientId and add the supplied list of tags to the device when it is created.

  @param clientId The Client ID to set the manager up with.
  @param isProduction A flag determining whether the application is signed with a Production/Distribution certificate
         or a Sandbox certificate for push notifications.
  @param tags An optional list of tags to be set on the device after it registers itself.
  @param completion This block will be called once the manager has finished setting itself up and is ready to upload locations. Unless
         there is an error encountered during that process, in which case the error parameter will be non-nil.
 */
+ (void)setupWithClientId:(NSString *)clientId
                  isProduction:(BOOL)isProduction
                          tags:(NSArray *)tags
                    completion:(void (^)(NSError *error))completion;

/** Setup the manager with the given clientId and configure it with the given tracking profile

  Deprecated in version 1.1.0.

  @param clientId The Client ID to set the manager up with.
  @param trackingProfile An optional kAGSGTTrackingProfile to configure the manager with once it is has been initialized.
  @param isProduction A flag determining whether the application is signed with a Production/Distribution certificate
         or a Sandbox certificate for push notifications.
  @param tags An optional list of tags to be set on the device after it registers itself.
  @param completion This block will be called once the manager has finished setting itself up and is ready to upload locations. Unless
         there is an error encountered during that process, in which case the error parameter will be non-nil.
  @deprecated Please use another setupWithClientId method.
 */
+ (void)setupWithClientId:(NSString *)clientId
               trackingProfile:(NSString *)trackingProfile
                  isProduction:(BOOL)isProduction
                          tags:(NSArray *)tags
               completion:(void (^)(NSError *error))completion __deprecated;

/** Setup the manager with the given clientId and configure it with the given tracking profile

  Deprecated in version 1.1.0.

  @param clientId The Client ID to set the manager up with.
  @param trackingProfile An optional kAGSGTTrackingProfile to configure the manager with once it is has been initialized.
  @param notificationTypes An optional type(s) of UIRemoteNotifications to register with Apple for. Set this to UIRemoteNotificationTypeNone to not register for remote notifications.
  @param isProduction A flag determining whether the application is signed with a Production/Distribution certificate
         or a Sandbox certificate for push notifications.
  @param tags An optional list of tags to be set on the device after it registers itself.
  @param completion This block will be called once the manager has finished setting itself up and is ready to upload locations. Unless
         there is an error encountered during that process, in which case the error parameter will be non-nil.
  @deprecated Please use another setupWithClientId method.
 */
+ (void)setupWithClientId:(NSString *)clientId
               trackingProfile:(NSString *)trackingProfile
registerForRemoteNotifications:(UIRemoteNotificationType)notificationTypes
                  isProduction:(BOOL)isProduction
                          tags:(NSArray *)tags
               completion:(void (^)(NSError *error))completion __deprecated;

/** Returns the singleton geotrigger manager instance.
 */
+ (instancetype)sharedManager;

#pragma mark On-Demand Update

/** Requests a single update. Useful for obtaining location information when the app enters the foreground.

  The location update will be sent to the `didReceiveOnDemandUpdate` block or the `manager:didReceiveLocationUpdates:isOnDemand:`
  delegate method, depending on which you have defined.
 */
- (void)requestOnDemandUpdate;

#pragma mark Push Notifications

/** Disable pinning the SSL Certificate to a known valid certificate when communicating with the Geotrigger Service.
*/
- (void)disableSSLCertPinning;

/** Enable pinning the SSL Certificate to a known valid certificate when communicating with the Geotrigger Service.
*/
- (void)enableSSLCertPinning;

/** Register the given token with the Geotrigger Service so that push notifications can be sent to this device.

    @param deviceToken The device token that is sent back to you from Apple in didRegisterForRemoteNotifications: on your app delegate.
    @param completion Block which will be called upon completion of the registration
*/
- (void)registerAPNSDeviceToken:(NSData *)deviceToken completion:(void (^)(NSDictionary *, NSError *))completion;

/** Handle a push notification received from the Geotrigger service.
 @param userInfo The userInfo <code>NSDictionary</code> that is given with the remote notification
 to <code>application:didReceiveRemoteNotification:</code> or <code>application:didFinishLaunchingWithOptions:</code>.
 */
+ (void)handlePushNotification:(NSDictionary *)userInfo;

/** Handle a push notification received from the Geotrigger service.
 @param userInfo The userInfo <code>NSDictionary</code> that is given with the remote notification
 to <code>application:didReceiveRemoteNotification:</code> or <code>application:didFinishLaunchingWithOptions:</code>.
 @param showAlert Flag to tell the SDK whether it should show a UIAlertView displaying the text of the notification. If set to no, and the notification has a URL set, this method will automatically open the URL.
 */
+ (void)handlePushNotification:(NSDictionary *)userInfo showAlert:(BOOL)showAlert;

#pragma mark Logging

/** Sets the log level used by the SDK. This defaults to AGSGTLogLevelWarn

  @param logLevel The AGSGTLogLevel to be used.
*/
+ (void)setLogLevel:(AGSGTLogLevel)logLevel;

/** Enables/Disables logs being written to a file on the device.
*
* @param enable Set to YES if you want the SDK to log to a file. It will log to the Library/Caches/Logs folder on the device.
*/
+ (void)enableFileLogging:(BOOL)enable;

@end

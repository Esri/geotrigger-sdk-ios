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

@class AGSGTLocationFix;

/** @constant AGSGTGeotriggerManager kAGSGTTrackingProfile Do not track location at all. */
static NSString * const kAGSGTTrackingProfileOff = @"off";
/** Adaptive mode intelligently turns GPS monitoring on and off based on many factors and feedback from the server in order to get the best location accuracy to battery usage ratio. This is the mode most apps will want to be in most of the time. */
static NSString * const kAGSGTTrackingProfileAdaptive = @"adaptive";
/** Rough mode will use `CoreLocation`'s `Significant Location Change` mode to track user location. This mode will use the least amount of battery, but at the cost of location accuracy. */
static NSString * const kAGSGTTrackingProfileRough = @"rough";
/** Fine mode receives the most accurate and up to date location information at the cost of battery usage. */
static NSString * const kAGSGTTrackingProfileFine = @"fine";

/**
* The log levels available in the Geotrigger SDK.
*/
typedef NS_ENUM(int, AGSGTLogLevel) {
    /** Do not log anything. */
    AGSGTLogLevelOff,
    /** Only log errors. */
    AGSGTLogLevelError,
    /** Only log warnings and errors. */
    AGSGTLogLevelWarn,
    /** Enable all of the logs. Be warned, this is very chatty, and should be used for debugging only! */
    AGSGTLogLevelDebug
};

/** <code>CLLocationManagerDelegate</code> implementation which handles the receiving and uploading of location updates to the Geotrigger Service.

 This object also provides hooks for you to respond to location updates and locations being sent to the server.
 */
@interface AGSGTGeotriggerManager : NSObject <CLLocationManagerDelegate, UIAlertViewDelegate>

/** Array of [AGSGTLocationFixes](AGSGTLocationFix) in the queue to be sent to the server during the next upload.
 */
@property(nonatomic, copy, readonly) NSArray *queuedLocationUpdates;

/** The last AGSGTLocationFix that was uploaded to the server.
 */
@property(nonatomic, strong, readonly) AGSGTLocationFix *lastSyncedLocation;

/** The tracking profile the AGSGTGeotriggerManager is currently using.

 This property is used to tell the AGSGTGeotriggerManager to optimize for accuracy vs battery life, or to turn off location updates completely.
 */
@property(nonatomic, copy) NSString *trackingProfile;

/** The last AGSGTLocationFix that was entered in the queue.
 */
@property(nonatomic, strong, readonly) AGSGTLocationFix *lastEnqueuedLocation;

/** The default tag of this device.
*/
@property(nonatomic, strong, readonly) NSString *deviceDefaultTag;

/** The deviceId of this device.
*/
@property(nonatomic, strong, readonly) NSString *deviceId;

/** Block to be called every time the `CLLocationManager` calls `locationManager:didUpdateLocations:`

 The `NSArray` that it is called with will be the same one that the `CLLocationManager` passes to its delegate.
 */
@property(copy) void(^didReceiveLocationUpdates)(NSArray *locations);

/** Block to be called every time [AGSGTLocationFixes](AGSGTLocationFix) are sent to the server.

 The block will be called with two parameters, the number of locations uploaded
 and an NSError (which will be nil on success).
 */
@property(copy) void(^didUploadLocations)(NSUInteger count, NSError *error);

/** Block to be called every time the tracking profile is changed.
 
 The block will be called with two parameters, the previous profile and the new current profile.
 */
@property(copy) void(^didChangeTrackingProfile)(NSString *old, NSString *new);

/** Setup the manager with the given clientId and configure it with the given tracking profile

  @param clientId The Client ID to set the manager up with.
  @param trackingProfile The kAGSGTTrackingProfile to configure the manager with once it is has been initialized.
  @param notificationTypes The type(s) of UIRemoteNotifications to register with Apple for. Set this to UIRemoteNotificationTypeNone to not register for remote notifications.
  @param completion This block will be called once the manager has finished setting itself up and is ready to upload locations. Unless
         there is an error encountered during that process, in which case the error parameter will be non-nil.
 */
+ (void)setupWithClientId:(NSString *)clientId
               trackingProfile:(NSString *)trackingProfile
registerForRemoteNotifications:(UIRemoteNotificationType)notificationTypes
                    completion:(void (^)(NSError *error))completion;

/** Returns the singleton geotrigger manager instance.
 */
+ (instancetype)sharedManager;

#pragma mark Location Queue
/** Clears all pending [AGSGTLocationFixes](AGSGTLocationFix) from the queue.
 */
- (void)clearLocationQueue;

/** Enqueues a AGSGTLocationFix to be uploaded on the next upload.
 
 @param locationFix an AGSGTLocationFix to be added to the location queue.
 */
- (void)enqueueLocationUpdate:(AGSGTLocationFix *)locationFix;

/** Trigger an upload of all pending [AGSGTLocationFixes](AGSGTLocationFix)

 @param force If true the upload will be forced and be performed immediately, if false the AGSGTGeotriggerManager
 will check its tracking profile and last upload time before performing the upload.
 */
- (void)uploadLocationQueue:(BOOL)force;

#pragma mark Push Notifications

/** Register the given token with the Geotrigger Service so that push notifications can be sent to this device.

    @param deviceToken The device token that is sent back to you from Apple in didRegisterForRemoteNotifications: on your app delegate.
    @param forProduction YES if this token was registered for the production environment, NO if for the sandbox.
    @param completion Block which will be called upon completion of the registration
*/
- (void)registerAPNSDeviceToken:(NSData *)deviceToken forProduction:(BOOL)forProduction completion:(void (^)(NSDictionary *, NSError *))completion;

/** Handle a push notification received from the Geotrigger service.
 @param userInfo The userInfo <code>NSDictionary</code> that is given with the remote notification
 to <code>application:didReceiveRemoteNotification:</code> or <code>application:didFinishLaunchingWithOptions:</code>.
 */
- (void)handlePushNotification:(NSDictionary *)userInfo;

#pragma mark Logging

/** Sets the log level used by the SDK. This defaults to AGSGTLogLevelWarn

  @param logLevel The AGSGTLogLevel to be used.
  @param console Set to YES if you want the SDK to log to the console.
  @param fileLogs Set to YES if you want the SDK to log to a file. It will log to the Library/Caches/Logs folder on the device.
*/
- (void)setLogLevel:(AGSGTLogLevel)logLevel enableConsoleLogs:(BOOL)console enableFileLogs:(BOOL)fileLogs;

@end


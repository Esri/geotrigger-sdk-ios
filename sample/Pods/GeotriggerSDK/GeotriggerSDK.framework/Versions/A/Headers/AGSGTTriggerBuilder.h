//
//  AGSGTTriggerBuilder
//  GeotriggerSDK
//
//  Created by ryana on 8/29/13
//  Copyright (c) 2013 ESRI. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CLLocation;

/**
 Helps construct an `NSDictionary` that can be used with [AGSGTApiClient](AGSGTApiClient) to create a trigger.

 See the (external) API reference for the [trigger/create](https://developers.arcgis.com/geotrigger-service/api-reference/trigger-create/)
 route for more information.
 */
@interface AGSGTTriggerBuilder : NSObject

/** An arbitrary ID for this trigger. */
@property (strong, nonatomic) NSString *triggerId;

/** Tag names to apply to this trigger. */
@property (copy, nonatomic) NSArray *tags;

/** Arbitrary JSON properties for the trigger.
 *
 * This can be used to attach any extra metadata to the trigger so that it can be retrieved later.
 */
@property (copy, nonatomic) NSDictionary *properties;

/** The type of bounding box that the server should return.
 *
 * Must be one of:
 *
 * - `geojson`
 * - `esrijson`
 */
@property (strong, nonatomic) NSString *boundingBoxReturnFormat;

/** The type of geometry that the server should return.

 Must be one of:

 - `geojson`
 - `esrijson`
*/
@property (strong, nonatomic) NSString *geoReturnFormat;

// Condition

/** The direction condition for this trigger.

 Must be one of:

 - `enter`
 - `leave`
*/
@property (strong, nonatomic) NSString *direction;

/** The geographic region for this trigger. */
@property (copy, nonatomic) NSDictionary *geo;

/** The date that the trigger will start being active. */
@property (strong, nonatomic) NSDate *fromTimestamp;

/** The date that the trigger will stop being active. */
@property (strong, nonatomic) NSDate *toTimestamp;

// Action

/** The URL that will be notified when this trigger is fired.
 *
 * Callback URLs can be used to execute custom logic on your own server when this trigger is fired.
 */
@property (strong, nonatomic) NSURL *callbackUrl;

/** The tracking profile that a device will switch to when it activates this trigger.

  Must be one of:

  * `off`
  * `rough`
  * `adaptive`
  * `fine`
*/
@property (strong, nonatomic) NSString *trackingProfile;


// Notification

/** The text of the notification that will be sent when this trigger is fired. */
@property (strong, nonatomic) NSString *notificationText;

/** The URL to be opened when the notification is tapped. */
@property (strong, nonatomic) NSURL *notificationUrl;

/** The name of the sound to be played when the notification is received. */
@property (strong, nonatomic) NSString *notificationSound;

/** The badge count that will be displayed on the app icon when the notification is received. */
@property (assign, nonatomic) int notificationBadge;

/** Arbitrary JSON data to be sent along with this trigger notification. */
@property (copy, nonatomic) NSDictionary *notificationData;

/** Set the geographic region for this trigger from a point and distance.

 @param location The <code>CLLocation</code> of the center point of the trigger.
 @param distance The distance in meters.
*/
- (void)setGeoWithLocation:(CLLocation *)location distance:(double)distance;

/** Set the geographic region for this trigger from a point and distance.

 @param latitude Latitude of the center point of the trigger.
 @param longitude Longitude of the center point of the trigger.
 @param distance A distance in meters.
*/
- (void)setGeoWithLatitude:(double)latitude longitude:(double)longitude distance:(double)distance;

/** Set the geographic region for this trigger from a GeoJSON geometry.

 @param geoJson An NSDictionary containing valid GeoJSON.
*/
- (void)setGeoWithGeoJson:(NSDictionary *)geoJson;

/** Set the geographic region for this trigger from a Esri JSON geometry.

 @param esriJson An NSDictionary containing valid Esri JSON.
*/
- (void)setGeoWithEsriJson:(NSDictionary *)esriJson;

/** Builds a parameters NSDictionary suitable for use with the AGSGTApiClient.
*
* @see AGSGTApiClient postPath:parameters:success:failure:
*/
- (NSDictionary *)build;

@end
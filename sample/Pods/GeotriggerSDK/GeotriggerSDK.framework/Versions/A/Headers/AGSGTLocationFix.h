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

/** @brief A location object used with the ArcGIS Geotrigger Service.
 */
@interface AGSGTLocationFix : NSObject

/** The <code>CLLocation</code> object associated with this AGSGTLocationFix.
 */
@property(nonatomic, strong, readonly) CLLocation *location;

/** A dictionary containing user-defined data for this location fix.
 */
@property(nonatomic, copy) NSDictionary *properties;

/** The battery level when this location fix was recorded.
 */
@property(nonatomic, assign) float batteryPercent;

/** The battery state when this location fix was recorded. Must be a valid <code>UIDeviceBatteryState</code> value.
 */
@property(nonatomic, assign) UIDeviceBatteryState batteryState;

/** The tracking profile that was in use when this location fix was recorded. Must be a AGSGTTrackingProfile value.
 */
@property(nonatomic, copy) NSString *trackingProfile;

/** Create a new AGSGTLocationFix from the given <code>CLLocation</code>.
 
 @param location a CLLocation to create this AGSGTLocationFix from.
 */
+ (instancetype)locationFixWithLocation:(CLLocation *)location;

/** Initialize an AGSGTLocationFix from the given <code>CLLocation</code>.
 
 @param location a CLLocation to initialize this AGSGTLocationFix with.
 */
- (id)initWithLocation:(CLLocation *)location;

/** Initialize an AGSGTLocationFix from the given <code>NSDictionary</code>.
 
 @param json an NSDictionary containing a serialized AGSGTLocationFix object.
 */
- (id)initWithJSON:(NSDictionary *)json;

/** Returns a JSON encoded NSDictionary representing this AGSGTLocationFix.
 */
- (NSDictionary *)encodeToJSON;

@end

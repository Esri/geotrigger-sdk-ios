//
//  AGSGTApiClient
//  GeotriggerSDK
//
//  Created by ryana on 8/21/13
//  Copyright (c) 2013 ESRI. All rights reserved.
//

/** A subclass of AFHTTPClient which provides access to the Esri Geotrigger Service API.
 */
@interface AGSGTApiClient : NSObject

/** Returns the singleton Geotrigger API client instance.
 */
+ (instancetype)sharedClient;

/** Enable pinning the SSL Certificate to a known valid certificate when communicating with the Geotrigger Service.
*/
- (void)enableSSLCertPinning;

/** Disable pinning the SSL Certificate to a known valid certificate when communicating with the Geotrigger Service.
*/
- (void)disableSSLCertPinning;

/** Set the token used for OAuth 2.0 Authentication with the Geotrigger Server.
 
 @param token The token to be used for authentication.
 */
- (void)setAuthorizationHeaderWithToken:(NSString *)token;

/** Clear all auth headers currently in use.
 */
- (void)clearAuthorizationHeader;

/** Make a request with the given parameters to the specified path.
 
 The request will be made asynchronously and upon completion, either the success or failure block will be called.
 
 @param path a Geotrigger Service API endpoint.
 @param parameters an <code>NSDictionary</code> containing parameters for the selected path.
 @param success an <code>NSBlock</code> that will handle the response.
 @param failure an <code>NSBlock</code> that will handle any errors.
 */
- (void)postPath:(NSString *)path parameters:(NSDictionary *)parameters success:(void (^)(id responseObject))success failure:(void (^)(NSError *error))failure;

@end
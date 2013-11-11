Pod::Spec.new do |s|

  # Metadata
  s.name         = "GeotriggerSDK"
  s.version      = "0.0.1"
  s.summary      = "A library for using the ArcGIS Geotrigger Service with iOS."
  # TODO: Write a fer reals description.
  s.description  = <<-DESC
                      A library for using the ArcGIS Geotrigger Service with iOS.
                   DESC
  s.homepage     = "https://github.com/geoloqi/Geotrigger-SDK-iOS"
  s.author       = "Esri Portland R & D Center"

  # Spec
  s.platform     = :ios, "6.0"
  # TODO: update this url when we go public with the Esri one. We'll also need to make sure that repo has the 0.2.0 tag on it.
  s.source       = { :git => "https://github.com/Esri/geotrigger-sdk-ios.git", :tag => s.version.to_s }
  # TODO: Add a license
  s.license      = { :file => "LICENSE", :type => "Commercial" }

  # Frameworks
  s.frameworks = "CoreLocation", "UIKit", "Foundation", "CoreGraphics", "Security", "SystemConfiguration", "MobileCoreServices"
  s.vendored_frameworks = "GeotriggerSDK.framework"
  s.preserve_paths = "GeotriggerSDK.framework"

  # Project settings
  s.requires_arc = true
  s.xcconfig = { "OTHER_LDFLAGS" => "-ObjC" }

end

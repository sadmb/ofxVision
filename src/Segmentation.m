//
//  Segmentation.m
//  example
//
//  Created by Roy Macdonald on 19-03-24.
//


#include "Segmentation.h"

@implementation Segmentation


- (instancetype)init {
    self = [super init];
    if(self) {
                
        segmentationRequest = [VNGeneratePersonSegmentationRequest new];
        segmentationRequest.qualityLevel = VNGeneratePersonSegmentationRequestQualityLevelBalanced;
        segmentationRequest.outputPixelFormat = kCVPixelFormatType_OneComponent8;
        handler = [[VNSequenceRequestHandler alloc] init];
        
        _quality = 2;
        
    }
    return self;
}

-(void) setQualityLevel:(int)quality{
    if(segmentationRequest){
        if(quality <= 1){
            [segmentationRequest setQualityLevel:VNGeneratePersonSegmentationRequestQualityLevelFast];
        }else if(quality == 2){
            [segmentationRequest setQualityLevel:VNGeneratePersonSegmentationRequestQualityLevelBalanced];
        }else if(quality >= 3){
            [segmentationRequest setQualityLevel:VNGeneratePersonSegmentationRequestQualityLevelAccurate];
        }
    }
}

-(CVPixelBufferRef)detect:(CGImageRef)image{
    
    if([handler  performRequests:@[ segmentationRequest] onCGImage: image error:nil]){
        if( [segmentationRequest.results count ] ){
            if([segmentationRequest.results firstObject]){
                return [segmentationRequest.results firstObject].pixelBuffer;
                
            }
        }
    }

    return nil;
}

@end

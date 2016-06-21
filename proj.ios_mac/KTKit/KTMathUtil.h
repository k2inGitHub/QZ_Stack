//
//  KTMathUtil.h
//  VPN-iOS
//
//  Created by 宋扬 on 16/2/24.
//  Copyright © 2016年 handloft. All rights reserved.
//

#import <Foundation/Foundation.h>


#define clamp(n,min,max)                        ((n < min) ? min : (n > max) ? max : n)

@interface KTMathUtil : NSObject

+ (int)randomIntRange:(int)num1 andMax:(int)num2;

+ (float)randomFloatRange:(float)num1 andMax:(float)num2;

+ (int)randomChoice:(NSArray *) weights;

@end

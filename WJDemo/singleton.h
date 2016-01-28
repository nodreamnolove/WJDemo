//
//  singleton.h
//  WJDemo
//
//  Created by mac on 16/1/28.
//  Copyright © 2016年 WanJi. All rights reserved.
//



// @interface
#define singleton_interface(className) \
+ (className *)shared##className;


#define singleton_implementation(className) \
static className *_instance; \
+ (id)allocWithZone:(NSZone *)zone \
{ \
static dispatch_once_t onceToken; \
dispatch_once(&onceToken, ^{ \
_instance = [super allocWithZone:zone]; \
}); \
return _instance; \
} \
+ (className *)shared##className \
{ \
static dispatch_once_t onceToken; \
dispatch_once(&onceToken, ^{ \
_instance = [[self alloc] init]; \
}); \
return _instance; \
}
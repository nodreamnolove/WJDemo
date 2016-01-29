//
//  MyWebVC.m
//  WJDemo
//
//  Created by 段瑞权 on 16/1/29.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "MyWebVC.h"
#import <WebKit/WebKit.h>


@interface MyWebVC ()<UIWebViewDelegate>

@property (nonatomic,strong) UIWebView * mywebview;

@end

@implementation MyWebVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.automaticallyAdjustsScrollViewInsets = NO;
    [self.view addSubview:self.mywebview];
 
//    [[UINavigationBar appearance]setTintColor:[UIColor redColor]];
}

-(UIWebView *)mywebview
{
    if (_mywebview == nil) {
        _mywebview = [[UIWebView alloc]initWithFrame:CGRectMake(0, 64, ScreenWidth, ScreenHigth-64)];
        _mywebview.scalesPageToFit = YES;
    }
    return _mywebview;
}

-(void)setUrlstring:(NSString *)urlstring
{
    _urlstring = urlstring;
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:urlstring] cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:60];
    [self.mywebview loadRequest:request];
}

-(void)setUrltitle:(NSString *)urltitle
{
    _urltitle = urltitle;
    self.title = urltitle;
}
-(void)webViewDidStartLoad:(UIWebView *)webView
{
    NSLog(@"%s",__func__);
}
-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    NSLog(@"%s",__func__);
}
-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"%s",__func__);
}

@end

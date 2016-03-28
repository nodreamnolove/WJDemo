//
//  HomePageVC.m
//  WJDemo
//
//  Created by hmh on 16/1/27.
//  Copyright © 2016年 WanJi. All rights reserved.
//

#import "HomePageVC.h"
#import "MyWebVC.h"
#import "UIView+HMH.h"
#import "ObuSDK.h"
#import "MBProgressHUD+MJ.h"
#import "MBProgressHUD+HMH.h"
#import "CardTransactionRecord.h"
#import "CardOwnerRecord.h"
#import "CardConsumeRecord.h"
#import "AsyncSocket.h"
#import "NetFunction.h"
#import "AppDelegate.h"
//#import "NSString+NSStringHexToBytes.h"
#import <CoreBluetooth/CoreBluetooth.h>
//#include "issue_Ble_send.h"
#import "DisplayViewController.h"
#import "HttpGetTool.h"

#define PicNum  3
#define ScrollNum 3600


@interface HomePageVC ()<UICollectionViewDataSource,UICollectionViewDelegateFlowLayout,UIScrollViewDelegate,CBCentralManagerDelegate,UITableViewDataSource,UIAlertViewDelegate>
{
    NSString *OBUID;
}

@property (nonatomic,strong) UIScrollView * scrollView;

@property (nonatomic,strong) UIPageControl * pageControl;

@property (nonatomic, strong) UICollectionView *collectionView;

//@property (nonatomic, strong) UIButton *transCommandBtn;

@property (nonatomic,assign) int   currentPage;

@property (nonatomic,assign) int   lastPage;

@property (nonatomic,assign) int   nextPage;

@property (nonatomic,strong) NSArray * picArr;

@property (nonatomic,strong) UIImageView * imageView0;

@property (nonatomic,strong) UIImageView * imageView1;

@property (nonatomic,strong) UIImageView * imageView2;

@property (nonatomic,strong) NSTimer * gundongTimer;

@property (nonatomic, strong) NSArray *titleArr;

@property (nonatomic, strong) ObuSDK *myObu;

@property (nonatomic, strong) UIView *displayView;

@property (nonatomic, strong) NSMutableArray *infosArr;

@property (nonatomic,strong) UITextField * moneyField;
@end

@implementation HomePageVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"首页";
    self.automaticallyAdjustsScrollViewInsets = NO;
    self.currentPage = (ScrollNum/2);
    self.lastPage = (ScrollNum/2)-1;
    self.nextPage = (ScrollNum/2)+1;
    UITapGestureRecognizer *imageViewTap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(imageViewClick:)];
    self.imageView0 = [[UIImageView alloc]initWithFrame:CGRectMake(ScreenWidth*(ScrollNum/2), 0, ScreenWidth, ScreenWidth/4)];
    self.imageView0.userInteractionEnabled = YES;
    self.imageView0.image = [UIImage imageNamed:@"obu"];
    self.imageView0.tag = self.currentPage%PicNum;
    [self.imageView0 addGestureRecognizer:imageViewTap];
    self.imageView0.userInteractionEnabled = YES;
    
    UITapGestureRecognizer *imageViewTap1 = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(imageViewClick:)];
    self.imageView1 = [[UIImageView alloc]initWithFrame:CGRectMake(ScreenWidth*(ScrollNum/2+1), 0, ScreenWidth, ScreenWidth/4)];
    self.imageView1.userInteractionEnabled = YES;
    self.imageView1.image = [UIImage imageNamed:@"chen"];
    self.imageView1.tag = self.nextPage%PicNum;
     [self.imageView1 addGestureRecognizer:imageViewTap1];
    self.imageView1.userInteractionEnabled = YES;
    
    UITapGestureRecognizer *imageViewTap2 = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(imageViewClick:)];
    self.imageView2 = [[UIImageView alloc]initWithFrame:CGRectMake(ScreenWidth*(ScrollNum/2-1), 0, ScreenWidth, ScreenWidth/4)];
    self.imageView2.userInteractionEnabled = YES;
    self.imageView2.image = [UIImage imageNamed:@"wenhua"];
    self.imageView2.tag = self.lastPage%PicNum;
    [self.imageView2 addGestureRecognizer:imageViewTap2];
    
    
    self.scrollView.contentOffset = CGPointMake((ScrollNum/2)*ScreenWidth, 0);
    [self.scrollView addSubview:self.imageView0];
    [self.scrollView addSubview:self.imageView1];
    [self.scrollView addSubview:self.imageView2];
    
//    [self.view addSubview:self.transCommandBtn];
    
    [self.view addSubview:self.scrollView];
    [self.view addSubview:self.pageControl];
    [self.view addSubview:self.collectionView];
    self.gundongTimer = [NSTimer scheduledTimerWithTimeInterval:3.0 target:self selector:@selector(scrollTimer) userInfo:nil repeats:YES];

   

}
-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [MBProgressHUD hideHUD];
}

-(NSMutableArray *)infosArr
{
    if (_infosArr==nil) {
        _infosArr = [NSMutableArray array];
    }
    return _infosArr;
}
-(NSArray *)titleArr
{
    if (_titleArr == nil) {
        _titleArr = @[@"蓝牙连接", @"蓝牙断开", @"读卡信息", @"读OBU信息", @"圈存初始化", @"圈存", @"交易记录", @"消费过程", @"持卡人信息",@"透传指令"];
    }
    return _titleArr;
}



-(UIScrollView *)scrollView
{
    if (_scrollView == nil) {
        _scrollView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 64, ScreenWidth, ScreenWidth/4)];
        _scrollView.contentSize = CGSizeMake(ScreenWidth*ScrollNum, ScreenWidth/4);
        _scrollView.backgroundColor = [UIColor clearColor];
        _scrollView.pagingEnabled = YES;
        _scrollView.showsHorizontalScrollIndicator = NO;
        _scrollView.showsVerticalScrollIndicator = NO;
        _scrollView.delegate = self;
        _scrollView.bounces = NO;
        _scrollView.tag = 99;
    }
    return _scrollView;
}

-(UIPageControl *)pageControl
{
    if (_pageControl == nil) {
        _pageControl = [[UIPageControl alloc]initWithFrame:CGRectMake(ScreenWidth/2-50, 64+ScreenWidth/4-20, 100, 10)];
        _pageControl.numberOfPages = PicNum;
        _pageControl.currentPage = 0;
        [_pageControl addTarget:self action:@selector(pageChange:) forControlEvents:UIControlEventValueChanged];
    }
    return _pageControl;
}

-(UICollectionView *)collectionView
{
    if (_collectionView == nil) {
        UICollectionViewFlowLayout *fallLayout =[[UICollectionViewFlowLayout alloc]init];
        [fallLayout setScrollDirection:UICollectionViewScrollDirectionVertical];
       
        _collectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(self.scrollView.frame)+2, ScreenWidth, ScreenHigth-CGRectGetMaxY(self.scrollView.frame)-60) collectionViewLayout:fallLayout];
        _collectionView.dataSource = self;
        _collectionView.delegate = self;
        _collectionView.backgroundColor = [UIColor whiteColor];
        [_collectionView registerClass:[UICollectionViewCell class] forCellWithReuseIdentifier:@"homePageCell"];
    }
    return _collectionView;
}

//-(UIButton *)transCommandBtn
//{
//    if (_transCommandBtn==nil) {
//        _transCommandBtn = [[UIButton alloc]initWithFrame:CGRectMake(20, CGRectGetMaxY(self.collectionView.frame), ScreenWidth-40, 38)];
//        [_transCommandBtn setTitle:@"数据透传" forState:UIControlStateNormal];
//        [_transCommandBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
//        _transCommandBtn.backgroundColor = [UIColor grayColor];
//        [_transCommandBtn addTarget:self action:@selector(transCommandAction:) forControlEvents:UIControlEventTouchUpInside];
//    }
//    return _transCommandBtn;
//}

#pragma mark scrollect 代理
-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (scrollView.tag == 99) {
    int page = floor((scrollView.contentOffset.x - ScreenWidth/2)/ScreenWidth)+1;
    self.pageControl.currentPage = page%PicNum;
    UIImageView *lastView;
    CGFloat nextViewx;
//    NSLog(@"page= %d,currentPage=%d,lastpage=%d",page,self.currentPage,self.lastPage);
    if ((page)!=self.currentPage) {
        
        if(page > self.currentPage)
        {
            if (self.currentPage > self.lastPage)  //1.正向
            {
                lastView = [scrollView viewWithTag:self.lastPage%PicNum];
                self.nextPage = (page+1)%ScrollNum;
                nextViewx = self.nextPage * ScreenWidth;
            }
            else                                //4.先反向后正向
            {
                lastView = [scrollView viewWithTag:(self.nextPage)%PicNum];
                self.nextPage = (page + 1)%ScrollNum;
                nextViewx =  self.nextPage*ScreenWidth;
            }
        }
       else if (page < self.currentPage)
       {
            if (self.lastPage > self.currentPage) //2.反向
            {
                lastView = [scrollView viewWithTag:self.lastPage%PicNum];
                self.nextPage = (page-1+ScrollNum)%ScrollNum;
                nextViewx =  self.nextPage* ScreenWidth;
            }
            else                                //3.先正向后反向
            {
                lastView = [scrollView viewWithTag:(self.nextPage+PicNum)%PicNum];
                self.nextPage = (page-1+ScrollNum)%ScrollNum;
                nextViewx =  self.nextPage*ScreenWidth;
                
            }
        }
        [lastView setViewX:nextViewx];
        self.lastPage = self.currentPage;
        self.currentPage = page;
        if (self.currentPage == 0|| self.currentPage == ScrollNum-1) {
            self.nextPage = ScrollNum/2+1;
            self.currentPage = ScrollNum/2;
            self.lastPage = ScrollNum/2-1;
            self.scrollView.contentOffset = CGPointMake(ScreenWidth*(ScrollNum/2), ScreenWidth/4);
            [self.imageView0 setViewX:ScreenWidth*(ScrollNum/2) ];
            [self.imageView1 setViewX:ScreenWidth*(ScrollNum/2+1)];
            [self.imageView2 setViewX:ScreenWidth*(ScrollNum/2-1)];
            self.pageControl.currentPage = self.currentPage%PicNum;
        }
    }
    }
}

-(void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    if (scrollView.tag == 99) {
         [self.gundongTimer setFireDate:[NSDate distantFuture]];
    }
   
}

-(void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    if (scrollView.tag == 99) {
        timeCount = self.currentPage;
        [self.gundongTimer setFireDate:[[NSDate date] dateByAddingTimeInterval:3]];
    }
    
}

#pragma mark collection代理
-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return 10;
}


- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    UICollectionViewCell *collectCell = [collectionView dequeueReusableCellWithReuseIdentifier:@"homePageCell" forIndexPath:indexPath];
    for (UIView *subview in collectCell.contentView.subviews) {
        [subview removeFromSuperview];
    }
    collectCell.backgroundColor = [UIColor lightGrayColor];
    UILabel *titleLabel = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 100, 100)];
    titleLabel.text = self.titleArr[indexPath.row];
    titleLabel.textColor = [UIColor blackColor];
    titleLabel.textAlignment = NSTextAlignmentCenter;
    titleLabel.numberOfLines = 0;
    [collectCell.contentView addSubview:titleLabel];
    return collectCell;
}

//cell大小
-(CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    return CGSizeMake(100, 100);
    
}

#pragma mark 

-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    NSString *tintString;
    __weak typeof(self) weakSelf = self;
    //连接
    if (indexPath.row == 0) {
        tintString = @"连接OBU";
        self.myObu = [ObuSDK sharedObuSDK];
            if (self.myObu) {
                [MBProgressHUD showMessage:tintString toView:nil];
                [self.myObu connectDevice:20 callBack:^(BOOL status, NSObject *data, NSString *errorMsg) {
                     dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    if (status==NO) {
                            [MBProgressHUD showError:[NSString stringWithFormat:@"连接失败:%@",errorMsg]];
                    }
                    else if (status == YES){
                        [MBProgressHUD showSuccess:@"连接成功"];
                        DisplayViewController *display = [DisplayViewController new];
                        display.title = @"连接信息";
                        display.objdata = data;
                        display.hidesBottomBarWhenPushed = YES;
                        [weakSelf.navigationController pushViewController:display animated:YES];
                        
//                            UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"连接成功" message: [NSString stringWithFormat:@"%@",data] delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
//                            [alert show];
    //                        [MBProgressHUD showInfo:data toView:nil];
                        
                    }
                
                     });
                }];
        }//断开
    }
    else if(indexPath.row == 1){
        tintString = @"断开OBU";
//       self.myObu = [ObuSDK sharedObuSDK];
        if (self.myObu)
        {
            [MBProgressHUD showMessage:tintString toView:nil];
            
          
            [self.myObu disconnectDevice:^(BOOL status, NSObject *data, NSString *errorMsg) {
                 dispatch_async(dispatch_get_main_queue(), ^{
                [MBProgressHUD hideHUD];
                if (status == YES) {
                    [MBProgressHUD showSuccess:@"断开成功"];
                }
                else
                {
                    [MBProgressHUD showSuccess:@"断开成功"];
                }
              });
           }];
        }
    }//读卡
    else if(indexPath.row == 2){
        tintString = @"读取IC卡信息";
        if (self.myObu) {
            [MBProgressHUD showMessage:tintString toView:nil];
            [self.myObu getCardInformation:^(BOOL status, NSObject *data, NSString *errorMsg) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title;
                    NSString *message;
                    if (status) {
                        title = @"读取卡片信息成功";
                        message = [NSString stringWithFormat:@"%@",data];
                        DisplayViewController *display = [DisplayViewController new];
                        display.title = @"IC卡信息";
                        display.objdata = data;
                        display.hidesBottomBarWhenPushed = YES;
                        [self.navigationController pushViewController:display animated:YES];
                    }
                    else
                    {
                        title = @"读取卡片信息失败";
                        message = [NSString stringWithFormat:@"%@",errorMsg];
                        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                        [alert show];
                    }
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }//读OBU
    else if (indexPath.row == 3){
        tintString = @"读取OBU信息";
        if(self.myObu){
            [MBProgressHUD showMessage:tintString toView:nil];
            [self.myObu getObuInformation:^(BOOL status, id data, NSString *errorMsg) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title;
                    NSString *message;
                    if (status) {
                        DisplayViewController *display = [DisplayViewController new];
                        display.title = @"OBU信息";
                        display.objdata = data;
                        display.hidesBottomBarWhenPushed = YES;
                        [weakSelf.navigationController pushViewController:display animated:YES];
                        title = @"读取OBU成功";
                        message = [NSString stringWithFormat:@"%@",data];
                    }
                    else{
                        title = @"读取OBU失败";
                        message = errorMsg;
                        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                        [alert show];
                        }
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }
    else if (indexPath.row == 4){
        tintString = @"圈存初始化";
        [self qunchunInit];
//        if(self.myObu){
//            [MBProgressHUD showMessage:tintString toView:nil];
//            [self.myObu loadCreditGetMac1:@"1" cardId:@"00000000000000000001" terminalNo:@"" picCode:@"123456" procType:@"02" keyIndex:@"01" callBack:^(BOOL status, id data, NSString *errorMsg) {
//                dispatch_async(dispatch_get_main_queue(), ^{
//                    [MBProgressHUD hideHUD];
//                    NSString *title;
//                    NSString *message;
//                    if (status) {
//                        DisplayViewController *display = [DisplayViewController new];
//                        display.title = @"圈存初始化成功";
//                        display.hidesBottomBarWhenPushed = YES;
//                        display.objdata = data;
//                        [weakSelf.navigationController pushViewController:display animated:YES];
//                    }
//                    else{
//                        title = @"圈存初始化失败";
//                        message = errorMsg;
//                        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
//                        [alert show];
//                    }
//                });
//            }];
//        }else
//            [MBProgressHUD showError:@"请先连接设备"];
    }
    else if (indexPath.row == 5){
        tintString = @"正在圈存";
        if(self.myObu){
            [MBProgressHUD showMessage:tintString toView:nil];
            
            [self.myObu loadCreditWriteCard:@"2016012713281112346111" callBack:^(BOOL status, id data, NSString *errorMsg) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title;
                    NSString *message;
                    if (status) {
                        title = @"圈存成功";
                        message = [NSString stringWithFormat:@"%@",data];
                    }
                    else{
                        title = @"圈存失败";
                        message = errorMsg;
                    }
                    
                    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                    [alert show];
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }
    else if (indexPath.row == 6){
        tintString = @"读取交易记录";
        if(self.myObu){
            [MBProgressHUD showMessage:tintString toView:nil];
            [self.myObu readCardTransactionRecord:@"123456" maxNumber:1 callBack:^(BOOL status, id data, NSString *errorMsg) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title=@"";
                    NSString *message;
                    if (status) {
                        title = @"读取交易记录成功";
                        NSArray *arrs = data;
                        for (CardTransactionRecord *record in arrs) {
                            NSString *infos = [NSString stringWithFormat:@"onlineSn:%@,overdrawLimit:%@,transAmount:%@,transType:%@,terminalNo:%@,transDate:%@,transTime:%@\n",record.onlineSn,record.overdrawLimit,record.transAmount,record.transType,record.terminalNo,record.transDate,record.transTime];
                             
                            message = [NSString stringWithFormat:@" %@\n%@",message,infos];
                        }
                        DisplayViewController *display = [DisplayViewController new];
                        display.title = @"交易记录信息";
                        display.objdata = data;
                        display.hidesBottomBarWhenPushed = YES;
                        [weakSelf.navigationController pushViewController:display animated:YES];
                    }
                    else{
                        title = @"读取交易记录失败";
                        message = errorMsg;
                    
                    
                    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                    [alert show];
                    }
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }
    else if (indexPath.row == 7){
        tintString = @"读取消费记录";
        if(self.myObu){
            [MBProgressHUD showMessage:tintString toView:nil];
            [self.myObu readCardConsumeRecord:1 callBack:^(BOOL status, id data, NSString *errorMsg) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title;
                    NSString *message;
                    if (status) {
                        title = @"读取消费记录成功";
                        NSArray *recordArr = data;
                        CardConsumeRecord *record = recordArr[0];
                        message = [NSString stringWithFormat:@"applicationId:%@,recordLength:%@,applicationLockFlag:%@,tollRoadNetworkId:%@,tollStationId:%@,tollLaneId:%@,timeUnix:%@,vehicleModel:%@,passStatus:%@,staffNo:%@,mtcSequenceNo:%@,vehicleNumber:%@,reserve1:%@,reserve2:%@",record.applicationId,record.recordLength,record.applicationLockFlag,record.tollRoadNetworkId,record.tollStationId,record.tollLaneId,record.timeUnix,
                                   record.vehicleModel,record.passStatus,record.staffNo,record.mtcSequenceNo,record.vehicleNumber,record.reserve1,record.reserve2];
//                        @property(nonatomic, copy)NSString* tollStationId; // 入/出口收费站号
//                        
//                        @property(nonatomic, copy)NSString* tollLaneId; // 入/出口收费车道号
//                        
//                        @property(nonatomic, copy)NSString* timeUnix; // 入/出口时间 UNIX时间
//                        
//                        @property(nonatomic, copy)NSString* vehicleModel; //车型
//                        
//                        @property(nonatomic, copy)NSString* passStatus; // 入出口状态
//                        
//                        @property(nonatomic, copy)NSString* reserve1; //保留字节1
//                        
//                        @property(nonatomic, copy)NSString* staffNo; //收费员工号二进制方式存放入口员工号后六位
//                        
//                        @property(nonatomic, copy)NSString* mtcSequenceNo; //收费员工号二进制方式存放入口员工号后六位
//                        
//                        @property(nonatomic, copy)NSString* vehicleNumber; //车牌号码
//                        
//                        @property(nonatomic, copy)NSString* reserve2;
                        DisplayViewController *display = [DisplayViewController new];
                        display.title = @"消费记录信息";
                        display.objdata = data;
                        display.hidesBottomBarWhenPushed = YES;
                        [weakSelf.navigationController pushViewController:display animated:YES];
                    }
                    else{
                        title = @"读取消费记录失败";
                        message = errorMsg;
                   
                    
                    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                    [alert show];
                         }
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }
    else if (indexPath.row == 8){
        tintString = @"读取持卡人信息";
        if(self.myObu){
            [MBProgressHUD showMessage:tintString toView:nil];
            [self.myObu readCardOwnerRecord:^(BOOL status, id data, NSString *errorMsg) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title;
                    NSString *message;
                    if (status) {
                        title = @"读取持卡人信息成功";
                        CardOwnerRecord *record = data;
                        
                        message = [NSString stringWithFormat:@"ownerId:%@,staffId:%@,ownerName:%@,ownerLicenseNumber:%@,ownerLicenseType:%@",record.ownerId,record.staffId,record.ownerName,record.ownerLicenseNumber,record.ownerLicenseType];
                        DisplayViewController *display = [DisplayViewController new];
                        display.title = @"持卡人信息信息";
                        display.objdata = @[data];
                        display.hidesBottomBarWhenPushed = YES;
                        [weakSelf.navigationController pushViewController:display animated:YES];
                    }
                    else{
                        title = @"读取持卡人信息失败";
                        message = errorMsg;
                        UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                        [alert show];
                        }
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }
    else if(indexPath.row == 9)
    {
        Byte reqBytes[8]={0x07,0x00,0xa4,0x00,0x00,0x02,0x3f,0x00};
        NSData *reqData = [NSData dataWithBytes:reqBytes length:8];
        if(self.myObu){
            [MBProgressHUD showMessage:@"发送透传数据" toView:nil];
            [self.myObu transCommand:reqData callBack:^(BOOL status, id data, NSString *errorMsg)  {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [MBProgressHUD hideHUD];
                    NSString *title;
                    NSString *message;
                    if (status) {
                        title = @"透传成功";
                        message = [NSString stringWithFormat:@"%@",data];
                    }
                    else
                    {
                        title = @"透传失败";
                        message = [NSString stringWithFormat:@"%@",errorMsg];
                        
                    }
                    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
                    [alert show];
                });
            }];
        }else
            [MBProgressHUD showError:@"请先连接设备"];
    }
}

#pragma mark 网络连接
-(void)getMacByNet
{
//    AppDelegate *app = [UIApplication sharedApplication].delegate;
//    
//    if (app.connection) {
//        FrameQuanCunInitRq  data_rq;
//        NSData *userName = [@"shaocong" hexToBytes];
//        uint8 username[22] = {115,104,97,111,99,104,111,110,103,00};
//        uint8 Addmoney[2] = {0x00,0x00,0x03,0xe8};
//        getQuanCunInit_Rq_Ble(&data_rq, username,Addmoney,2, 1, 1000);
//        
//    
//    }
}


#pragma mark 点击pagecontrol

-(void)pageChange:(id)sender
{
    
}
static int timeCount = ScrollNum/2;
-(void)scrollTimer
{
    timeCount++;
    if (timeCount == ScrollNum-1 ) {
        timeCount = ScrollNum/2;
    }
    [self.scrollView scrollRectToVisible:CGRectMake(timeCount*ScreenWidth, 0, ScreenWidth, ScreenHigth) animated:YES];
    
    
}
//-(void)transCommandAction:(id)sender
//{
//    
//    byte reqBytes[8]={0x07,0x00,0xa4,0x00,0x00,0x02,0x3f,0x00};
//    NSData *reqData = [NSData dataWithBytes:reqBytes length:8];
//    if(self.myObu){
//        [MBProgressHUD showMessage:@"发送透传数据" toView:nil];
//    [self.myObu transCommand:reqData callBack:^(BOOL status, id data, NSString *errorMsg)  {
//        dispatch_async(dispatch_get_main_queue(), ^{
//            [MBProgressHUD hideHUD];
//            NSString *title;
//            NSString *message;
//            if (status) {
//                title = @"透传成功";
//                message = [NSString stringWithFormat:@"%@",data];
//            }
//            else
//            {
//                title = @"透传失败";
//                message = [NSString stringWithFormat:@"%@",errorMsg];
//                
//            }
//            UIAlertView *alert = [[UIAlertView alloc]initWithTitle:title message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles: nil];
//            [alert show];
//        });
//    }];
//    }else
//        [MBProgressHUD showError:@"请先连接设备"];
//}

-(void)imageViewClick:(UITapGestureRecognizer *)ges
{
   
    //  测试
    NSString *urlstring;
    NSString *webTitle;
    if (ges.view.tag == (ScrollNum/2)%PicNum) {
        urlstring = @"http://www.wanji.net.cn/d/a/";
        webTitle = @"短程通信";
    }
    else if(ges.view.tag == (ScrollNum/2+1)%PicNum){
        urlstring = @"http://www.wanji.net.cn/d/b/20100824/368.html";
        webTitle = @"动态称重";
    }
    else if(ges.view.tag == (ScrollNum/2-1)%PicNum){
        urlstring = @"http://www.wanji.net.cn/b/b/";
        webTitle = @"公司文化";
    }
    if (urlstring.length>0) {
        MyWebVC *webVC = [MyWebVC new];
        webVC.urlstring = urlstring;
        webVC.title = webTitle;
        webVC.hidesBottomBarWhenPushed = YES;
        UIBarButtonItem *back = [[UIBarButtonItem alloc]initWithTitle:@"返回" style:UIBarButtonItemStylePlain target:nil action:nil];        
        self.navigationItem.backBarButtonItem = back;
        [self.navigationController pushViewController:webVC animated:YES];
    }
    
}


#pragma mark 圈存初始化

- (void)qunchunInit{
    
    NSString *userName = [AppDelegate instance].userName;
    if (!userName || userName.length < 1) {
        [MBProgressHUD showError:@"您还未登录"];
        return;
    }
    UIView *backView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, ScreenWidth, ScreenHigth)];
    backView.backgroundColor = [[UIColor lightGrayColor] colorWithAlphaComponent:0.5];
    [self.view addSubview:backView];
    UIView *alertView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 200, 120)];
    alertView.layer.cornerRadius = 4.0;
    alertView.layer.masksToBounds = YES;
    alertView.layer.borderWidth = 1.0;
    alertView.layer.borderColor = [UIColor purpleColor].CGColor;
    alertView.backgroundColor = [UIColor whiteColor];
    alertView.center = CGPointMake(self.view.center.x, self.view.center.y-50);
    [backView addSubview:alertView];
   
    UILabel *titleLabel = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 200, 20)];
    titleLabel.textColor = [UIColor blackColor];
    titleLabel.font = [UIFont systemFontOfSize:17];
    titleLabel.text = @"圈存金额";
    titleLabel.textAlignment = NSTextAlignmentCenter;
    [alertView addSubview:titleLabel];
    
    UITextField *inputMoney = [[UITextField alloc]initWithFrame:CGRectMake(0, 30, 200, 30)];
    inputMoney.placeholder = @"请输入圈存金额";
    self.moneyField = inputMoney;
    [alertView addSubview:inputMoney];
    
    UIButton *cancelBtn = [[UIButton alloc]initWithFrame:CGRectMake(0, 70, 100, 50)];
    [cancelBtn setTitle:@"取消" forState:UIControlStateNormal];
    cancelBtn.titleLabel.font = [UIFont systemFontOfSize:17];
    [cancelBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [cancelBtn addTarget:self action:@selector(btnCancelAction:) forControlEvents:UIControlEventTouchUpInside];
    cancelBtn.backgroundColor = [UIColor grayColor];
    cancelBtn.layer.borderWidth = 1.0;
    cancelBtn.layer.borderColor = [UIColor blackColor].CGColor;
    UIButton *sureBtn = [[UIButton alloc]initWithFrame:CGRectMake(100, 70, 100, 50)];
    [sureBtn setTitle:@"确认" forState:UIControlStateNormal];
    sureBtn.titleLabel.font = [UIFont systemFontOfSize:17];
    [sureBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [sureBtn addTarget:self action:@selector(btnSureAction:) forControlEvents:UIControlEventTouchUpInside];
    sureBtn.backgroundColor = [UIColor grayColor];
    sureBtn.layer.borderWidth = 1.0;
    sureBtn.layer.borderColor = [UIColor blackColor].CGColor;
    [alertView addSubview:cancelBtn];
    [alertView addSubview:sureBtn];
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapAction:)];
    [backView addGestureRecognizer:tap];
 
 }

- (void)btnCancelAction:(id)sender{
    
    UIView *maskView = [[sender superview]superview];
    [maskView removeFromSuperview];
}

- (void)btnSureAction:(id)sender{
    
    NSString *moneyCash = self.moneyField.text;
    if ([moneyCash floatValue]<0) {
        [MBProgressHUD showError:@"请输入正确金额"];
        return ;
    }
    UIView *maskView = [[sender superview]superview];
    [maskView removeFromSuperview];
    NSString *userName = [AppDelegate instance].userName;
    NSDate *today = [NSDate date];
    NSDateFormatter *format = [[NSDateFormatter alloc]init];
    format.dateFormat = @"yyyy-MM-dd HH:mm:ss";
    NSString  *curDayString = [format stringFromDate:today];
    NSString *requestFrameString = @"";
    NSMutableDictionary * frameDict = [NSMutableDictionary dictionary];
    
    [frameDict setObject:moneyCash forKey:@"QCAmount"];
    [frameDict setObject:userName forKey:@"CustomerAccount"];
    [frameDict setObject:curDayString forKey:@"TimeNode"];
    [frameDict setObject:requestFrameString  forKey:@"RequestFrame"];
    __weak typeof(self) weakSelf = self;
    [HttpGetTool OrderFrame:frameDict andSuccess:^(NSDictionary *dict) {
        //解析返回值
        //.......
        
        if(weakSelf.myObu){
            //填充返回值
            [weakSelf.myObu loadCreditGetMac1:moneyCash cardId:@"" terminalNo:@"" picCode:@"" procType:@"" keyIndex:@"" callBack:^(BOOL status, id data, NSString *errorMsg) {
                
            }];
        }
        
    } andFailure:^{
        
    }];
    
}

- (void)tapAction:(UITapGestureRecognizer *)gest{
    
    [gest.view removeFromSuperview];
    
}






@end
